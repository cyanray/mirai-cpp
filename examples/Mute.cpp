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

	bot.On<MemberMuteEvent>([&](MemberMuteEvent e)
		{
			auto mc = MessageChain()
				.Plain("恭喜老哥 " + e.Member.MemberName + " 喜提禁言套餐!");
			bot.SendMessage(e.Member.Group.GID, mc);
		});

	bot.On<MemberUnmuteEvent>([&](MemberUnmuteEvent e)
		{
			auto mc = MessageChain()
				.Plain("恭喜老哥 " + e.Member.MemberName + " 提前出狱!");
			bot.SendMessage(e.Member.Group.GID, mc);
		});

	bool res = false;
	res = bot.MuteAll(1029259687_gid);
	if (res)
	{
		cout << "全体禁言成功！" << endl;
	}
	else
	{
		cout << "全体禁言失败" << endl;
	}

	MiraiBot::SleepSeconds(5);

	res = bot.UnMuteAll(1029259687_gid);
	if (res)
	{
		cout << "解除全体禁言成功！" << endl;
	}
	else
	{
		cout << "解除全体禁言失败" << endl;
	}

	res = bot.Mute(1029259687_gid, 211795583_qq, 60);
	if (res)
	{
		cout << "禁言群员成功！" << endl;
	}
	else
	{
		cout << "禁言群员失败" << endl;
	}

	MiraiBot::SleepSeconds(5);

	res = bot.UnMute(1029259687_gid, 211795583_qq);
	if (res)
	{
		cout << "解除禁言群员成功！" << endl;
	}
	else
	{
		cout << "解除禁言群员失败" << endl;
	}


	bot.EventLoop();


	return 0;
}