### 编译本项目需要下面几个库，请先安装好它们：
	1. boost：主要使用了boost::asio
	2. protobuf：服务端与客户端的消息格式
	3. curl：服务端用来进行HTTP POST
	4. Visual Leak Detector：内存泄露检测，在windows上编译需要它


### 如何编译
#### 使用visual studio
	在Windows上直接打开vsprojects\build.sln来构建
#### 使用xcode
	在MacOS上直接打开xcodeprojects\build.xcworkspace来构建
#### 使用gyp
	在Linux上，使用Google的gyp构建系统来构建本项目：
	$ gyp --depth=. --generator-output=build
	$ cd build
	$ make


### 运行时还依赖如下服务：

####
	[wapis](https://github.com/avexer/wapis "wapis")：目前服务端通过该web service进行登录认证


### 接下来得找工作了，很多东西还没完善，有时间再搞吧。