#include <exception>
#include <algorithm>
#include "mirai/mirai_bot.hpp"
#include "mirai/defs/message_chain.hpp"
#include "mirai/events/events.hpp"
#include "mirai/third-party/httplib.h"

// fu*k windows.h
#ifdef max
#undef max
#endif
#ifdef SendMessage
#undef SendMessage
#endif
#ifdef CreateEvent
#undef CreateEvent
#endif

namespace Cyan
{

	MessageId_t TempMessage::Reply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.Group.GID, Sender.QQ, mc);
	}

	MessageId_t FriendMessage::Reply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.QQ, mc);
	}

	MessageId_t GroupMessage::Reply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.Group.GID, mc);
	}

	MessageId_t TempMessage::QuoteReply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.Group.GID, Sender.QQ, mc, MessageId());
	}

	MessageId_t FriendMessage::QuoteReply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.QQ, mc, MessageId());
	}

	MessageId_t GroupMessage::QuoteReply(const Cyan::MessageChain& mc) const
	{
		return bot_->SendMessage(Sender.Group.GID, mc, MessageId());
	}

	bool GroupMessage::Recall() const
	{
		return bot_->Recall(MessageId());
	}

	bool GroupMessage::AtMe() const
	{
		auto at = MessageChain.GetAll<AtMessage>();
		auto it = std::find_if(at.begin(), at.end(), [&](AtMessage a) {return a.Target() == bot_->GetBotQQ(); });
		if (it != at.end()) return true;
		else return false;
	}

	bool NewFriendRequestEvent::Response(int operate, const string& message)
	{
		json data =
		{
			{ "sessionKey",  bot_->GetSessionKey() },
			{ "eventId", this->EventId},
			{ "fromId",  int64_t(this->FromId)},
			{ "groupId", int64_t(this->GroupId)},
			{ "operate", operate},
			{ "message", message},
		};

		httplib::Client& http_client = *(bot_->GetHttpClient());
		auto res = http_client.Post("/resp/newFriendRequestEvent", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		return true;
	}

	bool MemberJoinRequestEvent::Response(int operate, const string& message)
	{

		json data =
		{
			{ "sessionKey",  bot_->GetSessionKey() },
			{ "eventId", this->EventId},
			{ "fromId",  int64_t(this->FromId)},
			{ "groupId", int64_t(this->GroupId)},
			{ "operate", operate},
			{ "message", message},
		};

		httplib::Client& http_client = *(bot_->GetHttpClient());
		auto res = http_client.Post("/resp/memberJoinRequestEvent", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		return true;
	}

	bool BotInvitedJoinGroupRequestEvent::Response(int operate, const string& message)
	{
		json data =
		{
			{ "sessionKey",  bot_->GetSessionKey() },
			{ "eventId", this->EventId},
			{ "fromId",  int64_t(this->FromId)},
			{ "groupId", int64_t(this->GroupId)},
			{ "operate", operate},
			{ "message", message},
		};

		httplib::Client& http_client = *(bot_->GetHttpClient());
		auto res = http_client.Post("/resp/botInvitedJoinGroupRequestEvent", data.dump(), "application/json;charset=UTF-8");
		if (!res)
			throw std::runtime_error("网络错误");
		if (res->status != 200)
			throw std::runtime_error("[mirai-api-http error]: " + res->body);
		return true;
	}

	MessageId_t Message::Reply(const Cyan::MessageChain& mc) const
	{
		switch (messageType_)
		{
		case	 MessageType::FriendMessage: return friendMessage_.Reply(mc);
		case MessageType::GroupMessage: return groupMessage_.Reply(mc);
		case MessageType::TempMessage: return tempMessage_.Reply(mc);
		default: throw std::runtime_error("错误的 MessageType .");
		}
	}

	MessageId_t Message::QuoteReply(const Cyan::MessageChain& mc) const
	{
		switch (messageType_)
		{
		case	 MessageType::FriendMessage: return friendMessage_.QuoteReply(mc);
		case MessageType::GroupMessage: return groupMessage_.QuoteReply(mc);
		case MessageType::TempMessage: return tempMessage_.QuoteReply(mc);
		default: throw std::runtime_error("错误的 MessageType .");
		}
	}
	
	bool Command::SenderIsManager()
	{
		// 控制台发来的指令，算作是Manager
		if (this->Sender.ToInt64() == 0) return true;
		vector<QQ_t> managers = this->GetMiraiBot().GetManagers();
		auto it = std::find_if(managers.begin(), managers.end(), 
			[&](QQ_t q) { return q.ToInt64() == this->Sender.ToInt64(); });
		return it != managers.end();
	}

}