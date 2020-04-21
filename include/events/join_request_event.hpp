#pragma once
#ifndef mirai_cpp_events_join_request_event_hpp_H_
#define mirai_cpp_events_join_request_event_hpp_H_

#include <nlohmann/json.hpp>
#include "CURLWrapper.h"
#include "defs/qq_types.hpp"
#include "defs/serializable.hpp"
#include "defs/message_chain.hpp"
#include "defs/friend.hpp"

namespace Cyan
{
	// 新成员入群请求
	class MemberJoinRequestEvent : public Serializable
	{
	public:
		int64_t EventId;
		QQ_t FromId;
		GID_t GroupId;
		string GroupName;
		string Nick;
		string Message;

		MemberJoinRequestEvent() = default;
		MemberJoinRequestEvent(const MemberJoinRequestEvent& gm)
		{
			EventId = gm.EventId;
			FromId = gm.FromId;
			GroupId = gm.GroupId;
			Nick = gm.Nick;
			GroupName = gm.GroupName;
			Message = gm.Message;
			bot_ = gm.bot_;
		}
		MemberJoinRequestEvent& operator=(const MemberJoinRequestEvent& t)
		{
			MemberJoinRequestEvent tmp(t);
			std::swap(this->EventId, tmp.EventId);
			std::swap(this->FromId, tmp.FromId);
			std::swap(this->GroupId, tmp.GroupId);
			std::swap(this->GroupName, tmp.GroupName);
			std::swap(this->Nick, tmp.Nick);
			std::swap(this->Message, tmp.Message);
			std::swap(this->bot_, tmp.bot_);
			return *this;
		}

		void SetMiraiBot(MiraiBot* bot)
		{
			this->bot_ = bot;
		}

		bool Accept()
		{
			return Respose(0, "");
		}

		bool Reject(const string& message = "")
		{
			return Respose(1, message);
		}

		bool Ignore(const string& message = "")
		{
			return Respose(2, message);
		}

		bool RejectAndBlock(const string& message = "")
		{
			return Respose(3, message);
		}

		bool IgnoreAndBlock(const string& message = "")
		{
			return Respose(4, message);
		}


		virtual ~MemberJoinRequestEvent() = default;
		virtual bool Set(const json& j) override
		{
			this->EventId = j["eventId"].get<int64_t>();
			this->FromId = QQ_t(j["fromId"].get<int64_t>());
			this->GroupName = j["groupName"].get<string>();
			this->GroupId = GID_t(j["groupId"].get<int64_t>());
			this->Nick = j["nick"].get<string>();
			this->Message = j["message"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "MemberJoinRequestEvent";
			j["eventId"] = this->EventId;
			j["fromId"] = (int64_t)this->FromId;
			j["groupId"] = (int64_t)this->GroupId;
			j["groupName"] = this->GroupName;
			j["nick"] = this->Nick;
			j["message"] = this->Message;
			return j;
		}

	private:
		MiraiBot* bot_;
		bool Respose(int operate, const string& message);
	};

}

#endif // !mirai_cpp_events_join_request_event_hpp_H_