#ifndef __GENERIC_H__
#define __GENERIC_H__


#include "CommonDefines.h"
#include "cstdint"
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <ranges>

#define DO_SOMETHING(FUNC) namespace { struct b__LINE__ { \
  b__LINE__(){FUNC} \
  ~b__LINE__(){FUNC} \
}; \
	b__LINE__ code; \
}

void GetCompileDate(int *year, int *month, int *day);
bool GetCompileVersion(int *v1, int *v2, int *v3, int *v4);
std::string GetVersionName();
bool IsBeta();
void DeleteSave(void);
bool WriteLog(const char *string, int value1, int value2, int value3);
bool IsKeyFile(const char *name);
long GetFileSizeLong(const char *path);
bool PushIntoArray(int* array, int value, size_t array_size);
bool RemoveFromArray(int* array, int value, size_t array_size);
bool ErrorLog(const char *string, int value);
bool IsShiftJIS(unsigned char c);
bool IsEnableBitmap(const char *path);

// https://stackoverflow.com/a/78269470/16862767
namespace _MapIterate_detail {
	template<typename K, typename V>
	auto predicate = [](K key, V value)->bool{ return true; };

	template<typename K, typename V>
	auto lookup(std::map<K, V> map, K key)
	{
		return map.at(key) | std::views::all;
	}
}

template<typename K, typename V, typename Predicate = decltype(_MapIterate_detail::predicate<K, V>)>
auto MapIterate(std::map<K, V>& map, Predicate&& predicate){
	auto transform = [&](const K& key)
    {
        return std::views::zip(std::views::repeat(key), _MapIterate_detail::lookup(map, key));
    };

    return std::views::filter(std::views::zip(std::views::keys(map), std::views::values(map)), [predicate](auto&& pair) -> bool {
		return predicate(std::get<0>(pair), std::get<1>(pair));
	}) | std::views::keys | std::views::transform(transform) | std::views::join;
}

#endif // __GENERIC_H__