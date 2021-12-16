#pragma once
#ifndef mirai_cpp_events_FriendNickChangedEvent_hpp_H_
#define mirai_cpp_events_FriendNickChangedEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "mirai/defs/MessageChain.hpp"
#include "mirai/defs/Friend.hpp"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief 好友昵称改变事件
	 */
	class FriendNickChangedEvent : public EventBase
	{
	public:
		Friend_t Friend;
		string From;
		string To;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::FriendNickChangedEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->Friend.Set(j["friend"]);
			this->From = j["from"].get<string>();
			this->To = j["to"].get<string>();
			return true;
		}

		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "FriendNickChangedEvent";
			j["friend"] = this->Friend.ToJson();
			j["from"] = this->From;
			j["to"] = this->To;
			return j;
		}
	};

}

#endif