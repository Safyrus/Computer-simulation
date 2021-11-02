make install
cd bin\Windows
echo 2 | .\main.exe false prog/verifCPU/verifCPU 1000000 false %1
cd ..\..