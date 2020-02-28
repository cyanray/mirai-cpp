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

	FriendImage img = bot.UploadFriendImage("D:\\pic_1.png");

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