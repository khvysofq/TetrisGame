#TetrisGame



## 编译配置

当前的项目会依赖四个开源库，都已经以`git submodule`的形式链接到项目中，因此你可以直接下载下来进行编译，不需要再去配置额外的库或者环境。

环境要求

- `CMake2.8 `,工程使用CMake来管理源代码，因此你必须要确保在您的计算机上面有`CMake 2.8`以上的版本。
- Windows平台确保已经安装VS2013版本，因为代码中使用了C++ 11的特性，当前只在VS2013上编译通过。
- Linux平台确保GCC4.7以上的版本，支持C++ 11特性


当前的项目已经在以下几个平台上编译通过

- Windows Microsoft Visual Studio 2013 
- Ubuntu 64 GCC 4.8

### 下载工程

```bash
	// 下载源代码工程
	git clone https://git.oschina.net/guangleihe/TetrisGame.git
	// 进入下载工程目录	
	cd TetrisGame
	// 更新第三方依赖库
	git submodule update --init --recursive
	// 创建一个编译文件夹，在这个文件夹中进行编译，保证源代码清洁
	mkdir build
	// 各个平台的编译方式参考下面的教程
	cd build
	// 执行CMake生成相应的工程文件
```

我们将下载源代码的路径称为`git_path`路径。通过上面的几个步骤您现在应该处于`git_path/TetrisGame/build`下。

请确保您已经安装了CMake2.8以上的版本，并且已经加入到bash的path路径中，然后执行CMake生成工程

```bash
cmake .. -DBUILD_CURL_TESTS=OFF 
		-DBUILD_CURL_EXE=OFF 
		-DJSONCPP_WITH_TESTS=OFF 
		-DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF 
		-DWITH_GFLAGS=OFF 
		-DHTTP_ONLY=ON
```

这一条命令会在Windows平台上面生成VS工程，在Linux平台上面生成Makefile文件。

### Windows平台上编译并使用

在Windows平台上生成的是VS的工程，可以直接打开进行编译。特别注意我们的编译是有顺序的，一般情况下先编译库，再编译相关的测试程序。虽然我们已经在CMake中指定了编译顺序，但是由于VS一般都采用多线程编译技术，因此一些代码量比较少的测试工程有可能会先编译起，在这种情况下，一些依赖库还没有编译好，所以会出现找不到库的问题。再使用VS编译一次就行了。

编译好了之后，生成的所有静态库，包括第三方静态库都会被放置到`git_path/TetrisGame/lib/win`下面。正常情况下，你在这个目录下应该会看到以下几个库

- `glog.lib`，由Google Glog生成的日志库
- `jsoncpp.lib`，由JsonCpp生成的解析Json库
- `libtetris.lib`，当前所编译的tetris库
- `libcurl_imp.lib`，由`libcurl`生成的库

其中`libcurl`生成的是动态库，因此在`git_path/TetrisGame/lib/win`下面，只有一个非常小的静态链接文件，真正的动态库应该在目录`git_path/TetrisGame/bin/win`中，所有的测试工程也都在这里面。用户使用的时候可以将这个路径加入到库搜索路径中，同时链接这些库。

**Windows用户使用注意**

1. 默认情况下，在Windows平台上都是使用的MT/MtD的形式编译的，如果想要切换请在VS工程上修改一下就可以了。

2. 用户在使用的时候要要记得将`git_path/TetrisGame/bin/win/libcurl.dll`库复制到自己的程序目录下才能够运行起得程序。

由于使用了三个开源库，而这些库又有自己的头文件，因此在包含头文件的时候要指定如下几个路径在自己工程中才能够正确的使用
- `git_path/TetrisGame/src`，用户需要包含主要的头文件使用时`#include "ali_search/ali_search.h"`
- `git_path/TetrisGame/third_part/jsoncpp/include`,这是Json的库，用户能够直接使用到。
- 在编译libcurl.dll的时候，如果出现`unresolved external symbol __imp__CertFreeCertificateContext`错误，那么需要在libcurl的vs2013工程中添加对`crypt32.lib`库的链接。

```
schannel.obj : error LNK2019: 
	unresolved external symbol __imp__CertFreeCertificateContext@4 
		referenced in function _schannel_connect_step3
```

> 如果用户希望使用Glog日志库，那么可以包含`#include "base/logging.h"`头文件，但是需要指定Glog的路径`git_path/TetrisGame/build/third_part/glog`和`git_path/TetrisGame/third_partglog/src`两个路径。为了更好的使用Glog，用户可以在自己程序的main函数开始的时候初始化最好初始化Glog
> 
```c++
	int main(int argc, char* argv[]){
	  google::InitGoogleLogging(argv[0]);
	  FLAGS_logtostderr = true;
	  FLAGS_stderrthreshold = 0;
	  FLAGS_colorlogtostderr = true;
	}
```

### Linux平台上编译安装

Linux平台我们已经指定了编译顺序，只需要使用`make`命令执行，就可以把整个工程全部编译起。正常情况下会生成以下几个文件

- `git_path/TetrisGame/lib/unix`下是所有的库文件，主要包含
	- `libtetris.a`，主要的`libtetris`工程库
	- `libcurl.so`，由`libcurl`生成的动态库
	- `libglog.a`，由google log生成的静态库
	- `libjsoncpp.a`，由jsoncpp生成的静态库
- `git_path/TetrisGame/bin/unix`，是所有的测试工程，只包含关于`TetrisGame`的测试工程。

