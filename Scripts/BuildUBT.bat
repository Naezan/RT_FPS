@echo off
pushd "%~dp0"

rem Building UnrealBuildTools
dotnet msbuild /restore /target:build /property:Configuration=Development ..\UnrealEngine\Engine\Source\Programs\UnrealBuildTool\UnrealBuildTool.csproj

rem Execute BuildTool
..\UnrealEngine\Engine\Binaries\DotNet\UnrealBuildTool.exe -projectfiles -project="..\RT_FPS.uproject" -game -rocket -progress

rem Building Project
dotnet msbuild "..\RT_FPS\RT_FPS.sln" /target:build /property:Configuration="Development Editor" /property:Platform=Win64 /verbosity:diagnostic

popd