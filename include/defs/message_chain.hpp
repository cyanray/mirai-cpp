#pragma once
#ifndef mirai_cpp_defs_message_chain_hpp_H_
#define mirai_cpp_defs_message_chain_hpp_H_

#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>
#include <memory>
#include "qq_types.hpp"
#include "serializable.hpp"
#include "defs/message_interface.hpp"
#include "defs/messages/PlainMessage.hpp"
#include "defs/messages/ImageMessage.hpp"
#include "defs/messages/FlashImageMessage.hpp"
#include "defs/messages/AtMessage.hpp"
#include "defs/messages/AtAllMessage.hpp"
#include "defs/messages/FaceMessage.hpp"
#include "defs/messages/XmlMessage.hpp"
#include "defs/messages/AppMessage.hpp"
#include "defs/messages/JsonMessage.hpp"
#include "defs/messages/PokeMessage.hpp"
#include "defs/messages/QuoteMessage.hpp"

using std::vector;

namespace Cyan
{

	class MessageChain : public ISerializable
	{
	public:
		friend MessageChain& operator+(const string& str, MessageChain& mc);
		template<int N>
		friend MessageChain& operator+(const char(&str)[N], MessageChain& mc);
		MessageChain();
		MessageChain(const MessageChain& mc);
		MessageChain(MessageChain&& mc) noexcept;
		MessageChain& operator=(const MessageChain& mc);
		MessageChain& operator=(MessageChain&& mc) noexcept;
		MessageChain& operator+(const MessageChain& mc);
		MessageChain& operator+(const string& val);
		MessageChain& operator+=(const string& val);
		bool operator==(const MessageChain& mc) const;
		bool operator!=(const MessageChain& mc) const;
		std::shared_ptr<IMessage> operator[](int i);
		virtual ~MessageChain() = default;

		template<class T>
		std::vector<T> GetAll() const
		{
			vector<T> tmp;
			for (size_t i = 0; i < messages_.size(); i++)
			{
				if (auto m = std::dynamic_pointer_cast<T>(messages_[i]))
				{
					tmp.push_back(*m);
				}
			}
			return tmp;
		}

		template<class T>
		T GetFirst() const
		{
			for (size_t i = 0; i < messages_.size(); i++)
			{
				if (auto m = std::dynamic_pointer_cast<T>(messages_[i]))
				{
					return *m;
				}
			}
			throw std::runtime_error("没有找到指定类型的元素");
		}

		template<class T>
		MessageChain& Add(const T& m)
		{
			static_assert(std::is_base_of<IMessage, T>::value, "只能接受 IMessage 的派生类");
			std::shared_ptr<IMessage> m_ptr(new T(m));
			messages_.push_back(m_ptr);
			return *this;
		}

		template<class T>
		void Remove(const T& m)
		{
			static_assert(std::is_base_of<IMessage, T>::value, "只能接受 IMessage 的派生类");
			messages_.erase(std::remove_if(messages_.begin(), messages_.end(),
				[&](std::shared_ptr<IMessage> item)
				{
					return *item == m;
				}), messages_.end());
		}

		size_t Count() const
		{
			return messages_.size();
		}

		void Clear()
		{
			messages_.clear();
		}

		std::vector<std::shared_ptr<IMessage>>& ToVector()
		{
			return messages_;
		}

		MessageChain& At(const QQ_t qq)
		{
			return this->Add(AtMessage(qq));
		}

		MessageChain& AtAll()
		{
			return this->Add(AtAllMessage());
		}

		MessageChain& Face(int faceID)
		{
			return this->Add(FaceMessage(faceID));
		}

		MessageChain& Face(const string& name)
		{
			auto face_msg = FaceMessage();
			face_msg.Name(name);
			return this->Add(face_msg);
		}

		MessageChain& Plain(const string& plainText)
		{
			return this->Add(PlainMessage(plainText));
		}

		template<typename T>
		MessageChain& Plain(const T& val)
		{
			std::stringstream ss;
			ss << val;
			return this->Add(PlainMessage(ss.str()));
		}

		MessageChain& Image(const MiraiImage& Image)
		{
			return this->Add(ImageMessage(Image));
		}

		MessageChain& FlashImage(const MiraiImage& Image)
		{
			return this->Add(FlashImageMessage(Image));
		}

		MessageChain& Xml(const string& xml_str)
		{
			return this->Add(XmlMessage(xml_str));
		}

		MessageChain& Json(const string& json_str)
		{
			return this->Add(JsonMessage(json_str));
		}

		MessageChain& App(const string& app_str)
		{
			return this->Add(AppMessage(app_str));
		}

		MessageChain& Poke(PokeType poke)
		{
			return this->Add(PokeMessage(poke));
		}

		string GetPlainText() const;

		string GetPlainTextFirst() const;

		MessageId_t MessageId() const
		{
			return messageId_;
		}

		int64_t Timestamp() const
		{
			return timestamp_;
		}

		virtual bool Set(const json& j) override;
		virtual json ToJson() const override;

	private:
		int64_t timestamp_;
		MessageId_t messageId_;
		vector<std::shared_ptr<IMessage>> messages_;
	};

	template<int N>
	inline MessageChain& operator+(const char(&str)[N], MessageChain& mc)
	{
		mc.messages_.insert(mc.messages_.begin(),std::make_shared<PlainMessage>(str));
		return mc;
	}

	inline MessageChain& operator+(const string& str, MessageChain& mc)
	{
		mc.messages_.insert(mc.messages_.begin(), std::make_shared<PlainMessage>(str));
		return mc;
	}

}


#endif // !mirai_cpp_defs_message_chain_hpp_H_