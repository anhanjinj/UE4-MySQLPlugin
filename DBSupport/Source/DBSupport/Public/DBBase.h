#pragma once

#include "../Private/DBSupportPrivatePCH.h"
#include <string>

//数据库连接类型，用于创建不同连接
typedef enum
{
	PG_CONNECTION,		//PG数据库连接
	MYSQL_CONNECTION	//MYSQL数据库连接
}CONNECTION_TYPE;

namespace DBBase
{
	struct IConnectionPtr
	{
		void * pPtr;

		virtual ~IConnectionPtr(){}
	};

	//记录集基类，用以多种数据库支持;
	class IRecordSet
	{
	public:
		//获得记录数
		virtual unsigned int RecordCount() = 0;

		//是否无效
        virtual bool IsEOF() = 0;

		//移动到第一记录
		virtual void MoveFirst() = 0;

		//移动到下一记录
		virtual void MoveNext() = 0;

		//获取整形数据
		virtual int	 GetIntegerValue(std::string fieldName) = 0;

		//获取长整形数据
		virtual __int64 GetLongLongValue(std::string fieldName) = 0;

		//获取字符数据
		virtual std::string GetStringValue(std::string fieldName) = 0;

		//获取记录集字段数
		virtual int	GetFieldCount() = 0;

		//获取字段名称
		virtual std::string	GetFieldName(int idx) = 0;

		//关闭一个记录集
		virtual void Close() = 0;
	};

	//数据库连接基类

	class IConnection
	{
	public:
		//连接数据库
		virtual bool Connect(std::string host, std::string dbname, std::string user,
			std::string password, std::string Encode = "GBK", int port = 3306) = 0;

		//断开连接
		virtual bool DisConnect() = 0;

		//是否连接
		virtual bool IsOpen() = 0;

		//执行SQL语句,无返回结果
		virtual bool Execute(std::string SQL) = 0;

		//执行查询语句，返回记录集;
		virtual IRecordSet* Query(std::string SQL) = 0;

		//开始事务
		virtual void BeginTrans() = 0;

		//执行事务
		virtual void CommitTrans() = 0;

		//回滚操作
		virtual void RollBackTrans() = 0;

		//设置数据库编码格式
		virtual int	SetClientEncoding(const char* encode) = 0;

		//直接向数据库传参；
		virtual IConnectionPtr* ConnectionPtr() = 0;

		//判断数据库中某个表是否存在
		virtual bool IsTableExist(std::string TableName) = 0;
	};
};

