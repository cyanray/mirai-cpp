#pragma once
#ifndef mirai_cpp_events_MemberMuteEvent_hpp_H_
#define mirai_cpp_events_MemberMuteEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "EventBase.hpp"
#include "mirai/defs/GroupMember.hpp"
#include <optional>
using std::optional;
using std::nullopt;

namespace Cyan
{
	/**
	 * \brief 群成员被禁言事件
	 */
	class MemberMuteEvent : public EventBase
	{
	public:
		int DurationSeconds = 0;
		GroupMember Member;
		std::optional<GroupMember> Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberMuteEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->DurationSeconds = j["durationSeconds"].get<int>();
			this->Member.Set(j["member"]);
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
			j["type"] = "MemberMuteEvent";
			j["durationSeconds"] = this->DurationSeconds;
			j["member"] = this->Member.ToJson();
			j["operator"] = (Operator != std::nullopt) ? this->Operator->ToJson() : json(nullptr);
			return j;
		}
	};

}

#endif