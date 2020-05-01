#include <iostream>
#include <algorithm>
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

	bot.On<GroupMessage>(
		[&](GroupMessage gm)
		{
			cout << gm.MessageChain.ToString() << endl;
			MessageChain mc = gm.MessageChain;

			// 把收到的图片再发回去
			vector<MiraiImage> imgs = mc.GetImage();
			if (imgs.size() != 0)
			{
				gm.QuoteReply(MessageChain().Plain("你发了 ").Plain(imgs.size()).Plain(" 张图"));
				for (auto&& img : imgs)
				{
					gm.Reply(MessageChain().FlashImage(img));
					MiraiBot::SleepMilliseconds(500);
				}
			}

			// 如果有人发微笑，就问候对方 (微笑的id是14)
			vector<int> faceId = mc.GetFace();
			auto result = find(faceId.begin(), faceId.end(), 14);
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

	bot.UseWebSocket().EventLoop();

	return 0;
}