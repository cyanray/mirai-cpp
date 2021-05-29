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

	// 获取群组列表
	auto groups = bot.GetGroupList();
	for (const auto& group : groups)
	{
		cout << "* " << group.GID << ", " << group.Name << ", " << GroupPermissionStr(group.Permission) << endl;
		auto members = bot.GetGroupMembers(group.GID);
		int i = 1;
		for (const auto& member : members)
		{
			cout << "   " << (i++) << ". " << member.MemberName << "(" << member.QQ << ") " << GroupPermissionStr(member.Permission);
			cout << ", " << (member.MemberName.empty() ? "无" : member.MemberName)		// 群名片
				 << ", " << (member.SpecialTitle.empty() ? "无" : member.SpecialTitle)	// 群头衔
				 << endl;
		}
		cout << endl;
	}

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