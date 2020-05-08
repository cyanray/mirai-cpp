#pragma once
#ifndef mirai_cpp_events_member_mute_event_hpp_H_
#define mirai_cpp_events_member_mute_event_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/group_member.hpp"
#include "event_interface.hpp"

namespace Cyan
{
	// 群成员被禁言事件
	class MemberMuteEvent : public EventBase
	{
	public:
		int DurationSeconds = 0;
		GroupMember_t Member;
		GroupMember_t Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberMuteEvent;
		}

		virtual void SetMiraiBot(MiraiBot* bot) override
		{
			this->bot_ = bot;
		}

		bool OperatorIsBot() const
		{
			return operator_is_null_;
		}

		virtual bool Set(const json& j) override
		{
			this->DurationSeconds = j["durationSeconds"].get<int>();
			this->Member.Set(j["member"]);
			if (!j["operator"].is_null())
			{
				this->Operator.Set(j["operator"]);
				this->operator_is_null_ = false;
			}
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "MemberMuteEvent";
			j["durationSeconds"] = this->DurationSeconds;
			j["member"] = this->Member.ToJson();
			j["operator"] = this->Operator.ToJson();
			return j;
		}

	private:
		MiraiBot* bot_;
		bool operator_is_null_ = true;
	};

}

#endif // !mirai_cpp_events_member_mute_event_hpp_H_