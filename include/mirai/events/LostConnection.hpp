#pragma once
#ifndef mirai_cpp_events_LostConnection_hpp_H_
#define mirai_cpp_events_LostConnection_hpp_H_

#include <string>
#include "mirai/exported.h"
#include "mirai/third-party/nlohmann/json.hpp"
#include "EventBase.hpp"
using std::string;
namespace Cyan
{
	/**
	 * \brief mirai-cpp 与 mirai-api-http 失去连接的事件
	 */
	class LostConnection : public EventBase
	{
	public:
		int Code = 0;
		string ErrorMessage;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::Default;
		}

		virtual bool Set(const json& j) override
		{
			throw std::logic_error("Function not yet implemented.");
		}
		virtual json ToJson() const override
		{
			throw std::logic_error("Function not yet implemented.");
		}

	};

}

#endif