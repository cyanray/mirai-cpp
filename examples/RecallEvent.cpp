#include <iostream>
#include <mirai.hpp>

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
			bot.Auth("INITKEY7A3O1a9v", 1589588851qq);
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
	try
	{

	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}

	return 0;
}