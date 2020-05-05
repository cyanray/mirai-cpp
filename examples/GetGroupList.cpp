#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
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
			bot.Auth("INITKEY7A3O1a9v", 1589588851_qq);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "成功登录 bot。" << endl;

	// 获取群组列表
	auto groups = bot.GetGroupList();
	int i = 1;
	for (const auto& g : groups)
	{
		cout << (i++) << ". " << g.GID << ", " << g.Name << ", " << GroupPermissionStr(g.Permission) << endl;
		// 获取群成员列表
		auto gMembers = bot.GetGroupMembers(g.GID);
		for (const auto& gMem : gMembers)
		{
			cout << " | " << gMem.QQ << ", " << gMem.MemberName << ", " << GroupPermissionStr(gMem.Permission);
			// 获取群成员的信息（群名片、群头衔）
			auto info = bot.GetGroupMemberInfo(gMem.Group.GID, gMem.QQ);
			cout << ", 群名片: " << (info.Name.empty() ? "无" : info.Name)
				 << ", 群头衔: " << (info.SpecialTitle.empty() ? "无" : info.SpecialTitle) << endl;
		}
	}


	bot.EventLoop();


	return 0;
}