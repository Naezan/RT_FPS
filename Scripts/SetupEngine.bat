@echo off

rem Fetch dependencies
D:\a\RT_FPS\RT_FPS\UnrealEngine\Engine\Binaries\DotNET\GitDependencies\win-x64\GitDependencies.exe --no-cache --exclude=Samples --exclude=Templates --exclude=FeaturePacks --exclude=Engine/Documentation --exclude=Engine/Saved --exclude=Win32 --exclude=win32 --exclude=Mac --exclude=osx64 --exclude=osx32 --exclude=Linux --exclude=linux --exclude=Linux32 --exclude=linux32 --exclude=Linux64 --exclude=linux64 --exclude=linux_x64 --exclude=Android --exclude=android --exclude=HoloLens --exclude=Win32 --exclude=WinRT --exclude=winrt --exclude=HTML5 --exclude=html5 --exclude=XboxOne --exclude=Switch --exclude=Dingo --exclude=PS4 --exclude=Python

rem Generate vcxproj files
D:\a\RT_FPS\RT_FPS\UnrealEngine\GenerateProjectFiles.bat 