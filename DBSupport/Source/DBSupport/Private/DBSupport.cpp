// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "DBSupportPrivatePCH.h"

#include "DBSupport.h"
#include <Core.h>
#include <ModuleManager.h>
#include <IPluginManager.h>


#include "DBMySQL.h"
#include "DBPostgreSQL.h"




#define LOCTEXT_NAMESPACE "FDBSupportModule"
using namespace DBBase;


class DBSupportModule : public IDBSupportPlugin
{
public:
	// IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	//获得连接对象指针;
	virtual IConnection * GetConnection(CONNECTION_TYPE type) override;
	
	//释放连接对象指针
	virtual bool ReleaseConnection(IConnection * pConn) override;

	void* DBLibHandle;
};



// This code will execute after your module is loaded into memory; the exact timing depends on the per-module LoadingPhase specified in the .uplugin file.
void DBSupportModule::StartupModule()
{

	// Note: These paths correspond to the RuntimeDependency specified in the .Build.cs script.
	const FString PluginBaseDir = IPluginManager::Get().FindPlugin("DBSupport")->GetBaseDir();
		

#ifdef DBSUPPORT_MYSQL

#if PLATFORM_WINDOWS && PLATFORM_64BITS
	const FString LibraryPath = FPaths::Combine(*PluginBaseDir, TEXT("ThirdParty/MySQL/x64/lib/libmysql.dll"));
#elif PLATFORM_WINDOWS && PLATFORM_32BITS
	const FString LibraryPath = FPaths::Combine(*PluginBaseDir, TEXT("ThirdParty/MySQL/x86/lib/libmysql.dll"));
#else
#	error Path to NatNet shared library not specified for this platform!
#endif

#endif // DBSUPPORT_MYSQL



	DBLibHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (DBLibHandle == nullptr)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("DBLibDLLLoadError", "Error: Failed to load mysql.dll"));
	}
}


// This function may be called during shutdown to clean up your module.
// For modules that support dynamic reloading, we call this function before unloading the module.
void DBSupportModule::ShutdownModule()
{

	// Unload the DLL.
	if (DBLibHandle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(DBLibHandle);
		DBLibHandle = nullptr;
	}


}

//获得连接对象指针
IConnection * DBSupportModule::GetConnection(CONNECTION_TYPE type)			
{

	switch (type)
	{
	case PG_CONNECTION:
	{
#ifdef DBSUPPORT_PGSQL
	    return	new PGConnection();;
#else
		return nullptr;
#endif
		
		break;
	}
	case MYSQL_CONNECTION:
	{
#ifdef DBSUPPORT_MYSQL
		return new MYSQLConnection();
#else
		return nullptr;
#endif

		break;
	}
	default:
	{
		
	}
	}
	return nullptr;
}
//断开连接，释放连接对象
bool DBSupportModule::ReleaseConnection(IConnection * pConn)
{
		if (pConn)
		{
			if (pConn->IsOpen())
			{
				pConn->DisConnect();
				delete pConn;
				pConn = NULL;
			}
		}

	return true;
}



#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(DBSupportModule, DBSupport)