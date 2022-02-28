#pragma once
#ifndef mirai_cpp_events_MemberCardChangeEvent_hpp_H_
#define mirai_cpp_events_MemberCardChangeEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "EventBase.hpp"
#include "mirai/defs/Group.hpp"
#include "mirai/defs/GroupMember.hpp"

namespace Cyan
{
	/**
	 * \brief 群名片修改事件(由于服务器并不会告知名片变动, 此事件只能由 mirai 在发现变动时才广播. 不要依赖于这个事件.)
	 */
	class MemberCardChangeEvent : public EventBase
	{
	public:
		string OriginName;
		string CurrentName;
		GroupMember Member;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberCardChangeEvent;
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
			j["type"] = "MemberCardChangeEvent";
			j["origin"] = this->OriginName;
			j["current"] = this->CurrentName;
			j["member"] = this->Member.ToJson();
			return j;
		}

	};

}

#endif