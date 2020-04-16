#pragma once
#ifndef mirai_cpp_events_event_processer_hpp_H_
#define mirai_cpp_events_event_processer_hpp_H_

#include <functional>
#include "friend_message.hpp"
#include "group_message.hpp"
#include "temp_message.hpp"

namespace Cyan
{
	typedef std::function<void(FriendMessage)> FriendMessageProcesser;
	typedef std::function<void(GroupMessage)> GroupMessageProcesser;
	typedef std::function<void(TempMessage)> TempMessageProcesser;
}


#endif // !mirai_cpp_events_event_processer_hpp_H_
