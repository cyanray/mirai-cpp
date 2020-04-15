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
	}
	cout << "成功登录 bot。" << endl;

	FriendImage img = bot.UploadFriendImage("D:\\test.png");
	GroupImage gImg = bot.UploadGroupImage("D:\\test.png");

	bot.OnFriendMessageReceived(
		[&](FriendMessage fm)
		{
			try
			{
				bot.SendFriendMessage(fm.Sender.QQ, MessageChain().Image(img));
			}
			catch (const std::exception & ex)
			{
				cout << ex.what() << endl;
			}
		});

	bot.OnGroupMessageReceived(
		[&](GroupMessage gm)
		{
			try
			{
				bot.SendGroupMessage(gm.Sender.Group.GID, MessageChain().Image(gImg));
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	try
	{
		bot.EventLoop();
	}
	catch (const std::exception & ex)
	{
		cout << ex.what() << endl;
	}

	return 0;
}