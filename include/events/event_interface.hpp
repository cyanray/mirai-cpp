#pragma once
#ifndef mirai_cpp_events_event_interface_hpp_H_
#define mirai_cpp_events_event_interface_hpp_H_
#include "events_name.hpp"

namespace Cyan
{
	class EventBase
	{
	public:
		EventBase() {}
		virtual static MiraiEvent GetMiraiEvent() const = 0;
		virtual SetMiraiBot(MiraiBot*) = 0;
		virtual ~EventBase() {}
	};



} // namespace Cyan

#endif // !mirai_cpp_events_event_interface_hpp_H_