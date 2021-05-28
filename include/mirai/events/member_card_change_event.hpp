#pragma once
#ifndef mirai_cpp_events_member_card_change_hpp_H_
#define mirai_cpp_events_member_card_change_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "event_interface.hpp"
#include "mirai/defs/group.hpp"
#include "mirai/defs/group_member.hpp"

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
		GroupMember Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberCardChangeEvent;
		}

		bool OperatorIsBot() const
		{
			return operator_is_null_;
		}

		virtual bool Set(const json& j) override
		{
			this->OriginName = j["origin"].get<string>();
			this->CurrentName = j["current"].get<string>();
			this->Member.Set(j["member"]);
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
			j["type"] = "MemberCardChangeEvent";
			j["origin"] = this->OriginName;
			j["current"] = this->CurrentName;
			j["member"] = this->Member.ToJson();
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

#endif // !mirai_cpp_events_member_card_change_hpp_H_