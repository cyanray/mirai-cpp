#pragma once
#ifndef mirai_cpp_events_NewFriendRequestEvent_hpp_H_
#define mirai_cpp_events_NewFriendRequestEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "mirai/exported.h"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief 新好友请求事件
	 */
	class EXPORTED NewFriendRequestEvent : public EventBase
	{
	public:
		int64_t EventId;
		QQ_t FromId;
		GID_t GroupId;
		string Nick;
		string Message;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::NewFriendRequestEvent;
		}

		bool Accept()
		{
			return Response(0, "");
		}

		bool Reject(const string& message = "")
		{
			return Response(1, message);
		}

		bool RejectAndBlock(const string& message = "")
		{
			return Response(2, message);
		}

		virtual bool Set(const json& j) override
		{
			this->EventId = j["eventId"].get<int64_t>();
			this->FromId = (QQ_t)j["fromId"].get<int64_t>();
			this->GroupId = (GID_t)j["groupId"].get<int64_t>();
			this->Nick = j["nick"].get<string>();
			this->Message = j["message"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "NewFriendRequestEvent";
			j["eventId"] = this->EventId;
			j["fromId"] = (int64_t)this->FromId;
			j["groupId"] = (int64_t)this->GroupId;
			j["nick"] = this->Nick;
			j["message"] = this->Message;
			return j;
		}

	private:
		bool Response(int operate, const string& message);
	};

}

#endif