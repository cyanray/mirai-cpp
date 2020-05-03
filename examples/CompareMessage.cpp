#include <iostream>
// 使用静态库必须要在引入 mirai.h 前定义这个宏
#define MIRAICPP_STATICLIB
#include <mirai.h>

int main()
{
	using namespace std;
	using namespace Cyan;
	system("chcp 65001");

	MessageChain mc1 = MessageChain().Plain("##").At(123456_qq).Face(14).Plain("Hello");
	MessageChain mc2 = MessageChain().Plain("##").At(1234569_qq).Face(14).Plain("Hello");

	if (mc1 == mc2)
		cout << "mc1 == mc2" << endl;
	else
		cout << "mc1 != mc2" << endl;

	if (mc1 != mc2)
		cout << "mc1 != mc2" << endl;
	else
		cout << "mc1 == mc2" << endl;

	return 0;
}