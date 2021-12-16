#pragma once
#ifndef mirai_cpp_defs_messages_MusicShare_hpp_H_
#define mirai_cpp_defs_messages_MusicShare_hpp_H_

#include <string>
#include <string_view>
#include "mirai/defs/IMessage.hpp"
#include "mirai/defs/MusicShareKind.hpp"
using std::string;
using std::string_view;

namespace Cyan
{
	class MusicShare : public IMessage
	{
	public:
		virtual const string& GetType() const override
		{
			return type_;
		}

		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const MusicShare*>(&m))
			{
				if (this->kind_ != m_ptr->kind_) return false;
				if (this->title_ != m_ptr->title_) return false;
				if (this->brief_ != m_ptr->brief_) return false;
				if (this->jumpUrl_ != m_ptr->jumpUrl_) return false;
				if (this->musicUrl_ != m_ptr->musicUrl_) return false;
				if (this->pictureUrl_ != m_ptr->pictureUrl_) return false;
				if (this->summary_ != m_ptr->summary_) return false;
			}
			return false;
		}

		virtual bool Set(const json& json) override
		{
			if (json["type"].is_null() || json["type"].get<string>() != this->GetType())
			{
				throw std::runtime_error("给定的json不正确");
			}
			kind_ = MusicShareKindStr(json["kind"].get<string>());
			title_ = json["title"].get<string>();
			summary_ = json["summary"].get<string>();
			brief_ = json["brief"].get<string>();
			jumpUrl_ = json["jumpUrl"].get<string>();
			musicUrl_ = json["musicUrl"].get<string>();
			pictureUrl_ = json["pictureUrl"].get<string>();
			return true;
		}

		virtual json ToJson() const override
		{
			json nodeListJson = json::array();
			return
			{
				{ "type", type_ },
				{ "kind", MusicShareKindStr(kind_) },
				{ "title", title_ },
				{ "summary", summary_ },
				{ "brief", brief_ },
				{ "jumpUrl", jumpUrl_ },
				{ "musicUrl", musicUrl_ },
				{ "pictureUrl", pictureUrl_ },
			};
		}

		virtual ~MusicShare() {}

		MusicShareKind Kind() const { return kind_; }
		void Kind(MusicShareKind value) { kind_ = value; }

		string Title() const { return title_; }
		void Title(std::string_view value) { title_ = value; }

		string Summary() const { return summary_; }
		void Summary(std::string_view value) { summary_ = value; }

		string Brief() const { return brief_; }
		void Brief(std::string_view value) { brief_ = value; }

		string JumUrl() const { return jumpUrl_; }
		void JumUrl(std::string_view value) { jumpUrl_ = value; }

		string PictureUrl() const { return pictureUrl_; }
		void PictureUrl(std::string_view value) { pictureUrl_ = value; }

		string MusicUrl() const { return musicUrl_; }
		void MusicUrl(std::string_view value) { musicUrl_ = value; }

	private:
		const string type_ = "MusicShare";
		MusicShareKind kind_ = MusicShareKind::NeteaseCloudMusic;
		string title_;
		string summary_;
		string jumpUrl_;
		string pictureUrl_;
		string musicUrl_;
		string brief_;
	};

}
#endif

