#include <iostream>
#include <ctime>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>

int main()
{
	using namespace std;
	using namespace Cyan;
	system("chcp 65001");
	MiraiBot bot("127.0.0.1", 539);
	while (true)
	{
		try
		{
			bot.Auth("INITKEY7A3O1a9v", 1589588851_qq);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "成功登录 bot。" << endl;

	try
	{
		bot.RegisterCommand("hello", { "hi" }, "helloddd", "helllll");
		auto managers = bot.GetManagers();
		for (auto manager : managers)
		{
			cout << "manager : " << manager << endl;
		}
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}

	bot.On<Command>(
		[&](Command e)
		{
			cout << "收到指令: " << e.CommandName << ", "
				<< "发送者: " << e.Sender.ToInt64() << ", "
				<< "发送群: " << e.GroupId.ToInt64() << endl;
			cout << "参数:" << endl;
			for (const auto& arg : e.Args)
				cout << arg << " ";
			cout << endl;
		});

	bot.SendCommand("hello", {});

	bot.EventLoop();
	return 0;
}