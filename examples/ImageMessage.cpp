#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>
using namespace std;
using namespace Cyan;

int main(int argc, char* argv[])
{
	system("chcp 65001");
	MiraiBot bot;
	SessionOptions opts = SessionOptions::FromCommandLine(argc, argv);
	bot.Connect(opts);
	cout << "Bot working..." << endl;

	string ImagePath("E:/test.png");

	bot.On<FriendMessage>(
		[&](FriendMessage m)
		{
			try
			{
				FriendImage img = bot.UploadFriendImage(ImagePath);
				m.Reply(MessageChain().Image(img));
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
				GroupImage gImg = bot.UploadGroupImage(ImagePath);
				bot.SendMessage(m.Sender.Group.GID, MessageChain().Image(gImg));
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	bot.On<TempMessage>(
		[&](TempMessage m)
		{
			try
			{
				TempImage tImg = bot.UploadTempImage(ImagePath);
				m.Reply(MessageChain().Image(tImg));
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	string command;
	while (cin >> command)
	{
		if (command == "exit")
		{
			bot.Disconnect();
			break;
		}
	}

	return 0;
}