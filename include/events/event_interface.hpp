#pragma once
#ifndef mirai_cpp_events_event_interface_hpp_H_
#define mirai_cpp_events_event_interface_hpp_H_
#include "events_name.hpp"
#include "defs/serializable.hpp"
namespace Cyan
{
	// Ç°ÖÃÉùÃ÷ MiraiBot Àà
	class MiraiBot;

	class EventBase : public Serializable
	{
	public:
		EventBase() {}
		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::Default;
		}
		virtual void SetMiraiBot(MiraiBot*) = 0;
		virtual ~EventBase() {}
	};



} // namespace Cyan

#endif // !mirai_cpp_events_event_interface_hpp_H_