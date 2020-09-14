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
javac -d bin/ src/*java  
cd bin  
java Test 127.0.0.1 8000  

# structure
host client\[access client <-> access server <-> bundle client\] <-> host server\[bundle server, bundle client\] <-> host controller\[bundle server\]

# docker usage
docker pull sauronwu/gtbundle:v1.1  
docker run -it -p 5100:5100 gtbundle:v1.1 /bin/bash
