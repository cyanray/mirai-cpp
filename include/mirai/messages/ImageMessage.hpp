#pragma once
#ifndef mirai_cpp_defs_messages_image_message_hpp_H_
#define mirai_cpp_defs_messages_image_message_hpp_H_
#include "mirai/defs/message_interface.hpp"
#include "mirai/defs/qq_types.hpp"

namespace Cyan
{
	class ImageMessage : public IMessage
	{
	public:
		ImageMessage() {}
		ImageMessage(const MiraiImage& m) : imageId_(m.Id), url_(m.Url), path_(m.Path) {}
		ImageMessage(const ImageMessage& m) : imageId_(m.imageId_), url_(m.url_), path_(m.path_) {}
		ImageMessage(ImageMessage&& m) noexcept
		{
			std::swap(this->imageId_, m.imageId_);
			std::swap(this->url_, m.url_);
			std::swap(this->path_, m.path_);
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual bool operator==(const IMessage& m) const override
		{
			if (auto m_ptr = dynamic_cast<const ImageMessage*>(&m))
			{
				// 用 ImageId 作为判断依据，必须有一个 ImageId 不为空
				if (!m_ptr->imageId_.empty() || !this->imageId_.empty())
				{
					return (m_ptr->imageId_ == this->imageId_);
				}
				// 如果 ImageId 都为空，那么用 url 再判断一下
				if (!m_ptr->url_.empty() || !this->url_.empty())
				{
					return (m_ptr->url_ == this->url_);
				}
				// 如果 url 都为空，那么用 path 再判断一下
				if (!m_ptr->path_.empty() || !this->path_.empty())
				{
					return (m_ptr->path_ == this->path_);
				}
				// 三个参数都为空，两个空的 ImageMessage 当然是相等的：
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
			if (!json["imageId"].is_null())
				imageId_ = json["imageId"].get<string>();
			if (!json["url"].is_null())
				url_ = json["url"].get<string>();
			if (!json["path"].is_null())
				path_ = json["path"].get<string>();
			return true;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "imageId", imageId_ },
				{ "url", url_ },
				{ "path", path_ }
			};
		}
		virtual ~ImageMessage() {}

		MiraiImage ToMiraiImage() const
		{
			MiraiImage tmp;
			tmp.Id = imageId_;
			tmp.Url = url_;
			tmp.Path = path_;
			return tmp;
		}

	private:
		const string type_ = "Image";
	protected:
		string imageId_;
		string url_;
		string path_;
	};

}
#endif

