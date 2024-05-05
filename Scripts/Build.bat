@echo off

rem Building UnrealBuildTools
dotnet msbuild /restore /target:build /property:Configuration=Development ..\UnrealEngine\Engine\Source\Programs\UnrealBuildTool\UnrealBuildTool.csproj

rem Execute BuildTool
D:\a\RT_FPS\RT_FPS\UnrealEngine\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe -projectfiles -project="D:\a\RT_FPS\RT_FPS\RT_FPS.uproject" -game -rocket -progress

rem Building Project
dotnet msbuild "D:\a\RT_FPS\RT_FPS\RT_FPS.sln" /target:build /property:Configuration="Development Editor" /property:Platform=Win64 /verbosity:diagnostic