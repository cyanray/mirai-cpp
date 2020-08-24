#pragma once
#ifndef mirai_cpp_events_command_event_hpp_H_
#define mirai_cpp_events_command_event_hpp_H_

#include <string>
#include <vector>
#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/qq_types.hpp"
#include "event_interface.hpp"

using std::string;
using std::vector;

namespace Cyan
{
	/**
	 * \brief 指令事件
	 */
	class Command : public EventBase
	{
	public:
		string CommandName;
		QQ_t Sender;
		GID_t GroupId;
		vector<string> Args;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::Command;
		}

		virtual bool Set(const json& j) override
		{
			CommandName = j["name"].get<string>();
			Sender = (QQ_t)(j["friend"].get<int64_t>());
			GroupId = (GID_t)(QQ_t)(j["group"].get<int64_t>());
			Args = j["args"].get<vector<string>>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "name" , CommandName },
				{ "friend" , Sender.ToInt64() },
				{ "group" , GroupId.ToInt64() },
				{ "args" , json(Args) }
			};
		}

	};

}

#endif // !mirai_cpp_events_command_event_hpp_H_