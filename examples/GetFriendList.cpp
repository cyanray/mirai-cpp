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
			bot.Auth("INITKEY7A3O1a9v", 1589588851ll);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
	}
	cout << "³É¹¦µÇÂ¼ bot¡£" << endl;

	auto friends = bot.GetFriendList();
	int i = 1;
	for (const auto& f : friends)
	{
		cout << (i++) << ". " << f.QQ << ", " << f.NickName << ", " << f.Remark << endl;
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