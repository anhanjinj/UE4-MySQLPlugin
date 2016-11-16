#include "DBSupportPrivatePCH.h"
#include "DBMySQL.h"

#ifdef DBSUPPORT_MYSQL

#include "comm_exception.h"
#include <sstream>

using namespace DBBase;

bool DBBase::MYSQLConnection::Connect(string host,string dbname, string user, string password, string Encoding, int port)
{
	m_pConn = mysql_real_connect(m_pConn, host.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
	
	if (!m_pConn)
	{
		std::string err = "数据库连接失败，原因:";
		err += mysql_error(m_pConn);
		return false;
	}

	mysql_query(m_pConn, "SET NAMES GBK");

	if (m_pPtr)
	{
		DisConnect();
		
	}

	m_pPtr = new MYSQLConnectionPtr();
	m_pPtr->pPtr = m_pConn;

	return true;
}

bool DBBase::MYSQLConnection::DisConnect()
{
	if(m_pPtr)
	{
		delete m_pPtr;
		m_pPtr = NULL;
	}

	m_pConn = nullptr;

	if (this)
	{
		delete this;
	}



	return true;
}

bool DBBase::MYSQLConnection::IsOpen()
{
	if (!m_pConn)
	{
		return false;
	}

	if (0 != mysql_ping(m_pConn))
	{
		return false;
	}

	return true;
}

bool DBBase::MYSQLConnection::Execute(string SQL)
{
	if (SQL.empty())
	{
		return true;
	}
 	int nRes = mysql_query(m_pConn, SQL.c_str());
	if (nRes != 0)
	{
		string err;
		err = "查询失败，原因：";
		err += mysql_error(m_pConn);

		return false;
	}

	return true;
}

IRecordSet * DBBase::MYSQLConnection::Query(string SQL)
{
	if (!IsOpen())
	{
		return nullptr;
	}

	int nErr = mysql_real_query(m_pConn, SQL.c_str(), (unsigned int	)strlen(SQL.c_str()));

	if (nErr == 0)
	{
		MYSQL_RES * result = mysql_store_result(m_pConn);

		if (!result)
		{
			return nullptr;
		}

		return new MYSQLRecordSet(result);
	}
	else
	{
		return new MYSQLRecordSet();
		
	}
		

	return nullptr;
}

DBBase::MYSQLConnection::MYSQLConnection()
{
	init();
}

DBBase::MYSQLConnection::MYSQLConnection(MYSQL * pConn)
{
	init();
	m_pConn = pConn;
}
DBBase::MYSQLConnection::~MYSQLConnection()
{

}

void DBBase::MYSQLConnection::init()
{
 	m_pConn  = mysql_init(NULL);
 	m_pPtr = NULL;
}

void DBBase::MYSQLConnection::BeginTrans()
{
	string strSQL = "BEGIN;";
	Execute(strSQL);
}

void DBBase::MYSQLConnection::CommitTrans()
{
	mysql_commit(m_pConn);
}

void DBBase::MYSQLConnection::RollBackTrans()
{
	mysql_rollback(m_pConn);
}

IConnectionPtr* DBBase::MYSQLConnection::ConnectionPtr()
{
	return m_pPtr;
}

int DBBase::MYSQLConnection::SetClientEncoding( const char * encode )
{
	string strSQL = "SET NAMES ";
	strSQL += encode;

	return (int)Execute(strSQL);
}

bool DBBase::MYSQLConnection::IsTableExist( string TableName )
{
	
	string str1 = "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME =  '" + TableName + "'";
	
	int nRet = mysql_query(m_pConn, str1.c_str());
	if(nRet != 0)
	{
		return false;
	}
	MYSQL_RES* result = mysql_store_result(m_pConn);
	if (result)
	{
		if (result->row_count > 0)
		{
			return true;
		}
	}

	return false;
}

unsigned int DBBase::MYSQLRecordSet::RecordCount()
{
	return  m_Data?  (m_Data->data ?  m_Data->data->rows : 0) : 0;
}

bool DBBase::MYSQLRecordSet::IsEOF()
{
	if (!m_Data)
	{
		return true;
	}
// 	if (m_Data->eof)
// 	{
// 		return true;
// 	}
	if (m_CurrentRecord >= RecordCount())
	{
		return true;
	}

	return false;
}

void DBBase::MYSQLRecordSet::MoveFirst()
{
	m_CurrentRecord = 0;
}

void DBBase::MYSQLRecordSet::MoveNext()
{
	++m_CurrentRecord;
}

int DBBase::MYSQLRecordSet::GetIntegerValue( string fieldName )
{
	string res = GetStringValue(fieldName);
	if(res == "") 
	{
		stringstream err;
		err<< "查询获取到空值.";
		err.str("");

		return -1;
	}

	int iRes = atoi(res.c_str());
	return iRes;
}

__int64 DBBase::MYSQLRecordSet::GetLongLongValue( string fieldName )
{
	string res = GetStringValue(fieldName);
	if (res == "")
	{
		stringstream err;
		err << "查询获取到空值.";
		err.str("");

		return -1;
	}

	__int64 lRes = _atoi64(res.c_str());

	return lRes;
}

std::string DBBase::MYSQLRecordSet::GetStringValue( string fieldName )
{
	if(IsEOF()) return "";
	
	mysql_data_seek(m_Data, m_CurrentRecord);
	MYSQL_ROW  sql_row = mysql_fetch_row(m_Data);
	mysql_field_seek(m_Data, 0);

	for (unsigned idx = 0;  idx < mysql_num_fields(m_Data); ++idx)
	{
		auto fd = mysql_fetch_field(m_Data);

		if (fieldName == fd->name)
		{
			if (sql_row)
			{
				return sql_row[idx];
			}
			else
			{
				return "";
			}
		}
	}

	return "";
}

DBBase::MYSQLRecordSet::MYSQLRecordSet()
{
	init();	
}

DBBase::MYSQLRecordSet::MYSQLRecordSet(MYSQL_RES * result )
{
 	init();
 	m_Data = result;
}
void DBBase::MYSQLRecordSet::Close()
{
	if (m_Data && m_Data->data)
	{
		mysql_free_result(m_Data);		
	}
	init();
	
	if (this)
	{
		delete this;
	}
}

void DBBase::MYSQLRecordSet::init()
{
 	m_CurrentRecord = 0;
 	m_Data = nullptr;
}

int DBBase::MYSQLRecordSet::GetFieldCount()
{
	if(IsEOF()) return 0;

	return m_Data->field_count;
}

std::string MYSQLRecordSet::MYSQLRecordSet::GetFieldName( int idx )
{
	if(IsEOF()) return "";
	if(idx > GetFieldCount() -1) return "";

	return mysql_fetch_field_direct(m_Data, idx)->name;
}
#endif