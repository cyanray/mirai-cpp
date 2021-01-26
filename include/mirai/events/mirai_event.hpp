#pragma once
#ifndef mirai_cpp_events_events_name_hpp_H_
#define mirai_cpp_events_events_name_hpp_H_
#include <string>

namespace Cyan
{

	// Mirai 事件类型
	enum class MiraiEvent
	{
		Default,								// 未知
		BotOnlineEvent,						// Bot 登录成功
		BotOfflineEventActive,				// Bot 主动离线
		BotOfflineEventForce,				// Bot 被挤下线
		BotOfflineEventDropped,				// Bot 被挤下线
		BotReloginEvent,						// Bot 主动重新登录
		FriendMessage,						// 好友消息
		GroupMessage,						// 群组消息
		TempMessage,							// 临时消息
		GroupRecallEvent,					// 群消息撤回
		FriendRecallEvent,					// 好友消息撤回
		BotMuteEvent,						// Bot被禁言
		BotUnmuteEvent,						// Bot被取消禁言
		BotJoinGroupEvent,					// Bot加入了一个新群
		GroupNameChangeEvent,				// 某个群名称改变
		GroupMuteAllEvent,					// 群全员禁言
		MemberJoinEvent,						// 新人入群事件
		MemberLeaveEventKick,				// 成员被踢出群(该成员不是Bot)
		MemberLeaveEventQuit,				// 成员主动离开群组
		MemberMuteEvent,						// 群成员被禁言(该成员不是Bot)
		MemberUnmuteEvent,					// 群成员被取消禁言(该成员不是Bot)
		NewFriendRequestEvent,				// 添加好友申请
		MemberJoinRequestEvent,				// 用户入群申请
		BotLeaveEventActive,					// Bot 主动离开群
		BotLeaveEventKick,					// Bot 被剔出群
		Message,								// 通用消息事件
		BotInvitedJoinGroupRequestEvent,		// Bot被邀请入群申请
		MemberCardChangeEvent,				// 群成员群名片被修改事件
		Command								// 指令事件
	};

	inline MiraiEvent MiraiEventStr(const std::string& miraiEvent)
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
		if (miraiEvent == "BotOnlineEvent") return MiraiEvent::BotOnlineEvent;
		if (miraiEvent == "BotOfflineEventActive") return MiraiEvent::BotOfflineEventActive;
		if (miraiEvent == "BotOfflineEventForce") return MiraiEvent::BotOfflineEventForce;
		if (miraiEvent == "BotOfflineEventDropped") return MiraiEvent::BotOfflineEventDropped;
		if (miraiEvent == "BotReloginEvent") return MiraiEvent::BotReloginEvent;
		if (miraiEvent == "BotJoinGroupEvent") return MiraiEvent::BotJoinGroupEvent;
		if (miraiEvent == "BotLeaveEventActive") return MiraiEvent::BotLeaveEventActive;
		if (miraiEvent == "BotLeaveEventKick") return MiraiEvent::BotLeaveEventKick;
		if (miraiEvent == "GroupNameChangeEvent") return MiraiEvent::GroupNameChangeEvent;
		if (miraiEvent == "GroupMuteAllEvent") return MiraiEvent::GroupMuteAllEvent;
		if (miraiEvent == "BotInvitedJoinGroupRequestEvent") return MiraiEvent::BotInvitedJoinGroupRequestEvent;
		if (miraiEvent == "MemberCardChangeEvent") return MiraiEvent::MemberCardChangeEvent;
		if (miraiEvent == "Command") return MiraiEvent::Command;
		return MiraiEvent::Default;
	}

	inline std::string MiraiEventStr(MiraiEvent miraiEvent)
	{
		std::string result;
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
		case Cyan::MiraiEvent::BotOnlineEvent:
			result = "BotOnlineEvent";
			break;
		case Cyan::MiraiEvent::BotOfflineEventActive:
			result = "BotOfflineEventActive";
			break;
		case Cyan::MiraiEvent::BotOfflineEventForce:
			result = "BotOfflineEventForce";
			break;
		case Cyan::MiraiEvent::BotOfflineEventDropped:
			result = "BotOfflineEventDropped";
			break;
		case Cyan::MiraiEvent::BotReloginEvent:
			result = "BotReloginEvent";
			break;
		case Cyan::MiraiEvent::BotInvitedJoinGroupRequestEvent:
			result = "BotInvitedJoinGroupRequestEvent";
			break;
		case Cyan::MiraiEvent::MemberCardChangeEvent:
			result = "MemberCardChangeEvent";
			break;
		case Cyan::MiraiEvent::Command:
			result = "Command";
			break;
		default:
			result = "Default";
			break;
		}
		return result;
	}

}

// https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
namespace std {
	template <> struct hash<Cyan::MiraiEvent> {
		size_t operator() (const Cyan::MiraiEvent& t) const { return size_t(t); }
	};
}

#endif // !mirai_cpp_events_events_name_hpp_H_
