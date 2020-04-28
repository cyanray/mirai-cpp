#pragma once
#ifndef mirai_cpp_events_friend_recall_event_hpp_H_
#define mirai_cpp_events_friend_recall_event_hpp_H_

#include <nlohmann/json.hpp>
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/group_member.hpp"

namespace Cyan
{
	// 好友消息撤回事件
	class FriendRecallEvent : public Serializable
	{
	public:
		int64_t Time = 0;
		QQ_t AuthorQQ;
		Cyan::MessageId MessageId;
		QQ_t Operator;

		FriendRecallEvent() = default;
		FriendRecallEvent(const FriendRecallEvent& gm)
		{
			Time = gm.Time;
			AuthorQQ = gm.AuthorQQ;
			MessageId = gm.MessageId;
			Operator = gm.Operator;
		}
		FriendRecallEvent& operator=(const FriendRecallEvent& t)
		{
			FriendRecallEvent tmp(t);
			std::swap(this->Time, tmp.Time);
			std::swap(this->AuthorQQ, tmp.AuthorQQ);
			std::swap(this->MessageId, tmp.MessageId);
			std::swap(this->Operator, tmp.Operator);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		virtual ~FriendRecallEvent() = default;
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

	private:
		MiraiBot* bot_;
	};

}

#endif // !mirai_cpp_events_friend_recall_event_hpp_H_