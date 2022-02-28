#pragma once
#ifndef mirai_cpp_defs_Announcement_hpp_H_
#define mirai_cpp_defs_Announcement_hpp_H_

#include "mirai/third-party/nlohmann/json.hpp"
#include "mirai/defs/QQType.hpp"
#include "mirai/defs/Group.hpp"
#include "ISerializable.hpp"

namespace Cyan
{
	/**
	 * @brief 群通告
	*/
	class GroupAnnouncement : public ISerializable
	{
	public:
		/**
		 * @brief 群组
		*/
		Group_t Group;

		/**
		 * @brief 群公告内容
		*/
		string Content;

		/**
		 * @brief 发送者 QQ
		*/
		QQ_t SenderQQ;

		/**
		 * @brief 群公告唯一Id
		*/
		string AnnouncementId;

		/**
		 * @brief 是否所有群成员已确认
		*/
		bool AllConfirmed;

		/**
		 * @brief 已经确认的人数
		*/
		int ConfirmedMembersCount;

		/**
		 * @brief 发布时间
		*/
		int64_t PublicationTime;

		virtual bool Set(const json& j) override
		{
			Group.Set(j["group"]);
			Content = j["content"].get<string>();
			SenderQQ = (QQ_t)j["senderId"].get<int64_t>();
			AnnouncementId = j["fid"].get<string>();
			AllConfirmed = j["allConfirmed"].get<bool>();
			ConfirmedMembersCount = j["confirmedMembersCount"].get<int>();
			PublicationTime = j["publicationTime"].get<int64_t>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "group", Group.ToJson() },
				{ "content", Content },
				{ "senderId", SenderQQ.ToInt64() },
				{ "fid", AnnouncementId },
				{ "allConfirmed", AllConfirmed },
				{ "confirmedMembersCount", ConfirmedMembersCount },
				{ "publicationTime", PublicationTime }
			};
		}
	};

}

#endif