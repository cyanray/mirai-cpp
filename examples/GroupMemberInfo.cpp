#include <iostream>
#include <ctime>
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
			bot.Verify("INITKEY7A3O1a9v", 1589588851_qq);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "成功登录 bot。" << endl;

	srand(time(0));

	auto gid = 1029259687_gid;
	auto qq = 1589588851_qq;

	auto memberInfo = bot.GetGroupMemberInfo(gid, qq);
	cout << "群名片: " << memberInfo.Name << ", 群头衔: " << memberInfo.SpecialTitle << endl;
	// 设置新的群昵称
	bot.SetGroupMemberName(gid, qq, "name_" + to_string(rand() % 1000));
	// 设置新的群头衔 (仅群主有权限, 没有权限会抛出异常)
	bot.SetGroupMemberSpecialTitle(gid, qq, "title_" + to_string(rand() % 1000));

	// 也可以修改了 memberInfo 之后,使用 SetGroupMemberInfo 方法
	// bot.SetGroupMemberInfo(gid, qq, memberInfo);

	bot.EventLoop();
	return 0;
}