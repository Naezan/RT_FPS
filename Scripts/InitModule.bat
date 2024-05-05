@echo off
pushd "%~dp0"

rem Initializing submodules
git submodule update --init --recursive

popd