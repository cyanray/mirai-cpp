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
			bot.Verify("INITKEY7A3O1a9v", 1589588851_qq);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "成功登录 bot。" << endl;

	bot.On<GroupMessage>(
		[&](GroupMessage gm)
		{
			gm.QuoteReply(gm.MessageChain);
		})
		.On<FriendMessage>(
		[&](FriendMessage fm)
		{
			fm.Reply("你好呀, " + fm.MessageChain);
		})
		.On<TempMessage>(
		[&](TempMessage tm)
		{
			tm.Reply(tm.MessageChain);
		});

	// 默认使用 WebSocket 拉取事件、消息
	// 如果要使用 HTTP 可以在 EventLoop 前执行 UseHTTP
	// 记录轮询事件时的错误
	bot.UseHttp().EventLoop([](const char* errMsg)
		{
			cout << "轮询事件时出错: " << errMsg << endl;
		});

	// 无参数则会在 cerr 输出错误
	// bot.EventLoop();

	return 0;
}