#pragma once
#ifndef mirai_cpp_events_Command_hpp_H_
#define mirai_cpp_events_Command_hpp_H_

#include <string>
#include <vector>
#include <optional>
#include "mirai/exported.h"
#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/qq_types.hpp"
#include "event_interface.hpp"

using std::optional;
using std::nullopt;
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
		std::optional<Friend_t> Friend;
		std::optional<GroupMember> GroupMember;
		vector<string> Args;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::CommandExecutedEvent;
		}

		virtual bool Set(const json& j) override
		{
			CommandName = j["name"].get<string>();
			Friend = Friend_t();
			Friend->Set(j["friend"]);
			GroupMember = Cyan::GroupMember();
			GroupMember->Set(j["member"]);
			Args = j["args"].get<vector<string>>();
			return true;
		}

		virtual json ToJson() const override
		{
			return
			{
				{ "name" , CommandName },
				{ "friend" , Friend->ToJson() },
				{ "member" , GroupMember->ToJson() },
				{ "args" , json(Args) }
			};
		}

	};

}

#endif