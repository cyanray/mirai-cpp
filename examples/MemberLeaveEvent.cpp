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

	bot.OnEventReceived<MemberLeaveEventKick>(
		[&](MemberLeaveEventKick e)
		{
			auto mc = MessageChain().Plain("恭喜老哥 " + e.Member.MemberName + " 喜提飞机票!");
			bot.SendMessage(e.Member.Group.GID, mc);
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