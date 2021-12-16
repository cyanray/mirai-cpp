#pragma once
#ifndef mirai_cpp_events_NudgeEvent_hpp_H_
#define mirai_cpp_events_NudgeEvent_hpp_H_

#include <exception>
#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "EventBase.hpp"

namespace Cyan
{
	/**
	 * \brief 戳一戳(头像)事件
	 */
	class NudgeEvent : public EventBase
	{
	public:
		enum class SubjectKind
		{
			Friend, Group
		};

		QQ_t FromId;
		QQ_t Target;
		int64_t RawSubjectId;
		SubjectKind FromKind;
		string Action;
		string Suffix;

		std::shared_ptr<UID_t> GetSubjectId()
		{
			if (FromKind == SubjectKind::Group)
			{
				return std::make_shared<GID_t>(RawSubjectId);
			}
			else
			{
				return std::make_shared<QQ_t>(RawSubjectId);
			}
		}

		static MiraiEvent GetMiraiEvent()
		{
			return MiraiEvent::NudgeEvent;
		}

		static string SubjectKindStr(SubjectKind k)
		{
			switch (k)
			{
			case Cyan::NudgeEvent::SubjectKind::Friend:
				return "Friend";
			case Cyan::NudgeEvent::SubjectKind::Group:
				return "Group";
			}
			return "";
		}

		static SubjectKind SubjectKindStr(const string& k)
		{
			if (k == "Friend") return SubjectKind::Friend;
			if (k == "Group") return SubjectKind::Group;
			throw std::runtime_error("Unknown SubjectKind.");
		}

		virtual bool Set(const json& j) override
		{
			this->FromId = (QQ_t)(j["fromId"].get<int64_t>());
			this->Target = (QQ_t)(j["target"].get<int64_t>());
			this->RawSubjectId = j["subject"]["id"].get<int64_t>();
			this->FromKind = SubjectKindStr(j["subject"]["kind"].get<string>());
			this->Action = j["action"].get<string>();
			this->Suffix = j["suffix"].get<string>();
			return true;
		}

		virtual json ToJson() const override
		{
			return
			{
				{ "type", "NudgeEvent" },
				{ "fromId", (int64_t)FromId },
				{ "target", (int64_t)Target },
				{ "subject", { {"id", RawSubjectId }, { "kind", SubjectKindStr(FromKind) } } },
				{ "action", Action },
				{ "suffix", Suffix }
			};
		}
	};

}

#endif