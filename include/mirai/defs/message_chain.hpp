#pragma once
#ifndef mirai_cpp_defs_message_chain_hpp_H_
#define mirai_cpp_defs_message_chain_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include <sstream>
#include <vector>
#include <memory>
#include "mirai/exported.h"
#include "qq_types.hpp"
#include "serializable.hpp"
#include "message_interface.hpp"
#include "messages/PlainMessage.hpp"
#include "messages/ImageMessage.hpp"
#include "messages/FlashImageMessage.hpp"
#include "messages/AtMessage.hpp"
#include "messages/AtAllMessage.hpp"
#include "messages/FaceMessage.hpp"
#include "messages/XmlMessage.hpp"
#include "messages/AppMessage.hpp"
#include "messages/JsonMessage.hpp"
#include "messages/PokeMessage.hpp"
#include "messages/QuoteMessage.hpp"
#include "messages/VoiceMessage.hpp"

using std::vector;

namespace Cyan
{

	class EXPORTED MessageChain : public ISerializable
	{
	public:
		typedef vector<std::shared_ptr<IMessage>>::iterator iterator;

		friend MessageChain& operator+(const string& str, MessageChain& mc);

		template<typename T>
		friend MessageChain& operator+(T&& val, MessageChain& mc);

		template<std::size_t N>
		friend MessageChain& operator+(const char(&str)[N], MessageChain& mc);

		MessageChain();
		MessageChain(const MessageChain& mc);
		MessageChain(MessageChain&& mc) noexcept;
		MessageChain& operator=(const MessageChain& mc);
		MessageChain& operator=(MessageChain&& mc) noexcept;
		MessageChain& operator+(const MessageChain& mc);
		MessageChain& operator+(const string& val);

		template<typename T>
		MessageChain& operator+=(T&& val)
		{
			return this->Add(std::forward<T>(val));
		}

		template<std::size_t N>
		MessageChain& operator+=(const char (&val)[N])
		{
			return this->Add<PlainMessage>(val);
		}

		bool operator==(const MessageChain& mc) const;
		bool operator!=(const MessageChain& mc) const;
		std::shared_ptr<IMessage> operator[](int i);
		virtual ~MessageChain() = default;

		iterator begin() { return messages_.begin(); }

		iterator end() { return messages_.end(); }

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
		MessageChain& Add(T&& m)
		{
			using real_type = typename std::remove_const<typename std::remove_reference<T>::type >::type;
			static_assert(std::is_base_of<IMessage, real_type>::value, "只能接受 IMessage 的派生类");
			std::shared_ptr<IMessage> m_ptr(new real_type(std::forward<T>(m)));
			messages_.emplace_back(std::move(m_ptr));
			return *this;
		}

		template<class T, class... Args>
		MessageChain& Add(Args&&... args)
		{
			static_assert(std::is_base_of<IMessage, T>::value, "只能接受 IMessage 的派生类");
			std::shared_ptr<IMessage> m_ptr(new T(args...));
			messages_.emplace_back(std::move(m_ptr));
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

		void RemoveAt(int index);

		size_t Count() const
		{
			return messages_.size();
		}

		bool Empty() const
		{
			return Count() == 0;
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
			return this->Add<AtMessage>(qq);
		}

		MessageChain& AtAll()
		{
			return this->Add<AtAllMessage>();
		}

		MessageChain& Face(int faceID)
		{
			return this->Add<FaceMessage>(faceID);
		}

		MessageChain& Face(const string& name)
		{
			return this->Add<FaceMessage>(name);
		}

		MessageChain& Plain(const string& plainText)
		{
			return this->Add<PlainMessage>(plainText);
		}

		template<typename T>
		MessageChain& Plain(const T& val)
		{
			std::stringstream ss;
			ss << val;
			return this->Add<PlainMessage>(ss.str());
		}

		MessageChain& Image(const MiraiImage& Image)
		{
			return this->Add<ImageMessage>(Image);
		}

		MessageChain& FlashImage(const MiraiImage& Image)
		{
			return this->Add<FlashImageMessage>(Image);
		}

		MessageChain& Xml(const string& xml_str)
		{
			return this->Add<XmlMessage>(xml_str);
		}

		MessageChain& Json(const string& json_str)
		{
			return this->Add<JsonMessage>(json_str);
		}

		MessageChain& App(const string& app_str)
		{
			return this->Add<AppMessage>(app_str);
		}

		MessageChain& Poke(PokeType poke)
		{
			return this->Add<PokeMessage>(poke);
		}

		MessageChain& Voice(const VoiceMessage& voice)
		{
			return this->Add<VoiceMessage>(voice);
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

	template<typename T>
	inline MessageChain& operator+(T&& val, MessageChain& mc)
	{
		using real_type = typename std::remove_const<typename std::remove_reference<T>::type >::type;
		static_assert(std::is_base_of<IMessage, real_type>::value, "只能接受 IMessage 的派生类");
		std::shared_ptr<IMessage> m_ptr(new real_type(std::forward<T>(val)));
		mc.messages_.insert(mc.messages_.begin(), m_ptr);
		return mc;
	}

	template<std::size_t N>
	inline MessageChain& operator+(const char(&str)[N], MessageChain& mc)
	{
		mc.messages_.insert(mc.messages_.begin(), std::make_shared<PlainMessage>(str));
		return mc;
	}

	inline MessageChain& operator+(const string& str, MessageChain& mc)
	{
		mc.messages_.insert(mc.messages_.begin(), std::make_shared<PlainMessage>(str));
		return mc;
	}
}


#endif // !mirai_cpp_defs_message_chain_hpp_H_