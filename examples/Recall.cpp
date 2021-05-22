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
			try
			{
				MessageId_t id = gm.Reply(gm.MessageChain);
				MiraiBot::SleepSeconds(5);
				bot.Recall(id);
				MiraiBot::SleepSeconds(2);
				// TODO: 不要撤回群主的消息，如果是BOT不是群主，撤回了群主的消息，会抛出异常
				gm.Recall();
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});


	bot.EventLoop();

	return 0;
}