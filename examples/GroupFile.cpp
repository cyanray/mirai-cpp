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
		// 获取群组列表
		auto groups = bot.GetGroupList();
		if (!groups.empty())
		{
			// 取群列表的第一个群
			auto& group = groups.front();
			// 获取该群的群文件列表
			vector<GroupFile> fileList = bot.GetGroupFiles(group.GID, true);
			if (fileList.empty())
			{
				cout << "这个群没有群文件!" << endl;
			}
			for (int i = 0; i < fileList.size(); i++)
			{
				cout << i << ". " << fileList[i].Name << endl;
				if (fileList[i].DownloadInfo)
				{
					cout << fileList[i].DownloadInfo->ToString() << endl;
				}
			}
		}
		else
		{
			cout << "你的机器人还没有加群噢!" << endl;
		}
	}
	catch (const std::exception& ex)
	{
		cout << "出错:" << ex.what() << endl;
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