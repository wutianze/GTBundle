# GTBundle

# 运行环境基本需求
1. cmake >= 3.12: https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4.tar.gz
2. g++ >= 7.5
3. fastdds >= 2.0.0
4. 推荐使用ubuntu >= 18.04

# init（凡是使用docker容器的都需要进行这一步）
`source script/setup.sh`  
`mkdir build # or clear the build dir`  
`cd build`  
`cmake ..`    
`make`

# run
- bundle #运行bundle  
`cd build`  
`./bundle server/client #总控侧运行server，非总控侧运行client`

- java socket client demo # 运行socket client，各个部门测试时这里的java应该是各自实现的，端口目前都是8000  
`cd access/java/`  
`javac -d bin/ -classpath ./src/fastjson-1.2.74.jar ./src/*.java`  
`cd bin # 注意在demo测试时，先启动非总控侧的java程序再启动总控侧的，总控侧的一启动就会发送一条命令要求各个部门上报数据`  
`java Generator 127.0.0.1 8000 1000 1000 # 在非总控侧，运行Generator来产生数据，前两个参数指定ip和端口，第3个参数指定循环次数，第4个参数指定每次循环间隔的ms数`
`java Generator 127.0.0.1 8000 1000 1000 # 在总控侧，运行Controller来收消息以及发布命令，参数同上`

# structure
host client\[access client <-> access server <-> bundle client\] <-> host server\[bundle server, bundle client\] <-> host controller\[bundle server\]

# generate new message type idl
`./script/generateIDL.sh SerCli`

# docker usage
`docker pull sauronwu/gtbundle:v1.2 # 如果是arm版本则docker pull sauronwu/gtbundle_arm:v1.0`  
`docker run -it -p 5100:5100 sauronwu/gtbundle:v1.2 /bin/bash`

# DDS configure
-如果你不是总线  
修改config/profiles.xml，clipub_transport字段的wan_addr地址改为容器的公网ip（host ip），cli_participant字段中需要有一个locator字段中的ip和端口改为消息总线DDS提供的ip和端口，目前是152.136.134.100和5100
-如果你是总线  
修改serpub_transport字段的wan_addr，ser_participant中的locator类似修改，加入所有已知的DDS client的ip和port

# 测试（上一次更新2020.11.9）
1. 第一阶段测试
  - 测试目的：各个部门本地的socket可以接入总线提供的容器，实现双向的收发消息
  - 测试方式：非总线部门使用`./bundle client`来启动容器内的socket server（localhost:8000）; 如果你的实现是socket server而不是socket client，则使用`./bundle test`来临时测试，请尽快更新至socket client；如果你是控制中心，则原则上就是总线，所以使用`./bundle server`来启动容器内的
  - 测试进程：目前3个部门都已经实现了socket联通，还需要进一步增强鲁棒性，另外无线部门需要更改成socket client
2. 第二阶段测试
  - 测试目的：各个部门本地的DDS client可以跨互联网接入总线的DDS server，实现DDS server - DDS client - socket server - socket client全链路联通
  - 测试方式：各个部门本地容器内的DDS client participant配置自己的网络参数（ip和端口），总线端加入所有部门DDS client的网络参数。然后在总线这端运行`./bundle server`，各部门运行`./bundle client`
  - 测试进程：总线在自己搭建的节点上完成了测试，和网络部门完成了广域网联通测试。其他部门目前还在申请公网ip
3. 临时需求
  - 需求内容：后续总线部署平台为arm服务器，需要将目前基于x86的项目整体移植。
  - 需求完成情况：制作了arm平台的docker容器，实现了移植。
  - 吐槽：这种需求应该在整个项目最开始时就提出，在接近完成时直接换底层平台，风险巨大，并且白白浪费了好几天的环境配置时间。
4. 第三阶段测试
  - 测试目的：实装消息格式和交互协议
  - 测试进程：还未开始
