#pragma once
#ifndef mirai_cpp__event_func_hpp_H_
#define mirai_cpp__event_func_hpp_H_

#include "mirai_bot.hpp"

namespace Cyan
{
	// 便捷函数, 必须定义到这里, 否则编译器找不到

	MessageId TempMessage::Reply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.Group.GID, Sender.QQ, mc);
	}

	MessageId FriendMessage::Reply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.QQ, mc);
	}

	MessageId GroupMessage::Reply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.Group.GID, mc);
	}

	MessageId TempMessage::QuoteReply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.Group.GID, Sender.QQ, mc, GetMessageId());
	}

	MessageId FriendMessage::QuoteReply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.QQ, mc, GetMessageId());
	}

	MessageId GroupMessage::QuoteReply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.Group.GID, mc, GetMessageId());
	}

	bool GroupMessage::Recall()
	{
		return bot_->Recall(GetMessageId());
	}

	bool NewFriendRequestEvent::Respose(int operate, const string& message)
	{
		static const string api_url = bot_->GetApiUrlPrefix() + "/resp/newFriendRequestEvent";

		json j;
		j["sessionKey"] = bot_->GetSessionKey();
		j["eventId"] = this->EventId;
		j["fromId"] = (int64_t)this->FromId;
		j["groupId"] = (int64_t)this->GroupId;
		j["operate"] = operate;
		j["message"] = message;

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);

		if (res.Ready)
		{
			return true;
		}
		else
			throw runtime_error(res.ErrorMsg);

	}

	bool MemberJoinRequestEvent::Respose(int operate, const string& message)
	{
		static const string api_url = bot_->GetApiUrlPrefix() + "/resp/memberJoinRequestEvent";

		json j;
		j["sessionKey"] = bot_->GetSessionKey();
		j["eventId"] = this->EventId;
		j["fromId"] = (int64_t)this->FromId;
		j["groupId"] = (int64_t)this->GroupId;
		j["operate"] = operate;
		j["message"] = message;

		string pData = j.dump();
		HTTP http; http.SetContentType("application/json;charset=UTF-8");
		auto res = http.Post(api_url, pData);

		if (res.Ready)
		{
			return true;
		}
		else
			throw runtime_error(res.ErrorMsg);

	}


}
#endif // !mirai_cpp__event_func_hpp_H_