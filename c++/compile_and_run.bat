@echo off
cd ..\data
echo 1 | ..\c++\bin\Windows\main.exe false %1.sasm 0
if %errorlevel% GEQ 1 (
   exit /b %errorlevel%
)
cd ..\c++
make install
cd bin\Windows
echo 2 | .\main.exe false %1 1000000
cd ..\..
