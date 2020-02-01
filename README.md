# UE4-MySQLPlugin
///////////////////////////////////////////////////
Update: Added 4.24 support
a UE4 plugin for MySQL
@author 634395578@qq.com	2016/11/16
if you find any bug , you can send a email to my email: 634395578@qq.com

//////////////////////////////////////////////////

	support database:
		
			*mysql-5.7.16
	
	later will support:	
		
		*postgresql
		

//////////////////////////////////////////////////




to use this plugin you shuld:

1. set up mysql in your pc.
2. add A plugin folder in your project, then copy "DBSupport" under it, restart your project.
3. if your project has vsproject, your build.cs should add "DBSupport" moudle :
		
		PublicDependencyModuleNames.AddRange(new string[] { ....."DBSupport", ..... });

4. add "#include "DBSupport.h" in the file which you want use the plugin.


///////////////////////////////////////////////////////


example code:

		#include "MyP.h"
		#include "MyPGameModeBase.h"
		
		#include "DBSupport.h"
		using namespace DBBase;

		void AMyPGameModeBase::StartPlay()
		{
			Super::StartPlay();

			if (IDBSupportPlugin::IsAvailable())
			{
				//init connection
				DBBase::IConnection * pDBC = IDBSupportPlugin::Get().GetConnection(MYSQL_CONNECTION);

				if (pDBC)
				{
					GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Red, "3");

					//connect to db
					if (!pDBC->IsOpen())
					{
						pDBC->Connect("127.0.0.1", "mydatabase", "root", "123456", "UTF-8", 3306);
					}
					if (pDBC->IsOpen())
					{
						
						//create table
						if (!pDBC->IsTableExist("TEST2"))
						{
							const char * cMysql1 = "CREATE TABLE TEST2(ID INT(8) PRIMARY KEY, NAME VARCHAR(16) );";
							pDBC->Execute(cMysql1);
						}
						
						//excute sql for insert
						const char * cMysql2 = "INSERT INTO TEST2 VALUES (1, 'BBB');";
						pDBC->Execute(cMysql2);

						Sleep(1);
						
						//excute sql for query
						const char * cMysql3 = "SELECT * FROM TEST2;";
						IRecordSet * pRec = pDBC->Query(cMysql3);

						//trvaversal the result record.
						pRec->MoveFirst();
						while (!pRec->IsEOF())
						{
							FString getValue = pRec->GetStringValue(pRec->GetFieldName(1)).c_str();

							getValue += " *****";
							GLog->Log(getValue);
							GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Green, getValue);

							pRec->MoveNext();
						}
						//close the result record.
						pRec->Close();

						//close the db connection.
						pDBC->DisConnect();

					}

				}

			}
		}

