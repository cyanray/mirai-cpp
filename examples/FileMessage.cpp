#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>

int main()
{
	using namespace std;
	using namespace Cyan;
	system("chcp 65001");
	MiraiBot bot("localhost", 8762);
	while (true)
	{
		try
		{
			bot.Verify("VerifyKeyASDEWQ", 1589588851_qq);
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
		[&](GroupMessage m)
		{
			try
			{
				auto files = m.MessageChain.GetAll<FileMessage>();
				for (const auto& file : files)
				{
					m.Reply(MessageChain().Plain("群文件消息: ").Plain(file.ToString()));
				}
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	bot.On<GroupMessage>(
		[&](GroupMessage m)
		{
			try
			{
				// 在机器人所在的群里发 test，让机器人发送一个文件
				if (m.MessageChain.GetPlainTextFirst() != "test") return;
				MiraiFile file = bot.UploadFileAndSend(m.Sender.Group.GID, "/home/uint128/常微分方程及其应用.pdf");
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	bot.EventLoop();

	return 0;
}