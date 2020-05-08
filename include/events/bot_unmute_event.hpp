#pragma once
#ifndef mirai_cpp_events_bot_unmute_event_hpp_H_
#define mirai_cpp_events_bot_unmute_event_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/message_chain.hpp"
#include "defs/group_member.hpp"

namespace Cyan
{
	// bot 被解除禁言事件
	class BotUnmuteEvent : public EventBase
	{
	public:
		GroupMember_t Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotUnmuteEvent;
		}

		virtual void SetMiraiBot(MiraiBot* bot) override
		{
			this->bot_ = bot;
		}

		virtual bool Set(const json& j) override
		{
			this->Operator.Set(j["operator"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotUnmuteEvent";
			j["operator"] = this->Operator.ToJson();
			return j;
		}

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_bot_unmute_event_hpp_H_