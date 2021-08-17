# UDP-Football-Game

一个使用 C 语言编写的基于 ncurses 库实现的支持多人联机的终端足球游戏。

## 游戏演示

客户端效果：

![client](https://i.loli.net/2021/08/17/HRhOLKpu7EXtICG.png)

服务端效果：

![server](https://i.loli.net/2021/08/17/Hov8OQR1ILJZX6b.png)

## 编译

1. 安装 ncurses 库：`sudo apt-get install libncurses5-dev`
2. 安装 cJSON 库：[DaveGamble/cJSON](https://github.com/DaveGamble/cJSON#building)
3. 编译服务端：进入 `server` 文件夹，执行编译脚本 `./G`
4. 编译客户端：进入 `client` 文件夹，执行编译脚本 `./G`

## 运行

### 服务端

1. 修改 `server.conf` 服务端配置文件
   ```
   LINES=25      # 游戏界面高度
   COLS=115      # 游戏界面宽度
   PORT=8888     # 监听端口
   DATAPORT=9999 # 数据端口
   ```
2. 运行 `./server`，服务端初始化完成后将监听客户端的上线

### 客户端

1. 修改 `football.conf` 客户端配置文件
   ```
   SERVERIP=127.0.0.1  # 服务端IP地址
   SERVERPORT=8888     # 服务端连接端口
   NAME=sakata         # 登录用户名
   LOGMSG=I am sakata  # 上线显示的登录信息
   LINES=25            # 游戏界面高度
   COLS=115            # 游戏界面宽度
   TEAM=1              # 队伍（ 0 或 1 ）
   ```
2. 运行 `./client`，成功连接到客户端后即可开始游戏。支持可选参数：`./client -h 服务器IP -p 端口 -n 用户名 -m 上线信息 -t 队伍编号`
3. 使用 `W/A/S/D` 操作人物在球场上移动，按 `J` 键可进行停球动作，按 `K` 键可进行踢球动作，按 `L` 键可进行带球动作。
4. 按 `Enter` 键可呼出消息窗口，输入要发送的消息，再次按下回车键即可发送消息。
5. 带球时按下空格键，可呼出力度控制条，再次按下空格键可使用当前选择的力度将球踢出。

## 技术及功能亮点
 - 线程池技术
 - 主从反应堆模式
 - I/O多路复用（epoll）
 - 支持心跳检测，可实时探测用户的在线状态
 - 服务端和客户端可实时监看当前球场及球员状态

## 使用的第三方库
 - ncurses
 - cJSON
