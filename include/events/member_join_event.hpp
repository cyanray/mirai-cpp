#pragma once
#ifndef mirai_cpp_events_member_join_event_hpp_H_
#define mirai_cpp_events_member_join_event_hpp_H_

#include <nlohmann/json.hpp>
#include "CURLWrapper.h"
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/message_chain.hpp"
#include "defs/group_member.hpp"

namespace Cyan
{
	// 新成员入群事件
	class MemberJoinEvent : public Serializable
	{
	public:
		GroupMember_t NewMember;

		MemberJoinEvent() = default;
		MemberJoinEvent(const MemberJoinEvent& gm)
		{
			NewMember = gm.NewMember;
		}
		MemberJoinEvent& operator=(const MemberJoinEvent& t)
		{
			MemberJoinEvent tmp(t);
			std::swap(this->NewMember, tmp.NewMember);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~MemberJoinEvent() = default;
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