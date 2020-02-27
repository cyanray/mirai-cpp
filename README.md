# mirai-cpp

## 项目简介

[mirai](https://github.com/mamoe/mirai) 是全开源 QQ 机器人 / QQ 协议支持库。

本项目为 mirai-http-api 的 C++ 封装，方便使用 C++ 开发 mirai-http-api 插件

## 项目依赖

1. **libcurl** HTTP 协议。

2. [**nlohmann/json**](https://github.com/nlohmann/json) 解析 JSON 数据。

3. [**CURLWrapper**](https://github.com/cyanray/CURLWrapper) 对 **libcurl** 的封装。

## 如何使用

### 1. 快速尝试

<details>

（以下内容基于 Windows 10 平台，使用 Visual Studio 2019 作为开发软件。）

本项目使用了 3 个第三方项目，其中 **CURLWrapper** 已经嵌入到本项目，而 **libcurl** 以及 **nlohmann/json** 需要额外安装。

有很多方法可以在你的电脑上下载并安装 **libcurl** 和 **nlohmann/json** 库，这里介绍一种更不容易出错的方法。

在这一切开始之前，你需要下载并安装 [**Git for windows**](https://gitforwindows.org/), 如果你已经安装并且很熟悉它，那么可以略过这个步骤。如果你不熟悉，在这之后可以去了解一下什么是 **Git** 。

然后，我们需要安装 [**vcpkg**](https://github.com/microsoft/vcpkg) , 这是一个来自微软的跨平台的 C++ 库管理器。如果你已经安装了 **vcpkg** 并且很熟悉它，那么可以略过这个步骤。

#### (1) 安装 **vcpkg** (如果你已经安装则可以略过)

1. 打开 Powershell ，找到一个合适的位置，执行以下命令：

```shell
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

2. 如果上面的代码执行无误，那么 **vcpkg** 已经成功编译。执行下面的命令让 **Visual Studio 2019** 与 **vcpkg** 相关联

```shell
.\vcpkg integrate install
```

#### (2) 使用 **vcpkg** 安装 libcurl 和 **nlohmann/json**

完成这一步你只需执行：

```shell
./vcpkg install curl nlohmann-json
```

#### (3) 使用 **vcpkg** 安装 **mirai-cpp**

这一步稍微复杂，你需要执行：

```shell
git clone https://github.com/cyanray/ports.git tmp ; mv tmp/* ports/ ; rm -Recurse -Force tmp
./vcpkg install mirai-cpp
```

#### (4) 在 **Visual Studio** 中创建一个项目，开始使用

尝试以下代码：

```c++
#include <iostream>
#include <mirai.hpp>

int main()
{
	using namespace std;
	using namespace Cyan;
	MiraiBot bot;
	while (true)
	{
		try
		{
            // InitKeyVl0CEUzZ 改为你的 InitKey，
            // 2110000000 改为你的 bot 的 QQ 号码
			bot.Auth("InitKeyVl0CEUzZ", 2110000000ll);
			break;
		}
		catch (const std::exception & ex)
		{
			cout << ex.what() << endl;
		}
	}
	cout << "成功登录 bot。" << endl;


	bot.OnFriendMessageReceived(
		[&](FriendMessage fm)
		{
			bot.SendFriendMessage(fm.Sender.QQ, fm.MessageChain);
		});

	bot.OnGroupMessageReceived(
		[&](GroupMessage gm)
		{
			bot.SendGroupMessage(gm.Sender.Group.GID, "为什么要 " + gm.MessageChain);
		});

	try
	{
		bot.EventLoop();
	}
	catch (const std::exception & ex)
	{
		cout << ex.what() << endl;
	}

	return 0;
}
```

如果一切正常，给你的机器人发消息，他会回复同样的消息给你！

</details>

### 2. 其他使用方式

未完待续……

## 代码风格

本项目的代码使用的是我自己喜欢的代码风格，如果你有更好的建议（比如修改为 Google-Style），欢迎提出 issues 或 pull request。
