#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
// 按需引用头文件
// 你也可以使用 #include <mirai.h> 引用所有头文件(可能导致编译缓慢)
#include <mirai/MiraiBot.hpp>
#include <mirai/events/OtherClientMessage.hpp>
#include <mirai/events/FriendSyncMessage.hpp>
using namespace std;
using namespace Cyan;

int main(int argc, char* argv[])
{
	// 源文件使用 UTF-8 编码保存，在 Windows 上需要切换代码页才不会显示乱码
#if defined(WIN32) || defined(_WIN32)
	system("chcp 65001");
#endif	

	MiraiBot bot;
	SessionOptions opts = SessionOptions::FromCommandLine(argc, argv);

	// 自动重试地与 mirai-api-http 建立连接
	while (true)
	{
		try
		{
			bot.Connect(opts);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}

	// 检查一下版本
	try
	{
		string mah_version = bot.GetMiraiApiHttpVersion();
		string mc_version = bot.GetMiraiCppVersion();
		cout << "mirai-api-http 的版本: " << mah_version
			<< "; mirai-cpp 的版本: " << mc_version << "; " << endl;
		if (mah_version != mc_version)
		{
			cout << "Warning: 你的 mirai-api-http 插件的版本与 mirai-cpp 的版本不同，可能存在兼容性问题。" << endl;
		}
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}

	cout << "Bot Working..." << endl;

	// 修改 groupId 以测试发送群公告！
	auto groupId = 12345678_gid;
	if (groupId != 12345678_gid)
	{
		try
		{
			MiraiImage img;
			// 设置 img 的 Path、Base64 或 Url 以设置群公告图片（设置 Id 无效）
			// img.Base64 = "...";
			auto options = GroupAnnouncement::PublishFlags::Pinned | GroupAnnouncement::PublishFlags::RequireConfirmation;
			auto result = bot.PublishGroupAnnouncement(groupId, "测试测试222", options, img);
			cout << "已经发送群公告：" << result.Content << endl;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
	}
	else
	{
		cout << "修改 groupId 以测试发送群公告！" << endl;
	}

	bot.On<LostConnection>([&](LostConnection e)
		{
			cout << e.ErrorMessage << " (" << e.Code << ")" << endl;
			while (true)
			{
				try
				{
					cout << "尝试与 mirai-api-http 重新建立连接..." << endl;
					bot.Reconnect();
					break;
				}
				catch (const std::exception& ex)
				{
					cout << ex.what() << endl;
				}
				MiraiBot::SleepSeconds(1);
			}
			cout << "成功与 mirai-api-http 重新建立连接!" << endl;
		});

	bot.On<EventParsingError>([&](EventParsingError e)
		{
			try
			{
				e.Rethrow();
			}
			catch (const std::exception& ex)
			{
				cout << "解析事件时出现错误: " << ex.what() << endl;
			}
		});

	string command;
	while (cin >> command)
	{
		if (command == "exit") break;
	}

	// 程序结束前必须释放 Session, 否则会导致 mirai-api-http 内存泄漏
	bot.Disconnect();

	return 0;
}