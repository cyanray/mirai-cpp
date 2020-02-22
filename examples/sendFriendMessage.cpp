#include <iostream>
#include <mirai.hpp>

int main()
{
	using namespace std;
	using namespace Cyan;
	system("chcp 65001");
	MiraiBot bot;
	try
	{
		bot.Auth("InitKeyVl0CEUzZ", 211795583ll);
	}
	catch (const std::exception & ex)
	{
		cout << ex.what() << endl;
	}
	cout << "成功登录 bot。" << endl;

	Group_t group;
	group.GID = 1234567ll;
	group.Name = "HELLP";
	group.Permission = GroupPermission::Member;

	cout << group.ToString() << endl;

	string str = R"( {"id":1234567,"name":"HELLP","permission":"MEMBER"} )";
	JsonDoc j; j.Parse(str.data());

	Group_t g2;
	g2.Set(j);

	cout << g2.ToString() << endl;

	vector<Friend_t> fs = bot.GetFriendList();

	for (auto f : fs)
	{
		cout << f.QQ << "\t" << f.NickName << "\t" << f.Remark << endl;

	}

	MessageChain mc;
	mc
		.Face(3)
		.Plain("你好")
		.Plain("\n")
		.Plain("世界!")
		.Plain("🤣🤣🤣");
	try
	{
		bot.SendFriendMessage(484599279ll, mc);
		bot.SendGroupMessage(1029259687ll, MessageChain().Plain("你好\n").Plain("祝福你我的朋友!"));
	}
	catch (const std::exception & ex)
	{
		cout << ex.what() << endl;
	}


	bot.Release();
	return 0;
}