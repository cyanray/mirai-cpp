#pragma once
#ifndef mirai_cpp_events_events_name_hpp_H_
#define mirai_cpp_events_events_name_hpp_H_

#include <functional>
#include "friend_message.hpp"
#include "group_message.hpp"
#include "temp_message.hpp"
#include "new_friend_event.hpp"
#include "member_join_event.hpp"

namespace Cyan
{

	// Mirai 事件类型
	enum class MiraiEvent
	{
		Default,				// 未知
		FriendMessage,			// 好友消息
		GroupMessage,			// 群组消息
		TempMessage,			// 临时消息
		GroupRecallEvent,		// 群消息撤回
		FriendRecallEvent,		// 好友消息撤回
		BotMuteEvent,			// Bot被禁言
		BotUnmuteEvent,			// Bot被取消禁言
		BotJoinGroupEvent,		// Bot加入了一个新群
		GroupNameChangeEvent,	// 某个群名称改变
		GroupMuteAllEvent,		// 群全员禁言
		MemberJoinEvent,		// 新人入群事件
		MemberLeaveEventKick,	// 成员被踢出群(该成员不是Bot)
		MemberLeaveEventQuit,	// 成员主动离开群组
		MemberMuteEvent,		// 群成员被禁言(该成员不是Bot)
		MemberUnmuteEvent,		// 群成员被取消禁言(该成员不是Bot)
		NewFriendRequestEvent,	// 添加好友申请
		MemberJoinRequestEvent	// 用户入群申请
	};

	inline MiraiEvent MiraiEventStr(const string& miraiEvent)
	{
		if (miraiEvent == "FriendMessage") return MiraiEvent::FriendMessage;
		if (miraiEvent == "GroupMessage") return MiraiEvent::GroupMessage;
		if (miraiEvent == "TempMessage") return MiraiEvent::TempMessage;
		if (miraiEvent == "NewFriendRequestEvent") return MiraiEvent::NewFriendRequestEvent;
		if (miraiEvent == "MemberJoinRequestEvent") return MiraiEvent::MemberJoinRequestEvent;
		return MiraiEvent::Default;
	}

	inline string MiraiEventStr(MiraiEvent miraiEvent)
	{
		string result;
		switch (miraiEvent)
		{
		case Cyan::MiraiEvent::FriendMessage:
			result = "FriendMessage";
			break;
		case Cyan::MiraiEvent::GroupMessage:
			result = "GroupMessage";
			break;
		case Cyan::MiraiEvent::TempMessage:
			result = "TempMessage";
			break;
		case Cyan::MiraiEvent::NewFriendRequestEvent:
			result = "NewFriendRequestEvent";
			break;
		case Cyan::MiraiEvent::MemberJoinRequestEvent:
			result = "MemberJoinRequestEvent";
			break;
		default:
			result = "Default";
			break;
		}
		return result;
	}

	template<typename T>
	MiraiEvent GetEventName()
	{
		// 故意返回字符串，导致编译期出错
		return "错误:你使用了没有定义的Event类型";
	}

	template<>
	MiraiEvent GetEventName<FriendMessage>()
	{
		return MiraiEvent::FriendMessage;
	}

	template<>
	MiraiEvent GetEventName<GroupMessage>()
	{
		return MiraiEvent::GroupMessage;
	}

	template<>
	MiraiEvent GetEventName<TempMessage>()
	{
		return MiraiEvent::TempMessage;
	}

	template<>
	MiraiEvent GetEventName<NewFriendRequestEvent>()
	{
		return MiraiEvent::NewFriendRequestEvent;
	}

	template<>
	MiraiEvent GetEventName<MemberJoinRequestEvent>()
	{
		return MiraiEvent::MemberJoinRequestEvent;
	}

}


#endif // !mirai_cpp_events_events_name_hpp_H_
