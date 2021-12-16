#pragma once
#ifndef mirai_cpp_events_MemberJoinEvent_hpp_H_
#define mirai_cpp_events_MemberJoinEvent_hpp_H_

#include <optional>
#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/GroupMember.hpp"
#include "EventBase.hpp"
using std::optional;

namespace Cyan
{

	/**
	 * \brief 新成员入群事件
	 */
	class MemberJoinEvent : public EventBase
	{
	public:
		GroupMember NewMember;
		std::optional<GroupMember> Inviter;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberJoinEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->NewMember.Set(j["member"]);
			if (!j["invitor"].is_null())
			{
				GroupMember tmp;
				tmp.Set(j["invitor"]);
				this->Inviter = tmp;
			}
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "MemberJoinEvent";
			j["member"] = this->NewMember.ToJson();
			// Not a typo, MAH made a typo.
			j["invitor"] = (Inviter ? this->Inviter->ToJson() : json(nullptr));
			return j;
		}

	};

}

#endif