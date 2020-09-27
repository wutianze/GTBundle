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
./bundle server/client

- java socket client demo 
cd access/java/  
javac -d bin/ src/*java  
cd bin  
java Test 127.0.0.1 8000  

# structure
host client\[access client <-> access server <-> bundle client\] <-> host server\[bundle server, bundle client\] <-> host controller\[bundle server\]

# docker usage
docker pull sauronwu/gtbundle:v1.1  
docker run -it -p 5100:5100 sauronwu/gtbundle:v1.1 /bin/bash

# DDS configure
-如果你不是总线  
修改config/profiles.xml，clipub_transport字段的wan_addr地址改为容器的公网ip（host ip），cli_participant字段中需要有一个locator字段中的ip和端口改为消息总线DDS提供的ip和端口，目前是152.136.134.100和5100
-如果你是总线  
修改serpub_transport字段的wan_addr，ser_participant中的locator类似修改，加入所有已知的DDS client的ip和port

# 测试（上一次更新2020.9.27）
1. 第一阶段测试
  - 测试目的：各个部门本地的socket可以接入总线提供的容器，实现双向的收发消息
  - 测试方式：非总线部门使用`./bundle client`来启动容器内的socket server（localhost:8000）; 如果你的实现是socket server而不是socket client，则使用`./bundle test`来临时测试，请尽快更新至socket client.
  - 测试进程：目前3个部门都已经实现了socket联通，还需要进一步增强鲁棒性，另外无线部门需要更改成socket client
2. 第二阶段测试
  - 测试目的：各个部门本地的DDS client可以跨互联网接入总线的DDS server，实现DDS server - DDS client - socket server - socket client全链路联通
  - 测试方式：各个部门本地容器内的DDS client participant配置自己的网络参数（ip和端口），总线端加入所有部门DDS client的网络参数。然后在总线这端运行`./bundle server`，各部门运行`./bundle client`
  - 测试进程：还未开始
3. 第三阶段测试
  - 测试目的：实装消息格式和交互协议
  - 测试进程：还未开始
