#pragma once
#ifndef mirai_cpp_events_event_interface_hpp_H_
#define mirai_cpp_events_event_interface_hpp_H_
#include "MiraiEvent.hpp"
#include "mirai/defs/ISerializable.hpp"
#include "mirai/defs/QQType.hpp"
namespace Cyan
{
	class EventBase : public ISerializable
	{
	public:
		EventBase() : bot_(nullptr) {}

		EventBase(const EventBase& eb)
		{
			this->bot_ = eb.bot_;
		}

		EventBase(EventBase&& eb) noexcept: bot_(nullptr)
		{
			std::swap(this->bot_, eb.bot_);
		}

		EventBase& operator=(const EventBase& eb)
		{
			EventBase tmp(eb);
			std::swap(this->bot_, tmp.bot_);
			return *this;
		}
		
		virtual bool Set(const json& json) override
		{
			return true;
		}

		virtual json ToJson() const override
		{
			return json::object();
		}

		virtual ~EventBase() = default;
		
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
		
	protected:
		MiraiBot* bot_;
	};



} // namespace Cyan

#endif // !mirai_cpp_events_event_interface_hpp_H_