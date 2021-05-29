#pragma once
#ifndef mirai_cpp_defs_messages_flash_image_message_hpp_H_
#define mirai_cpp_defs_messages_flash_image_message_hpp_H_

#include "ImageMessage.hpp"

namespace Cyan
{
	class FlashImageMessage : public ImageMessage
	{
	public:
		FlashImageMessage() {}
		FlashImageMessage(const MiraiImage& m) : ImageMessage(m) {}
		FlashImageMessage(const FlashImageMessage& m) : ImageMessage(m) {}
		FlashImageMessage(FlashImageMessage&& m) noexcept : ImageMessage(std::move(m)) {}
		virtual const string& GetType() const override
		{
			return type_;
		}
		virtual json ToJson() const override
		{
			return
			{
				{ "type", type_ },
				{ "imageId", ImageMessage::imageId_ },
				{ "url", ImageMessage::url_ },
				{ "path", ImageMessage::path_ }
			};
		}
	private:
		const string type_ = "FlashImage";
	};

}
#endif

