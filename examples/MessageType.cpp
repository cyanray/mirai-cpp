#include <iostream>
#include <algorithm>
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
			cout << gm.MessageChain.ToString() << endl;
			MessageChain mc = gm.MessageChain;

			// 把收到的图片再发回去
			vector<ImageMessage> imgs = mc.GetAll<ImageMessage>();
			if (imgs.size() != 0)
			{
				gm.QuoteReply(MessageChain().Plain("你发了 ").Plain(imgs.size()).Plain(" 张图"));
				for (auto&& img : imgs)
				{
					gm.Reply(MessageChain().FlashImage(img.ToMiraiImage()));
					MiraiBot::SleepMilliseconds(500);
				}
			}

			// 如果有人发微笑，就问候对方 (微笑的id是14)
			vector<FaceMessage> faceId = mc.GetAll<FaceMessage>();
			auto result = find_if(faceId.begin(), faceId.end(), [](FaceMessage m) {return m.FaceId() == 14; });
			if (result != faceId.end())
			{
				gm.QuoteReply(MessageChain().Face(14));
			}

			// 如果有人At我，就问候对方
			if (gm.AtMe())
			{
				gm.QuoteReply(MessageChain().Face(14));
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