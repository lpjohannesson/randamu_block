#ifndef __GENERIC_H__
#define __GENERIC_H__


#include "CommonDefines.h"
#include "cstdint"
#include <string>
#include <vector>

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

template<typename Num = int>
std::vector<Num, std::allocator<Num>> GetNumbers(Num begin, Num end){
	std::vector<Num, std::allocator<Num>> arr;
	for (Num num = begin; num < end; num++)
		arr.push_back(num);

	return arr;
}

#endif // __GENERIC_H__