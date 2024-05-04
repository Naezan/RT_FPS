@echo off
pushd "%~dp0"

rem Building UnrealBuildTools
dotnet msbuild /restore /target:build /property:Configuration=Development ..\UnrealEngine\Engine\Source\Programs\UnrealBuildTool\UnrealBuildTool.csproj

popd