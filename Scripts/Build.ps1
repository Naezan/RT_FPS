cd $PSScriptRoot\..\UnrealEngine

# Build Engine
. Engine\Binaries\DotNet\UnrealBuildTool.exe -projectfiles -project="$PSScriptRoot\..\RT_FPS.uproject" -game -rocket -progress

$batCommand = ".\dotnet msbuild ""$PSScriptRoot\..\RT_FPS.sln"" /target:build /property:Configuration=""Development Editor"" /property:Platform=Win64 /verbosity:diagnostic"
Invoke-Expression -Command $batCommand
# dotnet msbuild "$PSScriptRoot\..\RT_FPS.sln" /target:build /property:Configuration="Development Editor" /property:Platform=Win64 /verbosity:diagnostic

. Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project="$PSScriptRoot\..\RT_FPS.uproject" -noP4 -platform=Win64 -targetplatform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -build -stage -pak -archive -skipcookingeditorcontent -unversionedcookedcontent -archivedirectory="$PSScriptRoot\..\Build\RT_FPS"