#pragma once
#ifndef mirai_cpp_exceptions_network_exception_hpp_H_
#define mirai_cpp_exceptions_network_exception_hpp_H_

#include <exception>

namespace Cyan
{
	/**
	 * @brief ÍøÂç´íÎóÒì³£
	*/
	class NetworkException : public std::runtime_error
	{
	public:
		NetworkException(): std::runtime_error("ÍøÂç´íÎó.") {}
		virtual ~NetworkException() = default;
	};

}

#endif // !mirai_cpp_exceptions_network_exception_hpp_H_