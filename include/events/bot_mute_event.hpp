#pragma once
#ifndef mirai_cpp_events_bot_mute_event_hpp_H_
#define mirai_cpp_events_bot_mute_event_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/message_chain.hpp"
#include "defs/group_member.hpp"

namespace Cyan
{
	// bot 被禁言事件
	class BotMuteEvent : public Serializable
	{
	public:
		int DurationSeconds = 0;
		GroupMember_t Operator;

		BotMuteEvent() = default;
		BotMuteEvent(const BotMuteEvent& gm)
		{
			DurationSeconds = gm.DurationSeconds;
			Operator = gm.Operator;
		}
		BotMuteEvent& operator=(const BotMuteEvent& t)
		{
			BotMuteEvent tmp(t);
			std::swap(this->DurationSeconds, tmp.DurationSeconds);
			std::swap(this->Operator, tmp.Operator);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~BotMuteEvent() = default;
		virtual bool Set(const json& j) override
		{
			this->DurationSeconds = j["durationSeconds"].get<int>();
			this->Operator.Set(j["operator"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotMuteEvent";
			j["durationSeconds"] = this->DurationSeconds;
			j["operator"] = this->Operator.ToJson();
			return j;
		}

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_bot_mute_event_hpp_H_