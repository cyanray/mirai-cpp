#pragma once
#ifndef mirai_cpp_events_member_mute_event_hpp_H_
#define mirai_cpp_events_member_mute_event_hpp_H_

#include <nlohmann/json.hpp>
#include "CURLWrapper.h"
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/group_member.hpp"

namespace Cyan
{
	// 群成员被禁言事件
	class MemberMuteEvent : public Serializable
	{
	public:
		int DurationSeconds = 0;
		GroupMember_t Member;
		GroupMember_t Operator;

		MemberMuteEvent() = default;
		MemberMuteEvent(const MemberMuteEvent& gm)
		{
			DurationSeconds = gm.DurationSeconds;
			Member = gm.Member;
			Operator = gm.Operator;
		}
		MemberMuteEvent& operator=(const MemberMuteEvent& t)
		{
			MemberMuteEvent tmp(t);
			std::swap(this->DurationSeconds, tmp.DurationSeconds);
			std::swap(this->Member, tmp.Member);
			std::swap(this->Operator, tmp.Operator);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~MemberMuteEvent() = default;
		virtual bool Set(const json& j) override
		{
			this->DurationSeconds = j["durationSeconds"].get<int>();
			this->Member.Set(j["member"]);
			this->Operator.Set(j["operator"]);
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
	};

}

#endif // !mirai_cpp_events_member_mute_event_hpp_H_