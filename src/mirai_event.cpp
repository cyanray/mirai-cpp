#include "mirai/events/mirai_event.hpp"
#include "mirai/third-party/magic_enum.hpp"

namespace Cyan
{
	MiraiEvent MiraiEventStr(const std::string& miraiEvent)
	{
		auto mirai_event = magic_enum::enum_cast<MiraiEvent>(miraiEvent);
		if (mirai_event.has_value()) 
		{
			return mirai_event.value();
		}
		return MiraiEvent::Default;
	}

	std::string MiraiEventStr(MiraiEvent miraiEvent)
	{
		return std::string(magic_enum::enum_name(miraiEvent));
	}
}

