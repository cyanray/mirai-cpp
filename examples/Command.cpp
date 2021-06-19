#include <iostream>
#include <ctime>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>

using namespace std;
using namespace Cyan;

int main(int argc, char* argv[])
{
	system("chcp 65001");

	MiraiBot bot;
	SessionOptions opts = SessionOptions::FromCommandLine(argc, argv);
	bot.Connect(opts);

	cout << "Bot working..." << endl;

	// 指令事件
	bot.On<Command>(
		[&](Command e)
		{
			cout << e.ToJson() << endl;
			cout << "收到指令: " << e.CommandName << ", " << endl;
			cout << "参数:" << endl;
			cout << e.Args.ToString() << endl;
			cout << endl;

		});

	try
	{
		// 注册一个指令
		bot.RegisterCommand("hello", { "hi" }, "指令描述描述描述", "/hello 你想说的话");
		// 调用其他指令(可以调用其他插件注册的指令)
		bot.SendCommand({ "hello", "arg1", "arg2" });
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}

	string command;
	while (cin >> command)
	{
		if (command == "exit")
		{
			bot.Disconnect();
			break;
		}
	}

	return 0;
}