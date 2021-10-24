cd ../data
../c++/bin/Linux/main false $1.sasm 0 <<< 1
if [ $? -eq 1 ]; 
then 
    exit 1
fi
cd ../c++
make install
cd bin/Linux
./main false $1 1000000 <<< 2
