#pragma once
#ifndef mirai_cpp_events_MemberJoinEvent_hpp_H_
#define mirai_cpp_events_MemberJoinEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/group_member.hpp"
#include "event_interface.hpp"

namespace Cyan
{

	/**
	 * \brief 新成员入群事件
	 */
	class MemberJoinEvent : public EventBase
	{
	public:
		GroupMember NewMember;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberJoinEvent;
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

	};

}

#endif