cd $PSScriptRoot\..\UnrealEngine

# Build Engine
# . Engine\Build\BatchFiles\RunUAT.bat BuildGraph -target="Make Installed Build Win64" -script=Engine/Build/InstalledEngineBuild.xml -set:WithWin64=true -set:WithMac=false -set:WithAndroid=false -set:WithIOS=false -set:WithTVOS=false -set:WithLinux=false -set:WithLinuxArm64=false -set:WithClient=true -set:WithServer=true
. Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project="$PSScriptRoot\..\RT_FPS.uproject" -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory="$PSScriptRoot\..\Build"