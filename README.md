# GTBundle

# init
source script/setup.sh

mkdir build

cd build

cmake ..

make

# run
- bundle  
cd build  
./bundle server/client/controller  
- access java  
cd access/java/  
javac -d bin/ java/*java  
cd bin  
java Test 127.0.0.1 8000  

# structure
host client\[access client <-> access server <-> bundle client\] <-> host server\[bundle server, bundle client\] <-> host controller\[bundle server\]
