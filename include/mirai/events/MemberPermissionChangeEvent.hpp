#pragma once
#ifndef mirai_cpp_events_MemberPermissionChangeEvent_hpp_H_
#define mirai_cpp_events_MemberPermissionChangeEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "EventBase.hpp"
#include "mirai/defs/GroupMember.hpp"

namespace Cyan
{

	/**
	 * \brief 群成员权限改变事件
	 */
	class MemberPermissionChangeEvent : public EventBase
	{
	public:
		GroupPermission Origin = GroupPermission::Member;
		GroupPermission Current = GroupPermission::Member;
		GroupMember Member;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberPermissionChangeEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->Member.Set(j["member"]);
			this->Origin = GroupPermissionStr(j["origin"].get<string>());
			this->Current = GroupPermissionStr(j["current"].get<string>());
			return true;
		}
		
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "MemberPermissionChangeEvent";
			j["member"] = this->Member.ToJson();
			j["origin"] = GroupPermissionStr(Origin);
			j["current"] = GroupPermissionStr(Current);
			return j;
		}
	};

}

#endif