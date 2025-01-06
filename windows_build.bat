@echo off

rmdir /S /Q  build-dir
mkdir build-dir
cmake . -A x64 -B build-dir
cmake --build ../build-dir --config Release
REM cmake --install ../build-dir/ --config Release --prefix install
pause
