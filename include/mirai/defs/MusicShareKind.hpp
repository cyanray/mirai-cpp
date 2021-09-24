#pragma once
#ifndef mirai_cpp_defs_MusicShareKind_hpp_H_
#define mirai_cpp_defs_MusicShareKind_hpp_H_
#include <string>

namespace Cyan
{

	enum class MusicShareKind
	{
		NeteaseCloudMusic,
		QQMusic,
		MiguMusic,
		KugouMusic,
		KuwoMusic
	};

	MusicShareKind MusicShareKindStr(const std::string& miraiEvent);
	std::string MusicShareKindStr(MusicShareKind miraiEvent);

}

// https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
namespace std {
	template <> struct hash<Cyan::MusicShareKind> {
		size_t operator() (const Cyan::MusicShareKind& t) const { return size_t(t); }
	};
}
#endif