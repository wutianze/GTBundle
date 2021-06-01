# GTBundle
## Structure
- 总控需要自己实现一个java client，并且部署一个总线的容器，该java client的数据和本地的总线容器交互；总线容器会负责接入总线，即和边缘侧部署的总线容器交互；边缘侧设备同样需要自己实现一个java client并部署一个总线的容器，原理和总控的一样。
注：部署的总线容器需要公网ip，java client和总线容器的交互可以不走公网。

## Docker usage
`docker pull sauronwu/gtbundle:v2.1 # 如果是arm版本则docker pull sauronwu/gtbundle_arm:v1.0`  
`docker run -it -p 5100:5100 -p 8000:8000 sauronwu/gtbundle:v2.1 /bin/bash` # 此时会进入容器的bash，请进行下文的init和run操作，具体端口映射请根据自身需求来定，但请保持容器和host的端口一致。  
最新的x86容器版本为v2.1，采用了更新的fastdds:v2.1.0版本，推荐使用（已测试通过）。如果出现问题，请使用v1.3。

## init（凡是使用docker容器的都需要进行这一步）
`cd home/GTBundle`  
`git pull`  
`vi script/setup_server.sh` #修改script中的setup_\*文件, 如果你是非总控侧，则修改setup_client.sh中的GLOBAL_INDEX变量，总线会给每个部门指定单独的序号（目前暂定最终联调时骨干为0，高通量为1，接入为2）；如果你是总控，修改setup_server.sh中的LINKNUM变量，指定本次实验有多少个下属部门参与，比如本次只有无线和高通量参与联调，那么该数值为2（需要你有两个java client各负责一个部门消息的收发，另外两部门修改GLOBAL_INDEX为0和1）  
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
`java -cp ../src/fastjson-1.2.74.jar: Controller 127.0.0.1 8000 1000 1000 # 在总控侧，运行Controller来收消息以及发布命令，参数同上，如果此时有多个socket连接，则将上面命令中8000依次改为8000,8001,8002...`

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
