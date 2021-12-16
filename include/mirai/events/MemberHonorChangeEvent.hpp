#pragma once
#ifndef mirai_cpp_events_MemberHonorChangeEvent_hpp_H_
#define mirai_cpp_events_MemberHonorChangeEvent_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "EventBase.hpp"
#include "mirai/defs/GroupMember.hpp"

namespace Cyan
{

	/**
	 * \brief 群成员称号改变事件
	 */
	class MemberHonorChangeEvent : public EventBase
	{
	public:
		/**
		 * @brief 称号事件的行为
		*/
		enum class HonorAction
		{
			Achieve,		// 获得称号
			Lose			// 失去称号
		};

		static HonorAction HonorActionStr(const string& str);
		static string HonorActionStr(HonorAction v);

		/**
		 * @brief 称号行为
		*/
		HonorAction Action = HonorAction::Achieve;

		/**
		 * @brief 称号名称
		*/
		string Honor;

		/**
		 * @brief 群成员
		*/
		GroupMember Member;


		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::MemberHonorChangeEvent;
		}

		virtual bool Set(const json& j) override
		{
			this->Member.Set(j["member"]);
			this->Action = HonorActionStr(j["action"].get<string>());
			this->Honor = j["honor"].get<string>();
			return true;
		}
		
		virtual json ToJson() const override
		{
			json j = json::object();
			j["type"] = "MemberHonorChangeEvent";
			j["member"] = this->Member.ToJson();
			j["honor"] = Honor;
			j["action"] = HonorActionStr(Action);
			return j;
		}
	};

}

#endif