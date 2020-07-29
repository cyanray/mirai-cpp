#include "defs/message_chain.hpp"

namespace Cyan
{
	MessageChain::MessageChain() :messages_(), messageId_(0), timestamp_(0)
	{
		if (factory_.size() <= 0)
		{
			factory_.Register<PlainMessage>("Plain");
		}
	}

	MessageChain::MessageChain(const MessageChain& mc)
	{
		messages_ = mc.messages_;
		messageId_ = mc.messageId_;
		timestamp_ = mc.timestamp_;
	}

	MessageChain::MessageChain(MessageChain&& mc) noexcept
	{
		std::swap(messages_, mc.messages_);
		std::swap(messageId_, mc.messageId_);
		std::swap(timestamp_, mc.timestamp_);
	}

	MessageChain& MessageChain::operator=(const MessageChain& mc)
	{
		MessageChain tmp(mc);
		std::swap(messages_, tmp.messages_);
		std::swap(messageId_, tmp.messageId_);
		std::swap(timestamp_, tmp.timestamp_);
		return *this;
	}

	MessageChain& MessageChain::operator=(MessageChain&& mc) noexcept
	{
		std::swap(messages_, mc.messages_);
		std::swap(messageId_, mc.messageId_);
		std::swap(timestamp_, mc.timestamp_);
		return *this;
	}

	MessageChain& MessageChain::operator+(const MessageChain& mc)
	{
		messages_.insert(messages_.end(), mc.messages_.begin(), mc.messages_.end());
		return *this;
	}

	MessageChain& MessageChain::operator+(const string& val)
	{
		return this->Plain(val);
	}

	MessageChain& MessageChain::operator+=(const string& val)
	{
		return this->Plain(val);
	}

	bool MessageChain::operator==(const MessageChain& mc) const
	{
		if (this->messages_.size() != mc.messages_.size()) return false;
		for (size_t i = 0; i < messages_.size(); ++i)
		{
			if (messages_[i] != mc.messages_[i]) return false;
		}
		return true;
	}

	bool MessageChain::operator!=(const MessageChain& mc) const
	{
		return !this->operator==(mc);
	}


}