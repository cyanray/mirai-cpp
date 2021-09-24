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

	try
	{
		auto friends = bot.GetFriendList();
		int i = 1;
		for (const auto& f : friends)
		{
			cout << (i++) << ". " << f.QQ << ", " << f.NickName << ", " << f.Remark << endl;
		}
	}
	catch (const std::exception& ex)
	{
		cerr << ex.what() << endl;
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