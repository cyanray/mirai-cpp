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


	bot.OnFriendMessageReceived(
		[&](FriendMessage fm)
		{
			cout << fm.MessageChain.ToString() << endl;
			cout << fm.MessageChain.GetTimestamp() << endl;
			cout << fm.GetMessageId() << ", " << fm.GetTimestamp() << endl;
			try
			{
				bot.SendMessage(fm.Sender.QQ, fm.MessageChain);
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	bot.OnGroupMessageReceived(
		[&](GroupMessage gm)
		{
			cout << gm.MessageChain.ToString() << endl;
			cout << gm.GetMessageId() << ", " << gm.GetTimestamp() << endl;
			try
			{
				bot.SendMessage(gm.Sender.Group.GID, "为什么要 " + gm.MessageChain);
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});

	bot.OnTempMessageReceived(
		[&](TempMessage tm)
		{
			cout << tm.MessageChain.ToString() << endl;
			cout << tm.GetMessageId() << ", " << tm.GetTimestamp() << endl;
			try
			{
				bot.SendMessage(tm.Sender.Group.GID, tm.Sender.QQ, "为什么要 " + tm.MessageChain);
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