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

	bool res = false;
	res = bot.MuteAll(1029259687gid);
	if (res)
	{
		cout << "全体禁言成功！" << endl;
	}
	else
	{
		cout << "全体禁言失败" << endl;
	}

	MiraiBot::SleepSeconds(5);

	res = bot.UnMuteAll(1029259687gid);
	if (res)
	{
		cout << "解除全体禁言成功！" << endl;
	}
	else
	{
		cout << "解除全体禁言失败" << endl;
	}

	res = bot.Mute(1029259687gid, 211795583qq, 60);
	if (res)
	{
		cout << "禁言群员成功！" << endl;
	}
	else
	{
		cout << "禁言群员失败" << endl;
	}

	MiraiBot::SleepSeconds(5);

	res = bot.UnMute(1029259687gid, 211795583qq);
	if (res)
	{
		cout << "解除禁言群员成功！" << endl;
	}
	else
	{
		cout << "解除禁言群员失败" << endl;
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