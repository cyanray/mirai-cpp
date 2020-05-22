#pragma once
#ifndef mirai_cpp_events_event_interface_hpp_H_
#define mirai_cpp_events_event_interface_hpp_H_
#include "mirai_event.hpp"
#include "defs/serializable.hpp"
#include "defs/qq_types.hpp"
namespace Cyan
{
	class EventBase : public Serializable
	{
	public:
		EventBase() : bot_(nullptr) {}

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::Default;
		}
		virtual void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}
		virtual MiraiBot& GetMiraiBot()
		{
			return *bot_;
		}
		virtual ~EventBase() {}
	protected:
		MiraiBot* bot_;
	};



} // namespace Cyan

#endif // !mirai_cpp_events_event_interface_hpp_H_