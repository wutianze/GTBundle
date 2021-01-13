# GTBundle
## Structure
- 总控需要自己实现一个java client，并且部署一个总线的容器，该java client的数据和本地的总线容器交互；总线容器会负责接入总线，即和高通量、无线等部门处部署的总线容器交互；各部门同样需要自己实现一个java client并部署一个总线的容器，原理和总控的一样。
注：部署的总线容器需要公网ip，java client和总线容器的交互可以不走公网。

## Docker usage
`docker pull sauronwu/gtbundle:v1.2 # 如果是arm版本则docker pull sauronwu/gtbundle_arm:v1.0`  
`docker run -it -p 5100:5100 -p 8000:8000 sauronwu/gtbundle:v1.2 /bin/bash` # 此时会进入容器的bash，请进行下文的init和run操作，具体端口映射请根据自身需求来定

## init（凡是使用docker容器的都需要进行这一步）
`cd home/GTBundle`  
`git pull`  
`vi script/setup_server.sh` #修改script中的setup_\*文件, 如果你是非总控侧，则修改setup_client.sh中的GLOBAL_INDEX变量，总线会给每个部门指定单独的序号；如果你是总控，修改setup_server.sh中的LINKNUM变量，指定本次实验有多少个下属部门参与，比如本次只有无线和高通量参与联调，那么该数值为2（需要你有两个java client各负责一个部门消息的收发）  
`source script/setup_server.sh`# 如果你是非总控则改为setup_client.sh  
`mkdir build # or clear the build dir`  
`cd build`  
`cmake ..`    
`make`

## run
- bundle   
运行bundle即运行总线容器，运行前需要联系总线工作人员配置IP，见下文DDS configure部分  
`cd build`  
`./bundle server/client #总控侧运行server，非总控侧运行client`

- java socket client demo # 运行socket client，各个部门测试时这里的java应该是各自实现的，端口目前都是8000。注：java socket不应该在总线容器里执行，应该在各部门自己的机器或环境中执行，它和总线容器通过socket进行交互  
`cd access/java/`  
`javac -d bin/ -classpath ./src/fastjson-1.2.74.jar ./src/*.java`  
`cd bin # 注意在demo测试时，先启动非总控侧的java程序再启动总控侧的，总控侧的一启动就会发送一条命令要求各个部门上报数据`  
`java -cp ../src/fastjson-1.2.74.jar: Generator 127.0.0.1 8000 1000 1000 # 在非总控侧，运行Generator来产生数据，前两个参数指定ip和端口，第3个参数指定循环次数，第4个参数指定每次循环间隔的ms数`  
`java -cp ../src/fastjson-1.2.74.jar: Controller 127.0.0.1 8000 1000 1000 # 在总控侧，运行Controller来收消息以及发布命令，参数同上`

## DDS configure
- 如果你不是总控（运行的是./bundle client）  
修改config/profiles.xml，clipub_transport字段的wan_addr地址改为容器的公网ip（host ip），cli_participant字段中需要有一个locator字段中的ip和端口改为消息总线DDS提供的ip和端口  
- 如果你是总控(运行的是./bundle server)  
修改serpub_transport字段的wan_addr，ser_participant中的locator类似修改，加入所有已知的DDS client的ip和port

## For 总线开发人员
- generate new message type idl  
  `./script/generateIDL.sh SerCli`
- 运行环境基本需求
  1. cmake >= 3.12: https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4.tar.gz
  2. g++ >= 7.5
  3. fastdds >= 2.0.0
  4. 推荐使用ubuntu >= 18.04
  5. 如果使用docker容器，则只需要对应选择x86或arm即可

## 测试（上一次更新2021.1.12）
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
  - 测试进程：消息测试样例
      - 各部门接入总线
      - 总控下发消息："{"id":"id0","timestamp":"2021xxx","delay":10,"content":"report"}"，含义是开始上报（report），此时id和delay字段无意义
      - 各部门收到消息，开始周期性上报数据，以高通量为例：{"msgType":"cloud_platform_info_total","msgId":"1","msgTag":{"stationName":"盐城机房测试"},"content":{"disk_total":"0TB","ram_total":"1TB","net_band":"100G","cpu_total":"0"}}，此处消息格式各部门不一定一样。
      - 总控下发消息："{"id":"id0","timestamp":"2021xxx","delay":10,"content":"control"}"，含义是开始控制（control），调控ID为id0的应用，使之延迟在10以内（单位自行脑补。。。）
      - 各部门收到各自的调控指令，开始调控
      - 调控终止：当收到消息格式为"{"id":"id0","timestamp":"2021xxx","delay":-1,"content":"control"}"，即delay为负数，则表示对应用id0的延迟要求取消
