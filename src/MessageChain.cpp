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

	std::shared_ptr<IMessage> MessageChain::operator[](int i)
	{
		return messages_[i];
	}

	string MessageChain::GetPlainText() const
	{
		using std::stringstream;
		stringstream ss;
		for (auto m : messages_)
		{
			if (m->GetType() == "Plain")
			{
				auto mptr = std::dynamic_pointer_cast<PlainMessage>(m);
				if (mptr)
				{
					ss << mptr->GetText();
				}

			}
		}
		return ss.str();
	}

	string MessageChain::GetPlainTextFirst() const
	{
		for (auto m : messages_)
		{
			if (m->GetType() == "Plain")
			{
				auto mptr = std::dynamic_pointer_cast<PlainMessage>(m);
				if (mptr)
				{
					return mptr->GetText();
				}
			}
		}
		return string();
	}


}