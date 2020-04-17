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
	cout << "³É¹¦µÇÂ¼ bot¡£" << endl;

	bot.OnEventReceived<NewFriendEvent>(
		[&](NewFriendEvent newFriend)
		{
			newFriend.Accept();
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