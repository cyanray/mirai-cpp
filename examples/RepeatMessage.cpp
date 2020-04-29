#include <iostream>
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

	bot.OnEventReceived<GroupMessage>(
		[&](GroupMessage gm)
		{
			gm.QuoteReply(gm.MessageChain);
		});

	bot.OnEventReceived<FriendMessage>(
		[&](FriendMessage fm)
		{
			fm.Reply("你好呀, " + fm.MessageChain);
		});

	bot.OnEventReceived<TempMessage>(
		[&](TempMessage tm)
		{
			tm.Reply(tm.MessageChain);
		});


	// 记录轮询事件时的错误
	bot.EventLoop([](const char* errMsg) 
		{
			cout << "轮询事件时出错: " << errMsg << endl;
		});

	// 默认参数是在 cerr 输出错误
	// bot.EventLoop();

	return 0;
}