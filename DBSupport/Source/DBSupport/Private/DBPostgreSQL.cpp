#include "DBPostgreSQL.h"
#include "DBSupportPrivatePCH.h"


#ifdef DBSUPPORT_PGSQL

#include "comm_exception.h"
#include <sstream>

using namespace DBBase;

bool DBBase::PGConnection::Connect(string host,string dbname, string user, string password, string Encoding, int port)
{
		char pConnString[256];
		sprintf(pConnString,"hostaddr = %s dbname=%s user=%s password=%s",host.c_str(),dbname.c_str(),user.c_str(),password.c_str());
		m_pConn=PQconnectdb(pConnString);

		ConnStatusType code = PQstatus(m_pConn);
		if(code!=CONNECTION_OK)
		{
			DisConnect();
			
			string err;
			err ="���ݿ�����ʧ�ܣ�ԭ��:";
			err += PQerrorMessage(m_pConn);
			THROW_EX(err);
			return false;
		}

		PQsetClientEncoding(m_pConn, Encoding.c_str());

		return true;
}

bool DBBase::PGConnection::DisConnect()
{
	if(m_pPtr)
	{
		delete m_pPtr;
		m_pPtr = NULL;
	}

	if(m_pConn)
	{
		PQfinish(m_pConn);
		m_pConn = NULL;
	}

	return true;
}

bool DBBase::PGConnection::IsOpen()
{
	if(!m_pConn) return false;

	ConnStatusType code = PQstatus(m_pConn);
	if(code==CONNECTION_OK)
		return true;
	else
		return false;
}

bool DBBase::PGConnection::Execute(string SQL)
{
	if (SQL.empty())
	{
		return true;
	}
	//modify bu zhouhang 20151119 
	PGresult * pRes = PQexec(m_pConn, SQL.c_str());

	if (PQresultStatus(pRes) != PGRES_COMMAND_OK&& PQresultStatus(pRes) != PGRES_TUPLES_OK)
	{
		string err;
		err = "��ѯʧ�ܣ�ԭ��";
		err += PQerrorMessage(m_pConn);
		PQclear(pRes);
		THROW_EX(err);

		return false;
	}
	PQclear(pRes);

	return true;
}

IRecordSet * DBBase::PGConnection::Query(string SQL)
{
	PGresult * pRes = PQexec(m_pConn, SQL.c_str());

	if (PQresultStatus(pRes) != PGRES_TUPLES_OK)
	{
		string err;
		err += "��ѯʧ�ܣ�ԭ��";
		err += PQerrorMessage(m_pConn);
		PQclear(pRes);
		THROW_EX(err);
		return NULL;
	}

	PGRecordSet *recordSet = new PGRecordSet(pRes);
	recordSet->MoveFirst();

	return recordSet;
}

DBBase::PGConnection::PGConnection()
{

	init();
}

DBBase::PGConnection::PGConnection( PGconn * pConn )
{
	init(); 
	m_pConn = pConn;
}
DBBase::PGConnection::~PGConnection()
{
}

void DBBase::PGConnection::init()
{
	m_pConn = NULL;
	m_pPtr = NULL;
}

void DBBase::PGConnection::BeginTrans()
{
	string strSQL = "BEGIN;";
	string strSQL2 = "SAVEPOINT anser_savepoint;";
	Execute(strSQL);
	Execute(strSQL2);
}

void DBBase::PGConnection::CommitTrans()
{
	string strSQL = "COMMIT;";
	Execute(strSQL);
}

void DBBase::PGConnection::RollBackTrans()
{
	string strSQL = "ROLLBACK TO anser_savepoint;";
	Execute(strSQL);
}

IConnectionPtr* DBBase::PGConnection::ConnectionPtr()
{
	if(!m_pPtr)
		m_pPtr = new PGConnectionPtr();

	m_pPtr->pPtr = m_pConn;
	return m_pPtr;
}

int DBBase::PGConnection::SetClientEncoding( const char * encode )
{
	return PQsetClientEncoding(m_pConn, encode);
}

bool DBBase::PGConnection::IsTableExist( string TableName )
{
	try
	{
		string strSQL = "SELECT 0 FROM ";
		strSQL += TableName;
		strSQL += " LIMIT 1";
		Query(strSQL)->Close();
	}
	catch(comm_exception & )
	{
		return false;
	}

	return true;
}

unsigned int DBBase::PGRecordSet::RecordCount()
{
	return PQntuples(m_Data);
}

bool DBBase::PGRecordSet::IsEOF()
{
	if(!m_Data) return true;
	if(PQresultStatus(m_Data) != PGRES_TUPLES_OK) return true;
	if(m_CurrentRecord >= RecordCount()) return true;

	return false;
}

void DBBase::PGRecordSet::MoveFirst()
{
	m_CurrentRecord = 0;
}

void DBBase::PGRecordSet::MoveNext()
{
	++m_CurrentRecord;
}

int DBBase::PGRecordSet::GetIntegerValue( string fieldName )
{
	string res = GetStringValue(fieldName);
	if(res == "") 
	{
		stringstream err;
		err<< "��ѯ��ȡ����ֵ.";
		THROW_EX(err.str());
		err.str("");

		return -1;
	}

	int iRes = atoi(res.c_str());
	return iRes;
}

__int64 DBBase::PGRecordSet::GetLongLongValue( string fieldName )
{
	string res = GetStringValue(fieldName);
	if(res == "") 
	{
		stringstream err;
		err<< "��ѯ��ȡ����ֵ.";
		THROW_EX(err.str());
		err.str("");

		return -1;
	}

	__int64 lRes = _atoi64(res.c_str());

	return lRes;
}

std::string DBBase::PGRecordSet::GetStringValue( string fieldName )
{
	if(IsEOF()) return "";
	int nNum = PQfnumber(m_Data, fieldName.c_str());
	if(nNum < 0)
	{
		string err;
		err = "δ�ڲ�ѯ������ҵ���";
		err += fieldName;
		err +="һ�µ��ֶΣ���������Ƿ�����.";
		THROW_EX(err);
		return "";
	}
	char * cres = PQgetvalue(m_Data, m_CurrentRecord, nNum);
	string res = (!cres ?  "" : cres);

	return res;
}

DBBase::PGRecordSet::PGRecordSet()
{
	init();	
}

DBBase::PGRecordSet::PGRecordSet( PGresult * result )
{
	init();
	m_Data = result;
}
void DBBase::PGRecordSet::Close()
{
	PQclear(m_Data);
	init();

	if(this)
	{
		delete this;
	}
}

void DBBase::PGRecordSet::init()
{
	m_CurrentRecord =0;
	m_Data = NULL;
}

int DBBase::PGRecordSet::GetFieldCount()
{
	if(IsEOF()) return 0;
	return PQnfields(m_Data);
}

std::string DBBase::PGRecordSet::GetFieldName( int idx )
{
	if(IsEOF()) return "";
	if(idx > GetFieldCount() -1) return "";

	return PQfname(m_Data, idx);
}

#endif