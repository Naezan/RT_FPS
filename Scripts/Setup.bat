@echo off
pushd "%~dp0"

rem Initializing submodules
git submodule update --init --recursive

rem Download dependencies.
..\UnrealEngine\Engine\DotNET\GitDependencies.exe --no-cache --exclude=Samples --exclude=Templates --exclude=FeaturePacks --exclude=Engine/Documentation --exclude=Engine/Saved --exclude=Win32 --exclude=win32 --exclude=Mac --exclude=osx64 --exclude=osx32 --exclude=Linux --exclude=linux --exclude=Linux32 --exclude=linux32 --exclude=Linux64 --exclude=linux64 --exclude=linux_x64 --exclude=Android --exclude=android --exclude=HoloLens --exclude=Win32 --exclude=WinRT --exclude=winrt --exclude=HTML5 --exclude=html5 --exclude=XboxOne --exclude=Switch --exclude=Dingo --exclude=PS4 --exclude=Python
if %errorlevel% neq 0 exit /b %errorlevel%

rem Building UnrealBuildTools
dotnet msbuild /restore /target:build /property:Configuration=Development ..\UnrealEngine\Engine\Source\Programs\UnrealBuildTool\UnrealBuildTool.csproj

popd