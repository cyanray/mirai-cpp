#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>

int main()
{
	using namespace std;
	using namespace Cyan;

	// 源文件使用 UTF-8 编码保存，在 Windows 上需要切换代码页才不会显示乱码
#if defined(WIN32) || defined(_WIN32)
	system("chcp 65001");
#endif	

	// 16 条事件处理线程
	MiraiBot bot("127.0.0.1", 8762, 16);

	// 检查一下版本
	try
	{
		// 获取 mirai-api-http 插件的版本
		string mah_version = bot.GetMiraiApiHttpVersion();
		// 获取 mirai-cpp 的版本
		string mc_version = bot.GetMiraiCppVersion();
		cout << "! mirai-api-http 的版本: " << mah_version
			<< "; 当mirai-cpp 的版本: " << mc_version << "; " << endl;
		if (mah_version != mc_version)
		{
			cout << "! 警告: 你的 mirai-api-http 插件的版本与 mirai-cpp 的版本不同，可能存在兼容性问题。" << endl;
		}
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}

	// 自动重试地进行 Verify
	while (true)
	{
		try
		{
			bot.Verify("VerifyKeyASDEWQ", 1589588851_qq);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "Bot Working..." << endl;

	bot.On<NudgeEvent>(
		[&](NudgeEvent e)
		{
			// 注意: 使用 SendNudge 发送的戳一戳，也会触发该事件,
			// 注意: 因此必须过滤掉来自bot自己的戳一戳事件，不然会导致死循环
			if (e.FromId.ToInt64() == bot.GetBotQQ().ToInt64()) return;

			cout << e.FromId.ToInt64() << " " << e.Action << " " << e.Target << " " << e.Suffix << endl;
			// 如果别人戳机器人，那么就让机器人戳回去
			if (e.Target.ToInt64() != bot.GetBotQQ().ToInt64()) return;
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


	// 记录轮询事件时的错误
	bot.EventLoop([](const char* errMsg)
		{
			cout << "获取事件时出错: " << errMsg << endl;
		});

	return 0;
}