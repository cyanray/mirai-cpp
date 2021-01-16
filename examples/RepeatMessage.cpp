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

	// 16 条事件处理线程
	MiraiBot bot("127.0.0.1", 539, 16);

	// 检查一下版本
	try
	{
		// 获取 mirai-api-http 插件的版本
		string mah_version = bot.GetMiraiApiHttpVersion();
		// 获取 mirai-cpp 的版本
		string mc_version = bot.GetMiraiCppVersion();
		cout << "! mirai-api-http 的版本: " << mah_version
			<< "; 当mirai-cpp 的版本: " << mc_version << "; " << endl;
		if (mah_version != mc_version)
		{
			cout << "! 警告: 你的 mirai-api-http 插件的版本与 mirai-cpp 的版本不同，可能存在兼容性问题。" << endl;
		}
	}
	catch (const std::exception& ex)
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
	// 可以多次监听同一事件，每个处理函数都会被执行，但是不保证执行顺序
	bot.On<GroupMessage>(
		[&](GroupMessage m)
		{
			m.Reply("2222 " + m.MessageChain);
		});

	bot.On<FriendMessage>(
		[&](FriendMessage m)
		{
			m.Reply(m.MessageChain);
		});

	bot.On<TempMessage>(
		[&](TempMessage m)
		{
			m.Reply(m.MessageChain);
		});

	// 通用型消息
	// 收到 FriendMessage、GroupMessage、TempMessage 时都会调用它
	// 判断类型之后，也可调用对应的转换函数进行转换 (类型不正确将转换失败抛出异常)
	bot.On<Message>(
		[&](Message m)
		{
			cout << int64_t(m.Sender) << " 发来一条消息." << endl;
			m.Reply("Message事件可处理三种消息:" + m.MessageChain);

			// 判断是否群组消息
			if (m.GetMessageType() == MessageType::GroupMessage)
			{
				GroupMessage gm = m.ToGroupMessage();
				// TODO: 针对群组消息的特别处理
			}

		});


	// 记录轮询事件时的错误
	bot.EventLoop([](const char* errMsg)
		{
			cout << "获取事件时出错: " << errMsg << endl;
		});

	return 0;
}