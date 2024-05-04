cd $PSScriptRoot\..\UnrealEngine

# Build Engine
# msbuild "$PSScriptRoot\..\RT_FPS.sln" -t:rebuild -verbosity:diag -property:Configuration="Development Editor"
# ..\UnrealEngine\Engine\Build\BatchFiles\RunUAT.bat BuildGraph -target="Make Installed Build Win64" -script="%~dp0\..\UnrealEngine\Engine\Build\InstalledEngineBuild.xml" -set:CompileDatasmithPlugins=false -set:HostPlatformOnly=true -set:GameConfigurations=Development -set:WithDDC=false
. Engine\Build\BatchFiles\RunUAT.bat BuildGraph -target="Make Installed Build Win64" -script="$PSScriptRoot\..\UnrealEngine\Engine\Build\InstalledEngineBuild.xml" -set:WithWin64=true -set:WithMac=false -set:WithAndroid=false -set:WithIOS=false -set:WithTVOS=false -set:WithLinux=false -set:WithLinuxArm64=false -set:WithClient=true -set:WithServer=true
. Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project="$PSScriptRoot\..\RT_FPS.uproject" -noP4 -platform=Win64 -targetplatform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -build -stage -pak -archive -skipcookingeditorcontent -unversionedcookedcontent -archivedirectory="$PSScriptRoot\..\Build\RT_FPS"