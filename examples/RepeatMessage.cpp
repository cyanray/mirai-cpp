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
	try
	{
		string current_version = bot.GetApiVersion();
		string required_version = bot.GetRequiredApiVersion();
		cout << "! 需要的 API 版本: " << required_version
			<< "; 当前 API 版本: " << current_version << "; " << endl;
		if (current_version != required_version)
		{
			cout << "! 警告: 你的 mirai-api-http 插件的版本与 mirai-cpp 适配的版本不同，可能存在潜在的异常。" << endl;
		}
	}
	catch (const std::exception&) {}
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