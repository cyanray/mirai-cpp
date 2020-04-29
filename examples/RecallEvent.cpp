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


	bot.On<GroupRecallEvent>(
		[&](GroupRecallEvent gm)
		{
			try
			{
				auto mc = "刚刚有人撤回了: " + bot.GetGroupMessageFromId(gm.MessageId).MessageChain;
				bot.SendMessage(gm.Group.GID, mc);
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	bot.On<FriendRecallEvent>(
		[&](FriendRecallEvent gm)
		{
			try
			{
				auto mc = "刚刚有人撤回了: " + bot.GetFriendMessageFromId(gm.MessageId).MessageChain;
				bot.SendMessage(gm.AuthorQQ, mc);
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});


	bot.EventLoop();

	return 0;
}