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
		[&](GroupMessage m)
		{
			try
			{
				MiraiVoice voice = bot.UploadGroupVoice("D:\\5.amr");
				m.Reply(MessageChain().Voice(voice));
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});


	bot.EventLoop();

	return 0;
}