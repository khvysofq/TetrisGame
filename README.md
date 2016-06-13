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
schannel.obj : error LNK2019: unresolved external symbol __imp__CertFreeCertificateContext@4 referenced in function _schannel_connect_step3
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