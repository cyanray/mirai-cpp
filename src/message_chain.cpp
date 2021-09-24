#include "mirai/defs/message_chain.hpp"
#include "mirai/defs/simple_reflect.hpp"
#include "mirai/messages/messages.hpp"

namespace Cyan
{

	static Cyan::Reflection<IMessage> factory_;

	MessageChain::MessageChain() :messages_(), messageId_(0), timestamp_(0)
	{
		if (factory_.size() <= 0)
		{
			factory_.Register<PlainMessage>("Plain");
			factory_.Register<ImageMessage>("Image");
			factory_.Register<FlashImageMessage>("FlashImage");
			factory_.Register<AtMessage>("At");
			factory_.Register<AtAllMessage>("AtAll");
			factory_.Register<AppMessage>("App");
			factory_.Register<JsonMessage>("Json");
			factory_.Register<XmlMessage>("Xml");
			factory_.Register<FaceMessage>("Face");
			factory_.Register<PokeMessage>("Poke");
			factory_.Register<QuoteMessage>("Quote");
			factory_.Register<FileMessage>("File");
			factory_.Register<DiceMessage>("Dice");
			factory_.Register<ForwardMessage>("Forward");
			factory_.Register<MusicShare>("MusicShare");
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

	void MessageChain::RemoveAt(int index)
	{
		messages_.erase(messages_.begin() + index);
	}

	string MessageChain::GetPlainText() const
	{
		using std::stringstream;
		stringstream ss;
		for (const auto& m : messages_)
		{
			if (m->GetType() == "Plain")
			{
				auto mptr = std::dynamic_pointer_cast<PlainMessage>(m);
				if (mptr)
				{
					ss << mptr->Text();
				}

			}
		}
		return ss.str();
	}

	string MessageChain::GetPlainTextFirst() const
	{
		for (const auto& m : messages_)
		{
			if (m->GetType() == "Plain")
			{
				auto mptr = std::dynamic_pointer_cast<PlainMessage>(m);
				if (mptr)
				{
					return mptr->Text();
				}
			}
		}
		return string();
	}

	bool MessageChain::Set(const json& j)
	{
		if (!j.empty())
		{
			try
			{
				if (j[0]["type"].get<string>() == "Source")
				{
					this->messageId_ = j[0]["id"].get<int64_t>();
					this->timestamp_ = j[0]["time"].get<int64_t>();
				}
			}
			catch (...)
			{
				this->messageId_ = 0;
				this->timestamp_ = 0;
			}
			// ForwardMessage::Node 中的 MessageChain 没有 Source
			// 因此从 0 开始，防止漏掉消息
			for (size_t i = 0; i < j.size(); i++)
			{
				auto msg_ptr = factory_.DynamicCreate(j[i]["type"]);
				if (msg_ptr)
				{
					msg_ptr->Set(j[i]);
					messages_.push_back(msg_ptr);
				}
			}

		}

		return true;
	}

	json MessageChain::ToJson() const
	{
		json j = json::array();
		for (const auto& m : messages_)
		{
			j.push_back(m->ToJson());
		}
		return j;
	}


}