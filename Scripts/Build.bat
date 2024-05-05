@echo off

rem Set VCTargetsPath
SET VCTargetsPath=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Microsoft\VC\v170

rem Building UnrealBuildTools
dotnet msbuild "D:\a\RT_FPS\RT_FPS\UnrealEngine\Engine\Source\Programs\UnrealBuildTool\UnrealBuildTool.sln" /t:Restore /target:build /property:Configuration="Development"

rem Execute BuildTool
D:\a\RT_FPS\RT_FPS\UnrealEngine\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe -projectfiles -project="D:\a\RT_FPS\RT_FPS\RT_FPS.uproject" -game -rocket -progress

rem Building Project
dotnet msbuild "D:\a\RT_FPS\RT_FPS\RT_FPS.sln" /t:Restore /target:build /property:Configuration="Development Editor" /property:Platform=Win64 /verbosity:diagnostic