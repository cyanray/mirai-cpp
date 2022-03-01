#pragma once
#ifndef mirai_cpp_defs_messages_voice_message_hpp_H_
#define mirai_cpp_defs_messages_voice_message_hpp_H_
#include <utility>
#include "mirai/defs/IMessage.hpp"
#include "mirai/defs/QQType.hpp"

namespace Cyan
{
	class VoiceMessage : public IMessage
	{
	public:
		VoiceMessage() {}
		VoiceMessage(const MiraiVoice& m) : 
			voiceId_(m.Id), 
			url_(m.Url), 
			path_(m.Path), 
			base64_(m.Base64), 
			length_(0) {}
		VoiceMessage(const VoiceMessage& m) : 
			voiceId_(m.voiceId_), 
			url_(m.voiceId_), 
			path_(m.path_), 
			base64_(m.base64_), 
			length_(m.length_) {}
		VoiceMessage(VoiceMessage&& m) noexcept
		{
			std::swap(this->voiceId_, m.voiceId_);
			std::swap(this->url_, m.url_);
			std::swap(this->path_, m.path_);
			std::swap(this->base64_, m.base64_);
			std::swap(this->length_, m.length_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const VoiceMessage*>(&m))
			{
				// 用 voiceId 作为判断依据，必须有一个 voiceId 不为空
				if (!m_ptr->voiceId_.empty() || !this->voiceId_.empty())
				{
					return (m_ptr->voiceId_ == this->voiceId_);
				}
				// 如果 voiceId 都为空，那么用 url 再判断一下
				if (!m_ptr->url_.empty() || !this->url_.empty())
				{
					return (m_ptr->url_ == this->url_);
				}
				// 如果 url 都为空，那么用 path 再判断一下
				if (!m_ptr->path_.empty() || !this->path_.empty())
				{
					return (m_ptr->path_ == this->path_);
				}
				// 如果 path 都为空，那么用 base64 再判断一下
				if (!m_ptr->base64_.empty() || !this->base64_.empty())
				{
					return (m_ptr->base64_ == this->base64_);
				}
				// 所有参数都为空，两个空的 VoiceMessage 当然是相等的：
				return true;
			}
			// 类型都不同，直接不相等：
			return false;
		}
		virtual bool operator!=(const IMessage& m) const override
		{
			return !(*this == m);
		}
		virtual bool Set(const json& json) override
		{
			if (json["type"].is_null() || json["type"].get<string>() != this->GetType())
				throw std::runtime_error("给定的json不正确");

			voiceId_ = "";
			url_ = "";
			path_ = "";
			base64_ = "";
			length_ = 0;

			if (!json["voiceId"].is_null())
				voiceId_ = json["voiceId"].get<string>();
			if (!json["url"].is_null())
				url_ = json["url"].get<string>();
			if (!json["path"].is_null())
				path_ = json["path"].get<string>();
			if (!json["base64"].is_null())
				base64_ = json["base64"].get<string>();
			if (!json["length"].is_null())
				length_ = json["length"].get<size_t>();
			return true;
		}
		virtual json ToJson() const override
		{
			json result = 
			{
				{ "type", type_ },
				{ "voiceId", voiceId_.empty() ? nullptr : voiceId_ },
				{ "url", url_.empty() ? nullptr : url_ },
				{ "path", path_.empty() ? nullptr : path_ },
				{ "base64", base64_.empty() ? nullptr : base64_ },
				{ "length", length_ }
			};

			voiceId_.empty()
				? result["voiceId"] = json(nullptr)
				: result["voiceId"] = voiceId_;

			url_.empty()
				? result["url"] = json(nullptr)
				: result["url"] = url_;

			path_.empty()
				? result["path"] = json(nullptr)
				: result["path"] = path_;

			base64_.empty()
				? result["base64"] = json(nullptr)
				: result["base64"] = base64_;
			return result;
		}
		virtual ~VoiceMessage() {}

		MiraiVoice ToMiraiVoice() const
		{
			MiraiVoice tmp;
			tmp.Id = voiceId_;
			tmp.Url = url_;
			tmp.Path = path_;
			tmp.Length = length_;
			tmp.Base64 = base64_;
			return tmp;
		}

		const string& Id() const { return voiceId_; }
		void Id(std::string_view id) { voiceId_ = id; }

		const string& Url() const { return url_; }
		void Url(std::string_view url) { url_ = url; }

		const string& Path() const { return path_; }
		void Path(std::string_view path) { path_ = path; }

		const string& Base64() const { return base64_; }
		void Base64(const string& v) { base64_ = v; }

		size_t Length() const { return length_; }
		void Length(size_t v) { length_ = v; }


	private:
		const string type_ = "Voice";
	protected:
		size_t length_;
		string voiceId_;
		string url_;
		string path_;
		string base64_;
	};

}
#endif

