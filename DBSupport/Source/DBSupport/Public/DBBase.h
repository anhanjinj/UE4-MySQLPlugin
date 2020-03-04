#pragma once

#include "../Private/DBSupportPrivatePCH.h"
#include <string>

//���ݿ��������ͣ����ڴ�����ͬ����
typedef enum
{
	PG_CONNECTION,		//PG���ݿ�����
	MYSQL_CONNECTION	//MYSQL���ݿ�����
}CONNECTION_TYPE;

namespace DBBase
{
	struct IConnectionPtr
	{
		void * pPtr;

		virtual ~IConnectionPtr(){}
	};

	//��¼�����࣬���Զ������ݿ�֧��;
	class IRecordSet
	{
	public:
		//��ü�¼��
		virtual unsigned int RecordCount() = 0;

		//�Ƿ���Ч
        virtual bool IsEOF() = 0;

		//�ƶ�����һ��¼
		virtual void MoveFirst() = 0;

		//�ƶ�����һ��¼
		virtual void MoveNext() = 0;

		//��ȡ��������
		virtual int	 GetIntegerValue(std::string fieldName) = 0;

		//��ȡ����������
		virtual __int64 GetLongLongValue(std::string fieldName) = 0;

		//��ȡ�ַ�����
		virtual std::string GetStringValue(std::string fieldName) = 0;

		//��ȡ��¼���ֶ���
		virtual int	GetFieldCount() = 0;

		//��ȡ�ֶ�����
		virtual std::string	GetFieldName(int idx) = 0;

		//�ر�һ����¼��
		virtual void Close() = 0;
	};

	//���ݿ����ӻ���

	class IConnection
	{
	public:
		//�������ݿ�
		virtual bool Connect(std::string host, std::string dbname, std::string user,
			std::string password, std::string Encode = "GBK", int port = 3306) = 0;

		//�Ͽ�����
		virtual bool DisConnect() = 0;

		//�Ƿ�����
		virtual bool IsOpen() = 0;

		//ִ��SQL���,�޷��ؽ��
		virtual bool Execute(std::string SQL) = 0;

		//ִ�в�ѯ��䣬���ؼ�¼��;
		virtual IRecordSet* Query(std::string SQL) = 0;

		//��ʼ����
		virtual void BeginTrans() = 0;

		//ִ������
		virtual void CommitTrans() = 0;

		//�ع�����
		virtual void RollBackTrans() = 0;

		//�������ݿ�����ʽ
		virtual int	SetClientEncoding(const char* encode) = 0;

		//ֱ�������ݿ⴫�Σ�
		virtual IConnectionPtr* ConnectionPtr() = 0;

		//�ж����ݿ���ĳ�����Ƿ����
		virtual bool IsTableExist(std::string TableName) = 0;
	};
};

