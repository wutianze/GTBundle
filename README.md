# GTBundle

# init
source script/setup.sh

mkdir build # or clear the build dir

cd build

cmake ..

make

# run
- bundle  
cd build  
./bundle server/client # 非总线部门使用`./bundle client`来启动或测试; 如果你的实现是socket server而不是socket client，则使用`./bundle test`来临时测试，请尽快更新至socket client.

- java socket client demo 
cd access/java/  
javac -d bin/ src/*java  
cd bin  
java Test 127.0.0.1 8000  

# structure
host client\[access client <-> access server <-> bundle client\] <-> host server\[bundle server, bundle client\] <-> host controller\[bundle server\]

# docker usage
docker pull sauronwu/gtbundle:v1.1  
docker run -it -p 5100:5100 gtbundle:v1.1 /bin/bash

# DDS configure
-如果你不是总线
修改config/profiles.xml，clipub_transport字段的wan_addr地址改为容器的公网ip，cli_participant字段中第一个locator同样改为容器的公网ip第二个locator字段中的ip和端口改为消息总线DDS提供的ip和端口，目前是152.136.134.100和5100
