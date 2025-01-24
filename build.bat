
echo https://dev.mysql.com/get/Downloads/Connector-ODBC/9.2/mysql-connector-odbc-9.2.0-winx64.msi
rmdir /S /Q build
mkdir build
pushd build
cmake -A x64 -DCMAKE_BUILD_TYPE=relwithdebinfo ..
cmake --build . --config relwithdebinfo
popd

rem pause