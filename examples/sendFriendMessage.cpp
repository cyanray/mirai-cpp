#include <iostream>
#include <mirai.hpp>

int main()
{
	using namespace std;
	using namespace Cyan;
	system("chcp 65001");
	MiraiBot bot;
	while (true)
	{
		try
		{
			bot.Auth("InitKeyVl0CEUzZ", 211795583ll);
			break;
		}
		catch (const std::exception & ex)
		{
			cout << ex.what() << endl;
		}
	}
	cout << "成功登录 bot。" << endl;


	bot.OnFriendMessageReceived(
		[&](FriendMessage fm)
		{
			bot.SendFriendMessage(fm.Sender.QQ, fm.MessageChain);
		});

	bot.OnGroupMessageReceived(
		[&](GroupMessage gm)
		{
			gm.MessageChain = MessageChain().Plain("为什么要 ") + gm.MessageChain;
			bot.SendGroupMessage(gm.Sender.Group.GID, gm.MessageChain);
		});

	try
	{
		bot.EventLoop();
	}
	catch (const std::exception & ex)
	{
		cout << ex.what() << endl;
	}

	bot.Release();
	return 0;
}