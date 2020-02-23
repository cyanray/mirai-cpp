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

	try
	{
		//bot.Kick(1013323391ll, 484599279ll, "gundan");
	}
	catch (const std::exception & ex)
	{
		cout << ex.what() << endl;
	}

	//vector<Group_t> fs = bot.GetGroupList();

	//for (auto f : fs)
	//{
	//	try
	//	{
	//		bot.MuteAll(f.GID);
	//	}
	//	catch (const std::exception & ex)
	//	{
	//		cout << ex.what() << endl;
	//	}

	//	vector<GroupMember_t> gm = bot.GetGroupMembers(f.GID);
	//	for (auto m : gm)
	//	{
	//		cout << m.QQ << " " << m.MemberName << " " << m.Group.Name << endl;
	//	}
	//	cout << endl;

	//}

	FriendImage fImg = bot.UploadFriendImage("D:/b.jpg");

	MessageChain messageChain;
	messageChain
		.Face(3)
		//.Plain("伞兵一号得得得得得得得得得得")
		.Plain("\n")
		.Plain("Hello World!")
		.Plain("🤣🤣🤣")
		.Image(fImg);

	cout << messageChain.ToString() << endl;

	try
	{
		bot.SendFriendMessage(484599279ll, messageChain);
		//bot.UnMute(1013323391ll, 484599279ll);
		//bot.SendGroupMessage(1013323391ll, messageChain);
	}
	catch (const std::exception & ex)
	{
		cout << ex.what() << endl;
	}


	bot.Release();
	return 0;
}