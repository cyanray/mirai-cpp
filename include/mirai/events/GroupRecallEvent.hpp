#pragma once
#ifndef mirai_cpp_events_GroupRecallEvent_hpp_H_
#define mirai_cpp_events_GroupRecallEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "EventBase.hpp"
#include "mirai/defs/GroupMember.hpp"
#include "mirai/defs/QQType.hpp"
#include <optional>
using std::optional;
using std::nullopt;

namespace Cyan
{
	/**
	 * \brief 群成员消息撤回事件
	 */
	class GroupRecallEvent : public EventBase
	{
	public:
		int64_t Time = 0;
		QQ_t AuthorQQ;
		MessageId_t MessageId;
		Group_t Group;
		std::optional<GroupMember> Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::GroupRecallEvent;
		}
		
		virtual bool Set(const json& j) override
		{
			this->Time = j["time"].get<int64_t>();
			this->AuthorQQ = (QQ_t)j["authorId"].get<int64_t>();
			this->MessageId = j["messageId"].get<int64_t>();
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
			j["type"] = "GroupRecallEvent";
			j["time"] = this->Time;
			j["authorId"] = (int64_t)this->AuthorQQ;
			j["messageId"] = this->MessageId;
			j["group"] = this->Group.ToJson();
			j["operator"] = (Operator != std::nullopt) ? this->Operator->ToJson() : json(nullptr);
			return j;
		}
	};

}

#endif