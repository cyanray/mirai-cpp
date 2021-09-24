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

	bot.On<MemberJoinRequestEvent>(
		[&](MemberJoinRequestEvent newMember)
		{
			cout << newMember.Message << endl;
			newMember.Accept();
		});

	bot.On<MemberJoinEvent>(
		[&](MemberJoinEvent e)
		{
			string memberName = e.NewMember.MemberName;
			cout << memberName << endl;
			bot.SendMessage(e.NewMember.Group.GID,

				MessageChain().Plain("欢迎 " + memberName + " 加入本群!"));
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