#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>

int main()
{
	using namespace std;
	using namespace Cyan;

	// 源文件使用 UTF-8 编码保存，在 Windows 上需要切换代码页才不会显示乱码
#if defined(WIN32) || defined(_WIN32)
system("chcp 65001");
#endif	

	MiraiBot bot("127.0.0.1", 539);
	
	// 检查一下版本
	try
	{
		// 获取 mirai-api-http 插件地版本
		string current_version = bot.GetApiVersion();
		// 获取 mirai-cpp 适配的版本
		string required_version = bot.GetRequiredApiVersion();
		cout << "! 需要的 API 版本: " << required_version
			<< "; 当前 API 版本: " << current_version << "; " << endl;
		if (current_version != required_version)
		{
			cout << "! 警告: 你的 mirai-api-http 插件的版本与 mirai-cpp 适配的版本不同，可能存在潜在的异常。" << endl;
		}
	}
	catch (const std::exception&ex)
	{
		cout << ex.what() << endl;
	}

	// 自动重试地进行 Auth
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
	cout << "Bot Working..." << endl;

	// 监听各类事件
	bot.On<GroupMessage>(
		[&](GroupMessage m)
		{
			m.QuoteReply(m.MessageChain);
		});
	// 可以多次监听同一事件，不保证执行顺序
	bot.On<GroupMessage>(
		[&](GroupMessage m)
		{
			m.Reply("2222 " + m.MessageChain);
		});
	
	bot.On<FriendMessage>(
		[&](FriendMessage m)
		{
			m.Reply("你好呀, " + m.MessageChain);
		});

	bot.On<TempMessage>(
		[&](TempMessage m)
		{
			m.Reply(m.MessageChain);
		});


	// 记录轮询事件时的错误
	bot.EventLoop([](const char* errMsg)
		{
			cout << "获取事件时出错: " << errMsg << endl;
		});

	return 0;
}