### 编译本项目需要下面几个库，请先安装好它们：
1. boost：主要使用了boost::asio

2. protobuf：服务端与客户端的消息格式

3. curl：服务端用来进行HTTP POST

4. Visual Leak Detector：内存泄露检测，在windows上编译需要它


### 如何编译
1. visual studio
2. xcode
3. gyp

	`gyp --depth=. --generator-output=build`




### 运行时还依赖如下服务：

1. [wapis](https://github.com/avexer/wapis "wapis")：目前服务端通过该web service进行登录认证

