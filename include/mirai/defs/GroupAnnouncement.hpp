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
        bool AllConfirmed = false;

        /**
         * @brief 已经确认的人数
        */
        int ConfirmedMembersCount = 0;

        /**
         * @brief 发布时间
        */
        int64_t PublicationTime = 0;

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

        /**
         * @brief 发布群公告的设置
        */
        enum PublishFlags
        {
            /**
             * @brief 是否推送给新成员
            */
            SendToNewMember = 0x01,

            /**
             * @brief 是否置顶
            */
            Pinned = 0x02,

            /**
             * @brief 是否显示群成员修改群名片的引导
            */
            ShowEditCard = 0x04,

            /**
             * @brief 是否自动弹出
            */
            ShowPopup = 0x08,

            /**
             * @brief 是否需要群成员确认
            */
            RequireConfirmation = 0x10
        };


    };

}

#endif