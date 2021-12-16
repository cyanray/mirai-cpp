#pragma once
#ifndef mirai_cpp_events_FriendRecallEvent_hpp_H_
#define mirai_cpp_events_FriendRecallEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief 好友消息撤回事件
	 */
	class FriendRecallEvent : public EventBase
	{
	public:
		int64_t Time = 0;
		QQ_t AuthorQQ;
		Cyan::MessageId_t MessageId;
		QQ_t Operator;


		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::FriendRecallEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->Time = j["time"].get<int64_t>();
			this->AuthorQQ = (QQ_t)j["authorId"].get<int64_t>();
			this->MessageId = j["messageId"].get<int64_t>();
			this->Operator = (QQ_t)j["operator"].get<int64_t>();
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "GroupRecallEvent";
			j["time"] = this->Time;
			j["authorId"] = (int64_t)this->AuthorQQ;
			j["messageId"] = this->MessageId;
			j["operator"] = (int64_t)this->Operator;
			return j;
		}
	};

}

#endif