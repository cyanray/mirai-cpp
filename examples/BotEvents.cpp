#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai/MiraiBot.hpp>
#include <mirai/events/BotJoinGroupEvent.hpp>
#include <mirai/events/BotMuteEvent.hpp>
#include <mirai/events/BotUnmuteEvent.hpp>
#include <mirai/events/BotLeaveEventKick.hpp>
#include <mirai/events/BotOnlineEvent.hpp>
#include <mirai/events/BotOfflineEventActive.hpp>
#include <mirai/events/BotOfflineEventForce.hpp>
#include <mirai/events/BotOfflineEventDropped.hpp>
#include <mirai/events/BotReloginEvent.hpp>
#include <mirai/events/BotInvitedJoinGroupRequestEvent.hpp>
using namespace std;
using namespace Cyan;

int main(int argc, char* argv[])
{
	system("chcp 65001");
	MiraiBot bot;
	SessionOptions opts = SessionOptions::FromCommandLine(argc, argv);
	bot.Connect(opts);
	cout << "Bot working..." << endl;

	bot.On<BotMuteEvent>(
		[&](BotMuteEvent e)
		{
			cout << "爷被禁言了!" << endl;
		});

	bot.On<BotUnmuteEvent>(
		[&](BotUnmuteEvent e)
		{
			bot.SendMessage(e.Operator.Group.GID,

				MessageChain().Plain("👴 出狱了!"));
		});

	bot.On<BotJoinGroupEvent>(
		[&](BotJoinGroupEvent e)
		{
			MiraiBot::SleepSeconds(5);
			bot.SendMessage(e.Group.GID, MessageChain().Plain("👴 进群了！都来欢迎 👴！"));
		});

	// 似乎没有效果
	bot.On<BotLeaveEventKick>(
		[&](BotLeaveEventKick e)
		{
			cout << "👴被踢出群了: " << int64_t(e.Group.GID) << endl;
		});

	bot.On<BotOnlineEvent>(
		[&](BotOnlineEvent e)
		{
			cout << "Bot " << (int64_t)e.QQ << " 主动登录事件" << endl;
		});

	bot.On<BotOfflineEventActive>(
		[&](BotOfflineEventActive e)
		{
			cout << "Bot " << (int64_t)e.QQ << " 主动下线事件" << endl;
		});

	bot.On<BotOfflineEventForce>(
		[&](BotOfflineEventForce e)
		{
			cout << "Bot " << (int64_t)e.QQ << " 被挤下线事件" << endl;
		});

	bot.On<BotOfflineEventDropped>(
		[&](BotOfflineEventDropped e)
		{
			cout << "Bot " << (int64_t)e.QQ << " 因网络原掉线事件" << endl;
		});

	bot.On<BotReloginEvent>(
		[&](BotReloginEvent e)
		{
			cout << "Bot " << (int64_t)e.QQ << " 重新登录事件" << endl;
		});

	bot.On<BotInvitedJoinGroupRequestEvent>(
		[&](BotInvitedJoinGroupRequestEvent e)
		{
			cout << "邀请你入群：" << e.GroupName << ", " << e.Message << endl;
			e.Accept();
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