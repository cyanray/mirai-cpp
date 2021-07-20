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

	bot.OnEventReceived<NewFriendRequestEvent>(
		[&](NewFriendRequestEvent newFriend)
		{
			newFriend.Accept();
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