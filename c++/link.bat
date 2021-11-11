@echo off
cd ..\data
echo 3 | ..\c++\bin\Windows\main.exe false %1 %2 %3
cd ..\c++
