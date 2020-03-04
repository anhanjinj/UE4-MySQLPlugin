#pragma once

#include "../Public/DBBase.h"

#ifdef DBSUPPORT_MYSQL

using namespace std;

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <mysql.h>
#endif

namespace DBBase
{
	struct  MYSQLConnectionPtr :public IConnectionPtr
	{
		MYSQL *pPtr;

		virtual ~MYSQLConnectionPtr()
		{
			mysql_close(pPtr);
			pPtr = nullptr;
		}
	};


	class MYSQLRecordSet :public IRecordSet
	{
	public:
		MYSQLRecordSet();
		MYSQLRecordSet(MYSQL_RES * result);
		~MYSQLRecordSet(){}

		//��ü�¼��
		virtual unsigned int RecordCount();

		//�Ƿ���Ч
		virtual bool IsEOF();

		//�ƶ�����һ��¼
		virtual void MoveFirst();

		//�ƶ�����һ��¼
		virtual void MoveNext();

		//��ȡ��������
		virtual int	 GetIntegerValue(string fieldName);

		//��ȡ����������
		virtual __int64 GetLongLongValue(string fieldName);

		//��ȡ�ַ�����
		virtual string GetStringValue(string fieldName);

		//��ȡ��¼���ֶ���
		virtual int	GetFieldCount();

		//��ȡ�ֶ�����
		virtual string	GetFieldName(int idx);

		//�ر�һ����¼��
		virtual void Close();
	private:
		void init();
	private:
		//���ݿ��ѯ���
		MYSQL_RES * m_Data;

		//��ǰλ��;
		unsigned m_CurrentRecord;
	};

	class MYSQLConnection : public IConnection
	{
	public:
		MYSQLConnection();
		MYSQLConnection(MYSQL * pConn);
		virtual ~MYSQLConnection();

		//�������ݿ�
		virtual bool Connect(string host,string dbname, string user, string password, string Encoding ="GBK", int port = 3306);

		//�Ͽ�����
		virtual bool DisConnect();

		//�Ƿ�����
		virtual bool IsOpen();

		//ִ��SQL���,�޷��ؽ��
		virtual bool Execute(string SQL);

		//ִ�в�ѯ��䣬���ؼ�¼��;
		virtual IRecordSet * Query(string SQL);

		//��ʼ����
		virtual void BeginTrans();

		//ִ������
		virtual void CommitTrans(); 

		//�ع�����
		virtual void RollBackTrans();

		//�������ݿ�����ʽ
		virtual int	SetClientEncoding(const char * encode);

		//������ݿ�����ָ�룻
		virtual IConnectionPtr *ConnectionPtr();

		//�ж����ݿ���ĳ�����Ƿ����
		virtual bool IsTableExist(string TableName);
	private:
		void init();
	private:
		MYSQL *m_pConn;
		MYSQLConnectionPtr* m_pPtr;
	};
};
#endif