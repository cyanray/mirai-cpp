#pragma once
#ifndef mirai_cpp_defs_messages_poke_message_hpp_H_
#define mirai_cpp_defs_messages_poke_message_hpp_H_
#include <utility>
#include "mirai/defs/IMessage.hpp"

namespace Cyan
{
	enum class PokeType
	{
		Poke,
		ShowLove,
		Like,
		Heartbroken,
		SixSixSix,
		FangDaZhao
	};

	class PokeMessage : public IMessage
	{
	public:
		PokeMessage() : poke_(PokeType::Poke), name_() {}
		PokeMessage(PokeType p) : poke_(p), name_()
		{
			Poke(p);
		}
		PokeMessage(const PokeMessage& m) : poke_(m.poke_), name_(m.name_) {}
		PokeMessage(PokeMessage&& m) noexcept
		{
			std::swap(this->poke_, m.poke_);
			std::swap(this->name_, m.name_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const PokeMessage*>(&m))
			{
				return m_ptr->poke_ == this->poke_;
			}
			return false;
		}
		virtual bool operator!=(const IMessage& m) const override
		{
			return !(*this == m);
		}
		virtual bool Set(const json& json) override
		{
			if (json["type"].is_null() || json["type"].get<string>() != this->GetType())
				throw std::runtime_error("给定的json不正确");
			name_ = json["name"].get<string>();
			for (int i = 0; i < 6; ++i)
			{
				if (poke_name[i] == name_)
				{
					poke_ = static_cast<PokeType>(i);
					break;
				}
			}
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "name",name_ }
			};
		}
		virtual ~PokeMessage() {}

		PokeType Poke() const { return poke_; }
		void Poke(PokeType p)
		{
			this->poke_ = p;
			this->name_ = poke_name[static_cast<int>(p)];
		}

		string Name() const { return name_; }


	private:
		const string poke_name[6] =
		{ "Poke","ShowLove","Like","Heartbroken","SixSixSix","FangDaZhao" };
		const string type_ = "Poke";
		string name_;
		PokeType poke_;
	};

}
#endif

