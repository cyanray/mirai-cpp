#pragma once
#ifndef mirai_cpp_events_BotGroupPermissionChangeEvent_hpp_H_
#define mirai_cpp_events_BotGroupPermissionChangeEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "EventBase.hpp"
#include "mirai/defs/Group.hpp"

namespace Cyan
{

	/**
	 * \brief bot 群权限改变事件
	 */
	class BotGroupPermissionChangeEvent : public EventBase
	{
	public:
		GroupPermission Origin = GroupPermission::Member;
		GroupPermission Current = GroupPermission::Member;
		Group_t Group;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::BotGroupPermissionChangeEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->Group.Set(j["group"]);
			this->Origin = GroupPermissionStr(j["origin"].get<string>());
			this->Current = GroupPermissionStr(j["current"].get<string>());
			return true;
		}
		
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "BotGroupPermissionChangeEvent";
			j["group"] = this->Group.ToJson();
			j["origin"] = GroupPermissionStr(Origin);
			j["current"] = GroupPermissionStr(Current);
			return j;
		}
	};

}

#endif