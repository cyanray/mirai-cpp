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

	bot.On<NudgeEvent>(
		[&](NudgeEvent e)
		{
			// 注意: 使用 SendNudge 发送的戳一戳，也会触发该事件,
			// 注意: 因此必须过滤掉来自bot自己的戳一戳事件，不然会导致死循环
			if (e.FromId == bot.GetBotQQ()) return;

			cout << e.FromId.ToInt64() << " " << e.Action << " " << e.Target << " " << e.Suffix << endl;
			// 如果别人戳机器人，那么就让机器人戳回去
			if (e.Target != bot.GetBotQQ()) return;
			bot.SendNudge(e.FromId, *e.GetSubjectId());
			// 如果不喜欢上面这一行代码，也可以用下面的代码代替
			//if (e.FromKind == NudgeEvent::SubjectKind::Group)
			//{
			//	bot.SendNudge(e.FromId, (GID_t)e.RawSubjectId);
			//}
			//else
			//{
			//	bot.SendNudge(e.FromId, (QQ_t)e.RawSubjectId);
			//}
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