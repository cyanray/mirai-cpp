#pragma once
#ifndef mirai_cpp_events_GroupEntranceAnnouncementChangeEvent_hpp_H_
#define mirai_cpp_events_GroupEntranceAnnouncementChangeEvent_hpp_H_

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
	 * \brief 某群入群公告改变
	 */
	class GroupEntranceAnnouncementChangeEvent : public EventBase
	{
	public:
		string Origin;
		string Current;
		Group_t Group;
		std::optional<GroupMember> Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::GroupEntranceAnnouncementChangeEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->Origin = j["origin"].get<string>();
			this->Current = j["current"].get<string>();
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
			j["type"] = "GroupEntranceAnnouncementChangeEvent";
			j["origin"] = this->Origin;
			j["current"] = this->Current;
			j["group"] = this->Group.ToJson();
			j["operator"] = (Operator != std::nullopt) ? this->Operator->ToJson() : json(nullptr);
			return j;
		}
	};

}

#endif