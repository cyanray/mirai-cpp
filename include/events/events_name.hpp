#pragma once
#ifndef mirai_cpp_events_events_name_hpp_H_
#define mirai_cpp_events_events_name_hpp_H_

#include <functional>
#include "events/friend_message.hpp"
#include "events/group_message.hpp"
#include "events/temp_message.hpp"

namespace Cyan
{
	template<typename T>
	string GetEventName()
	{
		return typeid(T).name();
	}

	template<>
	string GetEventName<FriendMessage>()
	{
		return "FriendMessage";
	}

	template<>
	string GetEventName<GroupMessage>()
	{
		return "GroupMessage";
	}

	template<>
	string GetEventName<TempMessage>()
	{
		return "TempMessage";
	}

}


#endif // !mirai_cpp_events_events_name_hpp_H_
