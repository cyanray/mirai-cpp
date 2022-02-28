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
        BotOnlineEvent,							// Bot 登录成功
        BotOfflineEventActive,					// Bot 主动离线
        BotOfflineEventForce,					// Bot 被挤下线
        BotOfflineEventDropped,					// Bot 被挤下线
        BotReloginEvent,						// Bot 主动重新登录
        FriendMessage,							// 好友消息
        GroupMessage,							// 群组消息
        TempMessage,							// 临时消息
        GroupRecallEvent,						// 群消息撤回
        FriendRecallEvent,						// 好友消息撤回
        BotMuteEvent,							// Bot被禁言
        BotUnmuteEvent,							// Bot被取消禁言
        BotJoinGroupEvent,						// Bot加入了一个新群
        GroupNameChangeEvent,					// 某个群名称改变
        GroupMuteAllEvent,						// 群全员禁言
        MemberJoinEvent,						// 新人入群事件
        MemberLeaveEventKick,					// 成员被踢出群(该成员不是Bot)
        MemberLeaveEventQuit,					// 成员主动离开群组
        MemberMuteEvent,						// 群成员被禁言(该成员不是Bot)
        MemberUnmuteEvent,						// 群成员被取消禁言(该成员不是Bot)
        NewFriendRequestEvent,					// 添加好友申请
        MemberJoinRequestEvent,					// 用户入群申请
        BotLeaveEventActive,					// Bot 主动离开群
        BotLeaveEventKick,						// Bot 被剔出群
        Message,								// 通用消息事件
        BotInvitedJoinGroupRequestEvent,		// Bot被邀请入群申请
        MemberCardChangeEvent,					// 群成员群名片被修改事件
        CommandExecutedEvent,					// 指令事件
        NudgeEvent,								// 戳一戳(头像)事件
        StrangerMessage,						// 陌生人消息
        OtherClientMessage,						// 其他客户端消息
        FriendInputStatusChangedEvent,			// 好友输入状态改变事件
        FriendNickChangedEvent,					// 好友昵称改变事件
        GroupEntranceAnnouncementChangeEvent,	// 某群入群公告改变
        GroupAllowAnonymousChatEvent,			// 群匿名聊天权限改变
        GroupAllowConfessTalkEvent,				// 群坦白说权限改变
        GroupAllowMemberInviteEvent,			// 群员邀请好友加群权限改变
        MemberSpecialTitleChangeEvent,			// 群头衔改动事件
        BotGroupPermissionChangeEvent,			// bot 群权限改变事件
        MemberPermissionChangeEvent,			// 群成员权限改变事件
        MemberHonorChangeEvent,					// 群成员称号改变事件
        FriendSyncMessage,						// Bot在其他客户端发送给好友的消息
        GroupSyncMessage,						// Bot在其他客户端发送给群组的消息
        TempSyncMessage,						// Bot在其他客户端发送给群成员的临时消息
        StrangerSyncMessage						// Bot在其他客户端发送给陌生人的消息
    };

    MiraiEvent MiraiEventStr(const std::string& miraiEvent);

    std::string MiraiEventStr(MiraiEvent miraiEvent);

}

// https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
namespace std {
    template <> struct hash<Cyan::MiraiEvent> {
        size_t operator() (const Cyan::MiraiEvent& t) const { return size_t(t); }
    };
}

#endif // !mirai_cpp_events_events_name_hpp_H_
