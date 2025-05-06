#!/bin/bash

echo sudo apt install unixodbc unixodbc-dev
echo wget https://dev.mysql.com/get/Downloads/Connector-ODBC/9.2/mysql-connector-odbc_9.2.0-1ubuntu22.04_amd64.deb
echo sudo dpkg -i mysql-connector-odbc_9.2.0-1ubuntu22.04_amd64.deb
echo cat /etc/odbcinst.ini

echo sudo yum install unixODBC-devel

mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=relwithdebinfo ..
cmake --build . --config relwithdebinfo

cd ..

# popd

# echo continue && read -n 1
