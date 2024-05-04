@echo off
pushd "%~dp0"

rem Building UnrealBuildTools
dotnet msbuild /restore /target:build /property:Configuration=Development ..\UnrealEngine\Engine\Source\Programs\UnrealBuildTool\UnrealBuildTool.csproj
..\UnrealEngine\Engine\Build\BatchFiles\RunUAT.bat BuildGraph -target="Make Installed Build Win64" -script="%~dp0\..\UnrealEngine\Engine\Build\InstalledEngineBuild.xml" -set:CompileDatasmithPlugins=false -set:HostPlatformOnly=true -set:GameConfigurations=Development -set:WithDDC=false
popd