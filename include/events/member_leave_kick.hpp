#pragma once
#ifndef mirai_cpp_events_member_leave_kick_event_hpp_H_
#define mirai_cpp_events_member_leave_kick_event_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/group_member.hpp"

namespace Cyan
{
	// 群成员被踢出群事件
	class MemberLeaveEventKick : public Serializable
	{
	public:
		GroupMember_t Member;
		GroupMember_t Operator;

		MemberLeaveEventKick() = default;
		MemberLeaveEventKick(const MemberLeaveEventKick& gm)
		{
			Member = gm.Member;
			Operator = gm.Operator;
		}
		MemberLeaveEventKick& operator=(const MemberLeaveEventKick& t)
		{
			MemberLeaveEventKick tmp(t);
			std::swap(this->Member, tmp.Member);
			std::swap(this->Operator, tmp.Operator);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~MemberLeaveEventKick() = default;
		virtual bool Set(const json& j) override
		{
			this->Member.Set(j["member"]);
			this->Operator.Set(j["operator"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "MemberLeaveEventKick";
			j["member"] = this->Member.ToJson();
			j["operator"] = this->Operator.ToJson();
			return j;
		}

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_member_leave_kick_event_hpp_H_