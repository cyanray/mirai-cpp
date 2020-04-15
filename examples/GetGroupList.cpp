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
	}
	cout << "成功登录 bot。" << endl;

	auto groups = bot.GetGroupList();
	int i = 1;
	for (const auto& g : groups)
	{
		cout << (i++) << ". " << g.GID << ", " << g.Name << ", " << GroupPermissionStr(g.Permission) << endl;
		auto gMembers = bot.GetGroupMembers(g.GID);
		for (const auto& gMem : gMembers)
		{
			cout << " | " << gMem.QQ << ", " << gMem.MemberName << ", " << GroupPermissionStr(gMem.Permission) << endl;
		}
	}

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