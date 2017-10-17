# Main directory

##Building
mkdir build 
cd build
cmake -Darch=<arch> -DTOOL_CHAIN=gcc -DBENCHMARKS="<benchmark1;benchmark2;...;benchmarkn>" ..
make
make download
