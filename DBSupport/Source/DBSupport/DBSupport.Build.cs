// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using Path = System.IO.Path;

public class DBSupport : ModuleRules
{
	public DBSupport(TargetInfo Target)
    {
        PublicIncludePaths.AddRange(
			new string[] {
				"DBSupport/Public",
                "ThirdPart/Public",
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"DBSupport/Private",
                "ThirdPart/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "Projects"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        
        /****************************************/

        // If you update this path, ensure the DLL runtime delay load path in FOptitrackNatnetModule::StartupModule stays in sync.
        string DBPath = Path.GetFullPath( Path.Combine( ModuleDirectory, "..", "..", "ThirdParty", "MySql" ) );
        string DBPath86 = System.IO.Path.Combine(DBPath, "x86");
        string DBPath64 = System.IO.Path.Combine(DBPath, "x64");

        PublicSystemIncludePaths.Add( System.IO.Path.Combine(DBPath86, "include" ) );
        PublicSystemIncludePaths.Add(System.IO.Path.Combine(DBPath64, "include"));

        if ( Target.Platform == UnrealTargetPlatform.Win32 )
        {
            string DBLibBinPath = Path.Combine( DBPath86, "lib");
            PublicLibraryPaths.Add( DBLibBinPath );
            PublicAdditionalLibraries.Add( "libmysql.lib" );
            PublicDelayLoadDLLs.Add( "libmysql.dll" );
            //RuntimeDependencies.Add( new RuntimeDependency( Path.Combine( DBLibBinPath, "libmysql.dll" ) ) );
        }
        else if (Target.Platform == UnrealTargetPlatform.Win64 )
        {
            string DBLibBinPath = Path.Combine(DBPath64, "lib");
            PublicLibraryPaths.Add(DBLibBinPath);
            PublicAdditionalLibraries.Add("libmysql.lib");
            PublicDelayLoadDLLs.Add("libmysql.dll");
            //RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(DBLibBinPath, "libmysql.dll")));
        }
        else if ( Target.Platform == UnrealTargetPlatform.Android )
        {

        }
	}
}
