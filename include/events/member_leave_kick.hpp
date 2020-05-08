#pragma once
#ifndef mirai_cpp_events_member_leave_kick_event_hpp_H_
#define mirai_cpp_events_member_leave_kick_event_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/group_member.hpp"
#include "event_interface.hpp"

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

		virtual void SetMiraiBot(MiraiBot* bot) override
		{
			this->bot_ = bot;
		}

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