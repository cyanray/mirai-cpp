#pragma once
#ifndef mirai_cpp_events_MemberSpecialTitleChangeEvent_hpp_H_
#define mirai_cpp_events_MemberSpecialTitleChangeEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "EventBase.hpp"
#include "mirai/defs/Group.hpp"
#include "mirai/defs/GroupMember.hpp"

namespace Cyan
{
	/**
	 * \brief 群头衔改动事件
	 */
	class MemberSpecialTitleChangeEvent : public EventBase
	{
	public:
		string OriginName;
		string CurrentName;
		GroupMember Member;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberSpecialTitleChangeEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->OriginName = j["origin"].get<string>();
			this->CurrentName = j["current"].get<string>();
			this->Member.Set(j["member"]);
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "MemberSpecialTitleChangeEvent";
			j["origin"] = this->OriginName;
			j["current"] = this->CurrentName;
			j["member"] = this->Member.ToJson();
			return j;
		}

	};

}

#endif