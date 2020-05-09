#pragma once
#ifndef mirai_cpp_events_events_hpp_H_
#define mirai_cpp_events_events_hpp_H_

// 消息事件
#include "friend_message.hpp"
#include "group_message.hpp"
#include "temp_message.hpp"
// 机器人相关事件
#include "bot_online_event.hpp"
#include "bot_offline_active.hpp"
#include "bot_offline_force.hpp"
#include "bot_offline_dropped.hpp"
#include "bot_relogin_event.hpp"
// 好友相关事件
#include "new_friend_event.hpp"
#include "friend_recall_event.hpp"
// 群组相关事件
#include "group_recall_event.hpp"
#include "join_request_event.hpp"
#include "member_join_event.hpp"
#include "member_leave_kick.hpp"
#include "member_leave_quit.hpp"
#include "member_mute_event.hpp"
#include "member_unmute_event.hpp"
#include "bot_mute_event.hpp"
#include "bot_unmute_event.hpp"
#include "bot_join_group.hpp"
// 一些定义
#include "event_processer.hpp"
#include "events_name.hpp"
#include "event_interface.hpp"

#endif // !mirai_cpp_events_events_hpp_H_
