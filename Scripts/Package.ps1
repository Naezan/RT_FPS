cd $PSScriptRoot\..\UnrealEngine

# Build Engine
. Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project="$PSScriptRoot\..\RT_FPS.uproject" -noP4 -utf8output -platform=Win64 -clientconfig=Development -serverconfig=Development -nocompileeditor -skipbuildeditor -cook -maps=AllMaps -skipstage
# . Engine\Build\BatchFiles\RunUAT.bat BuildCookRun -project="$PSScriptRoot\..\RT_FPS.uproject" -noP4 -platform=Win64 -targetplatform=Win64 -clientconfig=Development -serverconfig=Development -cook -allmaps -build -stage -pak -archive -skipcookingeditorcontent -unversionedcookedcontent -archivedirectory="$PSScriptRoot\..\Build\RT_FPS"