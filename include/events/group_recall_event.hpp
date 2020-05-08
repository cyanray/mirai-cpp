#pragma once
#ifndef mirai_cpp_events_group_recall_event_hpp_H_
#define mirai_cpp_events_group_recall_event_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/group_member.hpp"

namespace Cyan
{
	// 群成员消息撤回事件
	class GroupRecallEvent : public EventBase
	{
	public:
		int64_t Time = 0;
		QQ_t AuthorQQ;
		Cyan::MessageId MessageId;
		Group_t Group;
		GroupMember_t Operator;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::GroupRecallEvent;
		}

		virtual void SetMiraiBot(MiraiBot* bot) override
		{
			this->bot_ = bot;
		}

		virtual bool Set(const json& j) override
		{
			this->Time = j["time"].get<int64_t>();
			this->AuthorQQ = (QQ_t)j["authorId"].get<int64_t>();
			this->MessageId = j["messageId"].get<int64_t>();
			this->Group.Set(j["group"]);
			this->Operator.Set(j["operator"]);
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
			j["operator"] = this->Operator.ToJson();
			return j;
		}

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_group_recall_event_hpp_H_