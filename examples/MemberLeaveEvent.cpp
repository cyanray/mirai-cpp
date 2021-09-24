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

	bot.On<MemberLeaveEventKick>(
		[&](MemberLeaveEventKick e)
		{
			auto mc = MessageChain().Plain("恭喜老哥 " + e.Member.MemberName + " 喜提飞机票!");
			bot.SendMessage(e.Member.Group.GID, mc);
		});

	bot.On<MemberLeaveEventQuit>(
		[&](MemberLeaveEventQuit e)
		{
			auto mc = MessageChain().Plain("老哥 " + e.Member.MemberName + " 主动离开了群聊!");
			bot.SendMessage(e.Member.Group.GID, mc);
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