#pragma once
#ifndef mirai_cpp_defs_message_interface_hpp_H_
#define mirai_cpp_defs_message_interface_hpp_H_

#include <nlohmann/json.hpp>
#include "serializable.hpp"

namespace Cyan
{
	/**
	 * @brief 消息的抽象类
	*/
	class IMessage : public ISerializable
	{
		IMessage() {}
		virtual const string& GetType() const = 0;
		virtual bool operator==(const MessageChain& mc) const = 0;
		virtual bool operator!=(const MessageChain& mc) const = 0;
		virtual ~IMessage() {}
	};

}
#endif

