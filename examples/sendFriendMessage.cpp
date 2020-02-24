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

	GroupImage gImg = bot.UploadGroupImage("D:/QQ20200223215250.jpg");

	MessageChain messageChain;
	messageChain
		.Face(4)
		.Plain("\n")
		.Plain("群消息测试")
		.Plain("🤣🤣🤣")
		.Image(gImg);

	cout << messageChain.ToString() << endl;

	bot.OnGroupMessageReceived(
		[&](GroupMessage gm)
		{
			bot.SendGroupMessage(gm.Sender.Group.GID, gm.MessageChain);
		});

	try
	{
		bot.EventLoop();
		//bot.SendFriendMessage(484599279ll, messageChain);
		//bot.UnMute(1013323391ll, 484599279ll);
		//bot.SendGroupMessage(1013323391ll, messageChain);
	}
	catch (const std::exception & ex)
	{
		cout << ex.what() << endl;
	}


	bot.Release();
	return 0;
}