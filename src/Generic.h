#ifndef __GENERIC_H__
#define __GENERIC_H__

#include <map>
#include <string>
#include <vector>
#include <ranges>

#define CONCAT_(prefix, suffix) prefix##suffix
#define CONCAT(prefix, suffix) CONCAT_(prefix, suffix)

#define DO_SOMETHING(FUNC) namespace { struct CONCAT(b, __LINE__) { \
  CONCAT(b, __LINE__)(){FUNC} \
}; \
	CONCAT(b, __LINE__) CONCAT(code_b, __LINE__); \
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

template<typename K, typename V, typename Pred = decltype(_MapIterate_detail::predicate<K, V>)>
std::map<K, V> MapIterate(std::map<K, V>& map, Pred&& predicate = std::forward<Pred&&>(_MapIterate_detail::predicate<K, V>)){
	std::map<K, V> projectedMap = map;
	std::erase_if(projectedMap, [predicate](const auto& item){
		const auto&[key, value] = item;
		return !predicate(key, value);
	});
	return projectedMap;
}

#endif // __GENERIC_H__