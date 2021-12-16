#pragma once
#ifndef mirai_cpp_defs_message_interface_hpp_H_
#define mirai_cpp_defs_message_interface_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "ISerializable.hpp"

namespace Cyan
{
	/**
	 * @brief 消息的抽象类
	*/
	class IMessage : public ISerializable
	{
	public:
		IMessage() {}
		virtual const string& GetType() const = 0;
		virtual bool operator==(const IMessage&) const = 0;
		virtual bool operator!=(const IMessage& m) const
		{
			return !(*this == m);
		}
		virtual ~IMessage() {}
	};

}
#endif

