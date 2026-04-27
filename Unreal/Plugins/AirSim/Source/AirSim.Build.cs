// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using UnrealBuildTool;
using System.IO;

public class AirSim : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string AirLibPath
    {
        get { return Path.Combine(ModulePath, "AirLib"); }
    }
    private string AirSimPluginPath
    {
        get { return Directory.GetParent(ModulePath).FullName; }
    }
    private string ProjectBinariesPath
    {
        get
        {
            return Path.Combine(
                Directory.GetParent(AirSimPluginPath).Parent.FullName, "Binaries");
        }
    }
    private string AirSimPluginDependencyPath
    {
        get { return Path.Combine(AirSimPluginPath, "Dependencies"); }
    }

    public AirSim(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnableExceptions = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ImageWrapper", "RenderCore", "RHI", "PhysicsCore", "AssetRegistry", "ChaosVehicles", "Landscape", "CinematicCamera" });
        PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore", "RenderCore" });

        PublicDefinitions.Add("MSGPACK_DISABLE_LEGACY_NIL=1");
        PublicDefinitions.Add("ZENOHCXX_ZENOHC=1");
        PublicDefinitions.Add("_SCL_SECURE_NO_WARNINGS=1");
        PublicDefinitions.Add("_CRT_SECURE_NO_WARNINGS=1");
        PublicDefinitions.Add("HMD_MODULE_INCLUDED=0");

        PublicIncludePaths.Add(Path.Combine(AirLibPath, "include"));
        string eigenPath = RunMiseWhere("conan:eigen");
        PublicIncludePaths.Add(Path.Combine(eigenPath, "include", "eigen3"));
        PublicIncludePaths.Add(Path.Combine(AirLibPath, "deps", "msgpack", "include"));
        string nlohmannPath = RunMiseWhere("conan:nlohmann_json");
        PublicIncludePaths.Add(Path.Combine(nlohmannPath, "include"));

        LoadAirSimDependency(Target, "MavLinkCom", "MavLinkCom");
        AddLibDependency("AirLib", Path.Combine(AirLibPath, "lib"), "AirLib", Target, false);
        AddZenohDependency(Target);
        AddOSLibDependencies(Target);
    }

    private void AddOSLibDependencies(ReadOnlyTargetRules Target)
    {
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // for SHGetFolderPath.
            PublicAdditionalLibraries.Add("Shell32.lib");

            //for joystick support
            PublicAdditionalLibraries.Add("dinput8.lib");
            PublicAdditionalLibraries.Add("dxguid.lib");
        }

        if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            // needed when packaging
            PublicAdditionalLibraries.Add("stdc++");
            PublicAdditionalLibraries.Add("supc++");
        }
    }

    private string RunMiseWhere(string tool)
    {
        var proc = new System.Diagnostics.Process();
        proc.StartInfo.FileName = "mise";
        proc.StartInfo.Arguments = "where " + tool;
        proc.StartInfo.RedirectStandardOutput = true;
        proc.StartInfo.UseShellExecute = false;
        proc.Start();
        string output = proc.StandardOutput.ReadToEnd().Trim();
        proc.WaitForExit();
        return output;
    }

    private bool AddZenohDependency(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if (Target.Platform == UnrealTargetPlatform.Mac || Target.Platform == UnrealTargetPlatform.Linux)
        {
            isLibrarySupported = true;
            string zenohC = RunMiseWhere("github:eclipse-zenoh/zenoh-c");
            string zenohCpp = RunMiseWhere("github:eclipse-zenoh/zenoh-cpp");
            PublicAdditionalLibraries.Add(Path.Combine(zenohC, "lib", "libzenohc.a"));
            PublicIncludePaths.Add(Path.Combine(zenohC, "include"));
            PublicIncludePaths.Add(Path.Combine(zenohCpp, "include"));
        }

        PublicDefinitions.Add(string.Format("WITH_ZENOH_BINDING={0}", isLibrarySupported ? 1 : 0));
        return isLibrarySupported;
    }

    static void CopyFileIfNewer(string srcFilePath, string destFolder)
    {
        FileInfo srcFile = new FileInfo(srcFilePath);
        FileInfo destFile = new FileInfo(Path.Combine(destFolder, srcFile.Name));
        if (!destFile.Exists || srcFile.LastWriteTime > destFile.LastWriteTime)
        {
            srcFile.CopyTo(destFile.FullName, true);
        }
        //else skip
    }

    private bool LoadAirSimDependency(ReadOnlyTargetRules Target, string LibName, string LibFileName)
    {
        string LibrariesPath = Path.Combine(AirLibPath, "deps", LibName, "lib");
        return AddLibDependency(LibName, LibrariesPath, LibFileName, Target, true);
    }

    private bool AddLibDependency(string LibName, string LibPath, string LibFileName, ReadOnlyTargetRules Target, bool IsAddLibInclude)
    {
        string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Mac) ? "x64" : "x86";
        string ConfigurationString = (Target.Configuration == UnrealTargetConfiguration.Debug) ? "Debug" : "Release";
        bool isLibrarySupported = false;


        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            isLibrarySupported = true;

            PublicAdditionalLibraries.Add(Path.Combine(LibPath, PlatformString, ConfigurationString, LibFileName + ".lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux || Target.Platform == UnrealTargetPlatform.Mac)
        {
            isLibrarySupported = true;
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "lib" + LibFileName + ".a"));
        }

        if (isLibrarySupported && IsAddLibInclude)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(AirLibPath, "deps", LibName, "include"));
        }
        PublicDefinitions.Add(string.Format("WITH_" + LibName.ToUpper() + "_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}
