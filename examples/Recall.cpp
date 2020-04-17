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


	bot.On<GroupMessage>(
		[&](GroupMessage gm)
		{
			try
			{
				MessageId id = gm.Reply(gm.MessageChain);
				MiraiBot::SleepSeconds(5);
				bot.Recall(id);
				MiraiBot::SleepSeconds(2);
				gm.Recall();
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