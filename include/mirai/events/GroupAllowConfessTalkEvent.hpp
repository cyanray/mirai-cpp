#pragma once
#ifndef mirai_cpp_events_GroupAllowConfessTalkEvent_hpp_H_
#define mirai_cpp_events_GroupAllowConfessTalkEvent_hpp_H_

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
	 * \brief 群坦白说权限改变
	 */
	class GroupAllowConfessTalkEvent : public EventBase
	{
	public:
		bool Origin;
		bool Current;
		Group_t Group;
		bool IsByBot;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::GroupAllowConfessTalkEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->Origin = j["origin"].get<bool>();
			this->Current = j["current"].get<bool>();
			this->Group.Set(j["group"]);
			this->IsByBot = j["isByBot"].get<bool>();
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "GroupAllowConfessTalkEvent";
			j["origin"] = this->Origin;
			j["current"] = this->Current;
			j["group"] = this->Group.ToJson();
			j["isByBot"] = this->IsByBot;
			return j;
		}
	};

}

#endif