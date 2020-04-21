#include <iostream>
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

	bot.On<BotMuteEvent>(
		[&](BotMuteEvent e)
		{
			cout << "爷被禁言了!" << endl;
		});

	bot.On<BotUnmuteEvent>(
		[&](BotUnmuteEvent e)
		{
			bot.SendMessage(e.Operator.Group.GID,
				MessageChain().Plain("👴 出狱了!"));
		});



	try
	{
		bot.EventLoop();
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}

	return 0;
}