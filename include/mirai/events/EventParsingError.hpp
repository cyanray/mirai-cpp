#pragma once
#ifndef mirai_cpp_events_EventParsingError_hpp_H_
#define mirai_cpp_events_EventParsingError_hpp_H_

#include <string>
#include "mirai/third-party/nlohmann/json.hpp"
#include "EventBase.hpp"
using std::string;
namespace Cyan
{
	/**
	 * \brief 解析事件错误 (一般因为与MAH版本不同导致)
	 */
	class EventParsingError : public EventBase
	{
	public:
		EventParsingError(std::exception_ptr ptr) : eptr(ptr) {}

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

		void Rethrow()
		{
			if (eptr)
			{
				std::rethrow_exception(eptr);
			}
		}

	private:
		std::exception_ptr eptr;
	};

}

#endif