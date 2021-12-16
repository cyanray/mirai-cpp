#pragma once
#ifndef mirai_cpp_events_Command_hpp_H_
#define mirai_cpp_events_Command_hpp_H_

#include <string>
#include <vector>
#include <optional>
#include "mirai/exported.h"
#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "EventBase.hpp"

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
		std::optional<GroupMember> Member;
		MessageChain Args;

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::CommandExecutedEvent;
		}

		virtual bool Set(const json& j) override
		{
			CommandName = j["name"].get<string>();
			if (!j["friend"].is_null())
			{
				Friend = Friend_t();
				Friend->Set(j["friend"]);
			}
			if (!j["member"].is_null())
			{
				Member = GroupMember();
				Member->Set(j["member"]);
			}
			Args.Set(j["args"]);
			return true;
		}

		virtual json ToJson() const override
		{
			return
			{
				{ "name" , CommandName },
				{ "friend" , Friend.has_value() ? Friend->ToJson() : json(nullptr) },
				{ "member" , Member.has_value() ? Member->ToJson() : json(nullptr) },
				{ "args" , Args.ToJson() }
			};
		}

	};

}

#endif