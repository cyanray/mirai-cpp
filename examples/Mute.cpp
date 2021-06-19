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

	bot.On<MemberMuteEvent>([&](MemberMuteEvent e)
		{
			// 机器人的操作不做处理
			if (e.OperatorIsBot())
			{
				cout << "机器人禁言操作" << endl;
				return;
			}
			auto mc = MessageChain()
				.Plain("恭喜老哥 " + e.Member.MemberName + " 喜提禁言套餐!");
			bot.SendMessage(e.Member.Group.GID, mc);
		});

	bot.On<MemberUnmuteEvent>([&](MemberUnmuteEvent e)
		{
			// 机器人的操作不做处理
			if (e.OperatorIsBot())
			{
				cout << "机器人解除禁言操作" << endl;
				return;
			}
			auto mc = MessageChain()
				.Plain("恭喜老哥 " + e.Member.MemberName + " 提前出狱!");
			bot.SendMessage(e.Member.Group.GID, mc);
		});

	bot.On<GroupMuteAllEvent>([&](GroupMuteAllEvent e)
		{
			// 机器人的操作不可访问 Operator
			if (e.OperatorIsBot())
			{
				cout << "机器人群禁言操作" << endl;
				return;
			}
		if(e.Current)
			cout << e.Operator.MemberName << " 开启了全群禁言." << endl;
		else
			cout << e.Operator.MemberName << " 关闭了全群禁言." << endl;
		
		});
	
	try
	{
		bot.MuteAll(1029259687_gid);
		cout << "全体禁言成功！" << endl;
	}
	catch (const exception& ex)
	{
		cout << "全体禁言失败: " << ex.what() << endl;
	}


	MiraiBot::SleepSeconds(5);

	try
	{
		bot.UnMuteAll(1029259687_gid);
		cout << "解除全体禁言成功！" << endl;
	}
	catch (const exception& ex)
	{
		cout << "解除全体禁言失败: " << ex.what() << endl;
	}

	try
	{
		bot.Mute(1029259687_gid, 211795583_qq, 60);
		cout << "禁言群成员成功！" << endl;
	}
	catch (const exception& ex)
	{
		cout << "禁言群成员失败: " << ex.what() << endl;
	}


	MiraiBot::SleepSeconds(5);

	try
	{
		bot.UnMute(1029259687_gid, 211795583_qq);
		cout << "解除禁言群成员成功！" << endl;
	}
	catch (const exception& ex)
	{
		cout << "解除禁言群成员失败: " << ex.what() << endl;
	}


	bot.EventLoop();

	return 0;
}