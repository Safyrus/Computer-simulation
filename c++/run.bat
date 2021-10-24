make install
cd bin\Windows
echo 2 | .\main.exe false null 1000000 false %1
cd ..\..