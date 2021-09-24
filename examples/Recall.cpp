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


	bot.On<GroupMessage>(
		[&](GroupMessage gm)
		{
			try
			{
				MessageId_t id = gm.Reply(gm.MessageChain);
				MiraiBot::SleepSeconds(5);
				bot.Recall(id);
				MiraiBot::SleepSeconds(2);
				// TODO: 不要撤回群主的消息，如果是BOT不是群主，撤回了群主的消息，会抛出异常
				gm.Recall();
			}
			catch (const std::exception& ex)
			{
				cout << ex.what() << endl;
			}
		});


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