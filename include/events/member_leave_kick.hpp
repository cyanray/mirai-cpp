#pragma once
#ifndef mirai_cpp_events_member_leave_kick_event_hpp_H_
#define mirai_cpp_events_member_leave_kick_event_hpp_H_

#include <nlohmann/json.hpp>
#include "event_interface.hpp"
#include "defs/group_member.hpp"

namespace Cyan
{
	// 群成员被踢出群事件
	class MemberLeaveEventKick : public EventBase
	{
	public:
		GroupMember_t Member;
		GroupMember_t Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberLeaveEventKick;
		}

		bool OperatorIsBot() const
		{
			return operator_is_null_;
		}
		
		virtual bool Set(const json& j) override
		{
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
			j["type"] = "MemberLeaveEventKick";
			j["member"] = this->Member.ToJson();
			if (!operator_is_null_)
				j["operator"] = this->Operator.ToJson();
			else
				j["operator"] = nullptr;
			return j;
		}
	private:
		bool operator_is_null_ = true;
	};

}

#endif // !mirai_cpp_events_member_leave_kick_event_hpp_H_