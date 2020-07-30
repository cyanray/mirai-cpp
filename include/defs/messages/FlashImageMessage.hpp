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
		FlashImageMessage(const MiraiImage& m)
		{
			ImageMessage::imageId_ = m.ID;
			ImageMessage::url_ = m.Url;
			ImageMessage::path_ = m.Path;
		}
		virtual const string& GetType() const override
		{
			return type_;
		}
	private:
		string type_ = "FlashImage";
	};

}
#endif

