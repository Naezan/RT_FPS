cd $PSScriptRoot\..\..\UnrealEngine

. Engine\Build\BatchFiles\Build.bat RT_FPSEditor Win64 Development -Project="$PSScriptRoot\..\..\RT_FPS.uproject" -waitmutex -NoHotReload