#include "mirai/events/MemberHonorChangeEvent.hpp"
#include "mirai/third-party/magic_enum.hpp"
#include <stdexcept>

namespace Cyan
{
	MemberHonorChangeEvent::HonorAction MemberHonorChangeEvent::HonorActionStr(const string& str)
	{
		auto result = magic_enum::enum_cast<MemberHonorChangeEvent::HonorAction>(str);
		if (result.has_value())
		{
			return result.value();
		}
		throw std::runtime_error("未知的 MemberHonorChangeEvent::HonorAction.");
	}

	std::string MemberHonorChangeEvent::HonorActionStr(MemberHonorChangeEvent::HonorAction v)
	{
		return std::string(magic_enum::enum_name(v));
	}
}
