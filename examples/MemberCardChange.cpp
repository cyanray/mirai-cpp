#include <iostream>
#include <ctime>
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
			bot.Verify("INITKEY7A3O1a9v", 1589588851_qq);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "成功登录 bot。" << endl;


	// 由于服务器并不会告知名片变动, 此事件只能由 mirai 在发现变动时才广播. 不要依赖于这个事件.
	bot.On<MemberCardChangeEvent>(
		[&](MemberCardChangeEvent e)
		{
			cout << e.Member.MemberName << "(" << e.Member.QQ.ToInt64() << ") 的群名片被修改了" << endl;
			cout << e.OriginName << ", " << e.CurrentName << endl;
		});

	bot.EventLoop();
	return 0;
}