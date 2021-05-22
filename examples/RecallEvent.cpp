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


	bot.On<GroupRecallEvent>(
		[&](GroupRecallEvent e)
		{
			try
			{
				if(e.OperatorIsBot())
				{
					cout << "不处理原因：bot 撤回了一条消息" << endl;
					return;
				}
				auto mc = "刚刚有人撤回了: " + bot.GetGroupMessageFromId(e.MessageId).MessageChain;
				auto mid = bot.SendMessage(e.Group.GID, mc);
				MiraiBot::SleepSeconds(5);
				bot.Recall(mid);
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	bot.On<FriendRecallEvent>(
		[&](FriendRecallEvent e)
		{
			try
			{
				auto mc = "刚刚有人撤回了: " + bot.GetFriendMessageFromId(e.MessageId).MessageChain;
				bot.SendMessage(e.AuthorQQ, mc);
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});


	bot.EventLoop();

	return 0;
}