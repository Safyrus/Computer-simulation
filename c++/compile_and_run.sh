cd ../data
echo 1 | ../c++/bin/Linux/main false $1.sasm 0
if [ $? -eq 1 ]; 
then 
    exit 1
fi
cd ../c++
make install
cd bin/Linux
echo 2 | ./main false $1 1000000