**Linux用户使用注意**

由于使用了三个开源库，而这些库又有自己的头文件，因此在包含头文件的时候要指定如下几个路径在自己Makefile中才能够正确的使用
- `git_path/TetrisGame/src`，用户需要包含主要的头文件使用时`#include "ali_search/ali_search.h"`
- `git_path/TetrisGame/third_part/jsoncpp/include`,这是Json的库，用户能够直接使用到。

> 如果用户希望使用Glog日志库，那么可以包含`#include "base/logging.h"`头文件，但是需要指定Glog的路径`git_path/TetrisGame/build/third_part/glog`和`git_path/TetrisGame/third_partglog/src`两个路径。为了更好的使用Glog，用户可以在自己程序的main函数开始的时候初始化最好初始化Glog
> 
```c++
	int main(int argc, char* argv[]){
	  google::InitGoogleLogging(argv[0]);
	  FLAGS_logtostderr = true;
	  FLAGS_stderrthreshold = 0;
	  FLAGS_colorlogtostderr = true;
	}
```

## TetrisGame Http 协议文档

`TetrisGame`提供基于http简单的`RESTful API`接口，保证用户不论使用什么样的语言都能够轻松的接入服务端。简单来说，我们提供了一共九个接口。这九个接口都使用简单的http get + url的请求形式发送给服务端，而服务端回复JSON数据。

接下来我们先讲服务端的JSON回复，然后再讲请求，因为九个接口的服务端返回字段都是一样的，因此将返回写到最前面。

## 服务端回复

服务端的JSON回复一共会存在七个平行的字段，分别是如下的字段。

| 字段名称 | 字段解释  | 类型  | 备注 |
|---|---|---|---|
| `game_state`  | 游戏当前的状态  |  字符串 |  `running`、`pause`、`game_over`  |
| `view_data` | 游戏当前的界面值，游戏为一个`12 * 20`的长方形界面  | 字符串  | 一共`240`个字符，其中空格(ASCII 32)代表无砖块`#`(ASCII 35)代表有砖块   |
| `next_tetrismino`  | 下一块方块的编号 | 整数  | `[0 , 18]`，一共有19种可能   |
| `score`  | 当前游戏的得分  | 整数 |  -  |
| `token_id`  | 当前游戏的唯一ID号  | 字符串  | 每一个游戏在服务端都有一个唯一的编号，用户的操作需要这个值   |
| `play_name`  | 当前游戏玩家的名称  | 字符串  |  用户在游戏开始的时候可以指定一个玩家名称，后面可以查看排名  |
| `status_code`  | 当前操作的返回状态  | 数字  |  一些操作出现了错误，会在这个地方进行显示  |

上面是一个简表，其它一些具体的解释后面也会给出来

## 九种基本操作

九中基本操作按传入的参数不同分为两类，开始游戏与其它，因为开始游戏的时候参数会有三个，剩下的八种操作都只有两个参数，而且参数名称都是一样的。

## 开始游戏

开始游戏需要传入三个参数

| 参数名称  | 值  | 类型  | 备注  |
|---|---|---|---|
| `oper`  | `start`  | 字符串  | -  |
| `player_name`  | 用户自定义的名称  |  字符串 | 未来会在排行榜上看到这个名称  |
| `rand_key`  | 随机种子  |  整数 |  游戏开始的随机数，可以复现一个游戏 |

上面特别注意的是里面的`rand_key`，如果`rand_key`不为`0`的话，那么就会以这个值为随机种子，这样用户接下来随机出来的砖块实际上都是有规律的，用户使用同样的`rand_key`会得到相同的结果，这样方便用户进行训练和专门优化。如果`rand_key`的值为0的话，那么就会按照当前的时间戳做为随机种子，这样用户就不能够预测游戏的值了。

未来过程中使用`rand_key`为0的用户和非`0`的用户将会分别排名。

在用户开始游戏之后，服务器会返回一个`token_id`，`token_id`用来在服务器上标识一个具体的游戏，在服务器上是唯一的，未来用户将会使用空上`token_id`来操作游戏。

## 其它操作

剩下的操作只会有两个参数

| 参数名称  | 值  | 类型  | 备注  |
|---|---|---|---|
| `oper`  | `start`  | 字符串  | 一共八种，后面有介绍  |
| `token_id`  | 标识游戏的唯一ID  |  字符串 | 由开始游戏之后服务器返回  |

下表是不同的`oper`代表的含义

| 操作名称  | 备注  |
|---|---|
| `stop`  | 直接终止游戏，只有当用户开始游戏之后，才能够中止游戏  |
| `pause`  | 暂停游戏，当前情况下用户要以暂时游戏一段时间，不过一个游戏最多暂停24小时，之后会自动停止游戏进行game_over状态  |
| `resume`  | 唤醒游戏，暂停游戏之后，可以调用这个参数来唤醒游戏  |
| `up`  | 方块变换，遵守传统的俄罗斯广场的操作规则，使用`up`来进行变换  |
| `down`  | 向下移动一格  |
| `left`  | 向左移动一格  |
| `right`  | 向右移动一格  |
| `get_state`  | 不做任何操作，只得到当前游戏的状态，一般用户可以在本地进行每一秒一次的刷新，以得到当前的游戏界面状态，因为一个游戏开始之后，会在服务端进行执行，每隔一秒就会向下移动一格，因此用户有必要进行定时刷新，以得到最新的游戏状态，以便做相应的判断。  |
