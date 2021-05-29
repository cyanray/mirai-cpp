#include <iostream>
#include <ctime>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>
#include <mirai/exceptions/exceptions.hpp>

using namespace std;
using namespace Cyan;

int main(int argc, char* argv[])
{

	system("chcp 65001");
	MiraiBot bot;
	SessionOptions opts = SessionOptions::FromCommandLine(argc, argv);
	bot.Connect(opts);

	srand(time(0));

	auto gid = 1013323391_gid;
	auto& qq = opts.BotQQ.Get();

	auto memberInfo = bot.GetGroupMemberInfo(gid, qq);
	cout << "群名片: " << memberInfo.MemberName << ", 群头衔: " << memberInfo.SpecialTitle << endl;

	try
	{
		// 设置新的群昵称
		bot.SetGroupMemberName(gid, qq, "name_" + to_string(rand() % 1000));

		// 设置新的群头衔 (仅群主有权限, 没有权限会抛出异常)
		bot.SetGroupMemberSpecialTitle(gid, qq, "title_" + to_string(rand() % 1000));
	}
	catch (const NetworkException& ex)
	{
		cout << "出现网络错误!" << endl;
	}
	catch (const MiraiApiHttpException& ex)
	{
		cout << "mirai-api-http 报告错误:" << ex.what() << endl;
	}
	catch (...)
	{
		cout << "出现未知错误." << endl;
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