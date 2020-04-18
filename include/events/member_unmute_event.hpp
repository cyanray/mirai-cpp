#pragma once
#ifndef mirai_cpp_events_member_unmute_event_hpp_H_
#define mirai_cpp_events_member_unmute_event_hpp_H_

#include <nlohmann/json.hpp>
#include "CURLWrapper.h"
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/group_member.hpp"

namespace Cyan
{
	// 群成员被取消禁言事件
	class MemberUnmuteEvent : public Serializable
	{
	public:
		GroupMember_t Member;
		GroupMember_t Operator;

		MemberUnmuteEvent() = default;
		MemberUnmuteEvent(const MemberUnmuteEvent& gm)
		{
			Member = gm.Member;
			Operator = gm.Operator;
		}
		MemberUnmuteEvent& operator=(const MemberUnmuteEvent& t)
		{
			MemberUnmuteEvent tmp(t);
			std::swap(this->Member, tmp.Member);
			std::swap(this->Operator, tmp.Operator);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~MemberUnmuteEvent() = default;
		virtual bool Set(const json& j) override
		{
			this->Member.Set(j["member"]);
			this->Operator.Set(j["operator"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "MemberUnmuteEvent";
			j["member"] = this->Member.ToJson();
			j["operator"] = this->Operator.ToJson();
			return j;
		}

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_member_unmute_event_hpp_H_