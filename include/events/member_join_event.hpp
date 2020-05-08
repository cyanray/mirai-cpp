#pragma once
#ifndef mirai_cpp_events_member_join_event_hpp_H_
#define mirai_cpp_events_member_join_event_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/message_chain.hpp"
#include "defs/group_member.hpp"
#include "event_interface.hpp"

namespace Cyan
{
	// 新成员入群事件
	class MemberJoinEvent : public EventBase
	{
	public:
		GroupMember_t NewMember;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberJoinEvent;
		}


		virtual void SetMiraiBot(MiraiBot* bot) override
		{
			this->bot_ = bot;
		}

		virtual bool Set(const json& j) override
		{
			this->NewMember.Set(j["member"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "MemberJoinEvent";
			j["member"] = this->NewMember.ToJson();
			return j;
		}

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_member_join_event_hpp_H_