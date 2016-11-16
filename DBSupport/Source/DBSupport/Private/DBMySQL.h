#pragma once

#include "../Public/DBBase.h"

#ifdef DBSUPPORT_MYSQL

using namespace std;

#if PLATFORM_WINDOWS
#include "AllowWindowsPlatformTypes.h"
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

		//获得记录数
		virtual unsigned int RecordCount();

		//是否无效
		virtual bool IsEOF();

		//移动到第一记录
		virtual void MoveFirst();

		//移动到下一记录
		virtual void MoveNext();

		//获取整形数据
		virtual int	 GetIntegerValue(string fieldName);

		//获取长整形数据
		virtual __int64 GetLongLongValue(string fieldName);

		//获取字符数据
		virtual string GetStringValue(string fieldName);

		//获取记录集字段数
		virtual int	GetFieldCount();

		//获取字段名称
		virtual string	GetFieldName(int idx);

		//关闭一个记录集
		virtual void Close();
	private:
		void init();
	private:
		//数据库查询结果
		MYSQL_RES * m_Data;

		//当前位置;
		unsigned m_CurrentRecord;
	};

	class MYSQLConnection : public IConnection
	{
	public:
		MYSQLConnection();
		MYSQLConnection(MYSQL * pConn);
		virtual ~MYSQLConnection();

		//连接数据库
		virtual bool Connect(string host,string dbname, string user, string password, string Encoding ="GBK", int port = 3306);

		//断开连接
		virtual bool DisConnect();

		//是否连接
		virtual bool IsOpen();

		//执行SQL语句,无返回结果
		virtual bool Execute(string SQL);

		//执行查询语句，返回记录集;
		virtual IRecordSet * Query(string SQL);

		//开始事务
		virtual void BeginTrans();

		//执行事务
		virtual void CommitTrans(); 

		//回滚操作
		virtual void RollBackTrans();

		//设置数据库编码格式
		virtual int	SetClientEncoding(const char * encode);

		//获得数据库连接指针；
		virtual IConnectionPtr *ConnectionPtr();

		//判断数据库中某个表是否存在
		virtual bool IsTableExist(string TableName);
	private:
		void init();
	private:
		MYSQL *m_pConn;
		MYSQLConnectionPtr* m_pPtr;
	};
};
#endif