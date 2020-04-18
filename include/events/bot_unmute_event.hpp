#pragma once
#ifndef mirai_cpp_events_bot_unmute_event_hpp_H_
#define mirai_cpp_events_bot_unmute_event_hpp_H_

#include <nlohmann/json.hpp>
#include "CURLWrapper.h"
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/message_chain.hpp"
#include "defs/group_member.hpp"

namespace Cyan
{
	// bot 被解除禁言事件
	class BotUnmuteEvent : public Serializable
	{
	public:
		GroupMember_t Operator;

		BotUnmuteEvent() = default;
		BotUnmuteEvent(const BotUnmuteEvent& gm)
		{
			Operator = gm.Operator;
		}
		BotUnmuteEvent& operator=(const BotUnmuteEvent& t)
		{
			BotUnmuteEvent tmp(t);
			std::swap(this->Operator, tmp.Operator);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~BotUnmuteEvent() = default;
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