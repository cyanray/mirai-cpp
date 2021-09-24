#include "mirai/defs/MusicShareKind.hpp"
#include "mirai/third-party/magic_enum.hpp"
#include <stdexcept>

namespace Cyan
{
	MusicShareKind MusicShareKindStr(const std::string& musicShareKind)
	{
		auto kind = magic_enum::enum_cast<MusicShareKind>(musicShareKind);
		if (kind.has_value())
		{
			return kind.value();
		}
		throw std::runtime_error("未知的 MusicShareKind.");
	}

	std::string MusicShareKindStr(MusicShareKind musicShareKind)
	{
		return std::string(magic_enum::enum_name(musicShareKind));
	}
}
