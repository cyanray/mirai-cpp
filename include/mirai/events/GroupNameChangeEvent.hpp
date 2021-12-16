#pragma once
#ifndef mirai_cpp_events_GroupNameChangeEvent_hpp_H_
#define mirai_cpp_events_GroupNameChangeEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "EventBase.hpp"
#include "mirai/defs/Group.hpp"
#include "mirai/defs/GroupMember.hpp"
#include <optional>
using std::optional;
using std::nullopt;


namespace Cyan
{
	/**
	 * \brief 群名称被改变
	 */
	class GroupNameChangeEvent : public EventBase
	{
	public:
		string OriginName;
		string CurrentName;
		Group_t Group;
		std::optional<GroupMember> Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::GroupNameChangeEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->OriginName = j["origin"].get<string>();
			this->CurrentName = j["current"].get<string>();
			this->Group.Set(j["group"]);
			if (!j["operator"].is_null())
			{
				GroupMember tmp;
				tmp.Set(j["operator"]);
				Operator = tmp;
			}
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "GroupNameChangeEvent";
			j["origin"] = this->OriginName;
			j["current"] = this->CurrentName;
			j["group"] = this->Group.ToJson();
			j["operator"] = (Operator != std::nullopt) ? this->Operator->ToJson() : json(nullptr);
			return j;
		}
	};

}

#endif