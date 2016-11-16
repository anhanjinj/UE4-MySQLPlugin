// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DBBase.h"
#include <ModuleManager.h>

/**
* The public interface to this module
*/
class IDBSupportPlugin : public IModuleInterface
{
public:

	//获得连接对象指针;
	virtual DBBase::IConnection * GetConnection(CONNECTION_TYPE type) = 0;

	//释放连接对象指针
	virtual  bool ReleaseConnection(DBBase::IConnection * pConn) = 0;

public:
	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline IDBSupportPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< IDBSupportPlugin >("DBSupport");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("DBSupport");
	}
};