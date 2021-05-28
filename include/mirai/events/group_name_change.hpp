#pragma once
#ifndef mirai_cpp_events_group_name_change_hpp_H_
#define mirai_cpp_events_group_name_change_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "event_interface.hpp"
#include "mirai/defs/group.hpp"
#include "mirai/defs/group_member.hpp"

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
		GroupMember Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::GroupNameChangeEvent;
		}

		bool OperatorIsBot() const
		{
			return operator_is_null_;
		}

		virtual bool Set(const json& j) override
		{
			this->OriginName = j["origin"].get<string>();
			this->CurrentName = j["current"].get<string>();
			this->Group.Set(j["group"]);
			if (!j["operator"].is_null())
			{
				this->Operator.Set(j["operator"]);
				this->operator_is_null_ = false;
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
			if (!operator_is_null_)
				j["operator"] = this->Operator.ToJson();
			else
				j["operator"] = nullptr;
			return j;
		}

	private:
		bool operator_is_null_ = true;
	};

}

#endif // !mirai_cpp_events_group_name_change_hpp_H_