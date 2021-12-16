#pragma once
#ifndef mirai_cpp_events_FriendInputStatusChangedEvent_hpp_H_
#define mirai_cpp_events_FriendInputStatusChangedEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "mirai/defs/MessageChain.hpp"
#include "mirai/defs/Friend.hpp"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief 好友输入状态改变事件
	 */
	class FriendInputStatusChangedEvent : public EventBase
	{
	public:
		Friend_t Friend;
		bool Inputting;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::FriendInputStatusChangedEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->Inputting = j["inputting"].get<bool>();
			this->Friend.Set(j["friend"]);
			return true;
		}

		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "FriendInputStatusChangedEvent";
			j["inputting"] = this->Inputting;
			j["friend"] = this->Friend.ToJson();
			return j;
		}
	};

}

#endif