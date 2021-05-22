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





	bot.EventLoop();


	return 0;
}