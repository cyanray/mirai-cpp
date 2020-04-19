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
		if (miraiEvent == "MemberJoinEvent") return MiraiEvent::MemberJoinEvent;
		if (miraiEvent == "BotMuteEvent") return MiraiEvent::BotMuteEvent;
		if (miraiEvent == "BotUnmuteEvent") return MiraiEvent::BotUnmuteEvent;
		if (miraiEvent == "MemberMuteEvent") return MiraiEvent::MemberMuteEvent;
		if (miraiEvent == "MemberUnmuteEvent") return MiraiEvent::MemberUnmuteEvent;
		if (miraiEvent == "MemberLeaveEventKick") return MiraiEvent::MemberLeaveEventKick;
		if (miraiEvent == "MemberLeaveEventQuit") return MiraiEvent::MemberLeaveEventQuit;
		if (miraiEvent == "GroupRecallEvent") return MiraiEvent::GroupRecallEvent;
		if (miraiEvent == "FriendRecallEvent") return MiraiEvent::FriendRecallEvent;
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
		case Cyan::MiraiEvent::MemberJoinEvent:
			result = "MemberJoinEvent";
			break;
		case Cyan::MiraiEvent::BotMuteEvent:
			result = "BotMuteEvent";
			break;
		case Cyan::MiraiEvent::BotUnmuteEvent:
			result = "BotUnmuteEvent";
			break;
		case Cyan::MiraiEvent::MemberMuteEvent:
			result = "MemberMuteEvent";
			break;
		case Cyan::MiraiEvent::MemberUnmuteEvent:
			result = "MemberUnmuteEvent";
			break;
		case Cyan::MiraiEvent::MemberLeaveEventKick:
			result = "MemberLeaveEventKick";
			break;
		case Cyan::MiraiEvent::MemberLeaveEventQuit:
			result = "MemberLeaveEventQuit";
			break;
		case Cyan::MiraiEvent::GroupRecallEvent:
			result = "GroupRecallEvent";
			break;
		case Cyan::MiraiEvent::FriendRecallEvent:
			result = "FriendRecallEvent";
			break;
		default:
			result = "Default";
			break;
		}
		return result;
	}

	template<typename T>
	MiraiEvent GetEventType()
	{
		// 故意返回字符串，导致编译期出错
		return "错误:你使用了没有定义的Event类型";
	}

	template<>
	MiraiEvent GetEventType<FriendMessage>()
	{
		return MiraiEvent::FriendMessage;
	}

	template<>
	MiraiEvent GetEventType<GroupMessage>()
	{
		return MiraiEvent::GroupMessage;
	}

	template<>
	MiraiEvent GetEventType<TempMessage>()
	{
		return MiraiEvent::TempMessage;
	}

	template<>
	MiraiEvent GetEventType<NewFriendRequestEvent>()
	{
		return MiraiEvent::NewFriendRequestEvent;
	}

	template<>
	MiraiEvent GetEventType<MemberJoinRequestEvent>()
	{
		return MiraiEvent::MemberJoinRequestEvent;
	}

	template<>
	MiraiEvent GetEventType<MemberJoinEvent>()
	{
		return MiraiEvent::MemberJoinEvent;
	}

	template<>
	MiraiEvent GetEventType<BotMuteEvent>()
	{
		return MiraiEvent::BotMuteEvent;
	}

	template<>
	MiraiEvent GetEventType<BotUnmuteEvent>()
	{
		return MiraiEvent::BotUnmuteEvent;
	}

	template<>
	MiraiEvent GetEventType<MemberMuteEvent>()
	{
		return MiraiEvent::MemberMuteEvent;
	}

	template<>
	MiraiEvent GetEventType<MemberUnmuteEvent>()
	{
		return MiraiEvent::MemberUnmuteEvent;
	}

	template<>
	MiraiEvent GetEventType<MemberLeaveEventKick>()
	{
		return MiraiEvent::MemberLeaveEventKick;
	}

	template<>
	MiraiEvent GetEventType<MemberLeaveEventQuit>()
	{
		return MiraiEvent::MemberLeaveEventQuit;
	}

	template<>
	MiraiEvent GetEventType<GroupRecallEvent>()
	{
		return MiraiEvent::GroupRecallEvent;
	}

	template<>
	MiraiEvent GetEventType<FriendRecallEvent>()
	{
		return MiraiEvent::FriendRecallEvent;
	}

}


#endif // !mirai_cpp_events_events_name_hpp_H_
