#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>

using namespace std;
using namespace Cyan;

void print(const Profile& p)
{
	cout << "昵称:" << p.NickName << endl;
	cout << "邮箱:" << p.Email    << endl;
	cout << "签名:" << p.Sign     << endl;
	cout << "年龄:" << p.Age      << endl;
	cout << "性别:" << p.Sex      << endl;
	cout << "等级:" << p.Level    << endl;
	cout << endl;
}

int main(int argc, char* argv[])
{
	system("chcp 65001");

	MiraiBot bot;
	SessionOptions opts = SessionOptions::FromCommandLine(argc, argv);
	bot.Connect(opts);
	cout << "Bot working..." << endl;

	// 获取一个好友用于测试
	QQ_t friendQQ = bot.GetFriendList().front().QQ;
	// 获取一个群成员用于测试
	GroupMember groupMember;
	for (const auto& group : bot.GetGroupList())
	{
		auto members = bot.GetGroupMembers(group.GID);
		if (members.size() != 0)
		{
			groupMember = members.front();
		}
	}


	Profile botProfile = bot.GetBotProfile();
	print(botProfile);
	Profile friendProfile = bot.GetFriendProfile(friendQQ);
	print(friendProfile);
	Profile groupMemberProfile = bot.GetGroupMemberProfile(groupMember.Group.GID, groupMember.QQ);
	print(groupMemberProfile);

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