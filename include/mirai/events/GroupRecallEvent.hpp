#pragma once
#ifndef mirai_cpp_events_GroupRecallEvent_hpp_H_
#define mirai_cpp_events_GroupRecallEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "event_interface.hpp"
#include "mirai/defs/group_member.hpp"
#include "mirai/defs/qq_types.hpp"

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
		Cyan::MessageId_t MessageId;
		Group_t Group;
		GroupMember Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::GroupRecallEvent;
		}

		bool OperatorIsBot() const
		{
			return operator_is_null_;
		}
		
		virtual bool Set(const json& j) override
		{
			this->Time = j["time"].get<int64_t>();
			this->AuthorQQ = (QQ_t)j["authorId"].get<int64_t>();
			this->MessageId = j["messageId"].get<int64_t>();
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
			j["type"] = "GroupRecallEvent";
			j["time"] = this->Time;
			j["authorId"] = (int64_t)this->AuthorQQ;
			j["messageId"] = this->MessageId;
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

#endif