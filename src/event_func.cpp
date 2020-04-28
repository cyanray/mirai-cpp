#include "mirai_bot.hpp"
#include "events/events.hpp"
#include "httplib.h"

namespace Cyan
{

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
		json data =
		{
			{ "sessionKey",  bot_->GetSessionKey() },
			{ "eventId", this->EventId},
			{ "fromId",  (int64_t)this->FromId},
			{ "groupId", (int64_t)this->GroupId},
			{ "operate", operate},
			{ "message", message},
		};

		httplib::Client& http_client = *(bot_->GetHttpClient());
		auto res = http_client.Post("/resp/newFriendRequestEvent", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			return true;
		}
		else
			throw runtime_error("网络错误");

	}

	bool MemberJoinRequestEvent::Respose(int operate, const string& message)
	{

		json data =
		{
			{ "sessionKey",  bot_->GetSessionKey() },
			{ "eventId", this->EventId},
			{ "fromId",  (int64_t)this->FromId},
			{ "groupId", (int64_t)this->GroupId},
			{ "operate", operate},
			{ "message", message},
		};

		httplib::Client& http_client = *(bot_->GetHttpClient());
		auto res = http_client.Post("/resp/memberJoinRequestEvent", data.dump(), "application/json;charset=UTF-8");
		if (res)
		{
			if (res->status != 200)
				throw std::runtime_error("[mirai-api-http error]: " + res->body);
			return true;
		}
		else
			throw runtime_error("网络错误");

	}

}