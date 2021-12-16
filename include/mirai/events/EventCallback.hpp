#pragma once
#ifndef mirai_cpp_events_event_processer_hpp_H_
#define mirai_cpp_events_event_processer_hpp_H_

#include <functional>
#include <memory>
#include "EventBase.hpp"

namespace Cyan
{
	template<typename T>
	using EventCallback = std::function<void(T)>;

	typedef std::shared_ptr<EventBase> WeakEvent;

	typedef std::function<WeakEvent(WeakEvent)> CallbackInvoker;
}


#endif // !mirai_cpp_events_event_processer_hpp_H_
