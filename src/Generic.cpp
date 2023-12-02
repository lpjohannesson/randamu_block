#include "Generic.h"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>

#include "ResourceCode.h"

#include "Main.h"

void GetCompileDate(int *year, int *month, int *day)
{
	int i;
	char strMonth[0x10];

	const char *table[13] = {
		"XXX",
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec",
	};

	sscanf(__DATE__, "%s %d %d", strMonth, day, year);	// The expansion of __DATE__ is not reproductible. TODO : Think about changing this to be reproductible

	for (i = 0; i < 12; ++i)	// This being 12 instead of 13 might be a bug, but it works anyway by accident
		if (!memcmp(&strMonth, table[i], 3))
			break;

	*month = i;
}

bool GetCompileVersion(int *v1, int *v2, int *v3, int *v4)
{
	*v1 = 1;
	*v2 = 43;
	*v3 = 0;
	*v4 = 5;
	return true;
}

bool PushIntoArray(int* array, int value, size_t array_size){
	int i = 0;
	while (i < array_size && array[i] != 0)
		i++;
	
	if (i >= array_size) return false;

	array[i] = value;
	return true;
}

bool RemoveFromArray(int* array, int value, size_t array_size){
	return true;
}

std::string GetVersionName(){
	return "Gamekutsu Rebaldamu";
}

bool IsBeta(){
	return true;
}

void DeleteSave(void)
{
	std::string path;

	path = gModulePath + "/Profile.json";
	remove(path.c_str());
	path = gModulePath + "/RNG.json";
	remove(path.c_str());
}

void DeleteLog(void)
{
	std::string path;

	path = gModulePath + "/debug.txt";
	remove(path.c_str());
}

bool WriteLog(const char *string, int value1, int value2, int value3)
{
	std::string path;
	FILE *fp;

	path = gModulePath + "/debug.txt";
	fp = fopen(path.c_str(), "a+");

	if (fp == NULL)
		return false;

	fprintf(fp, "%s,%d,%d,%d\n", string, value1, value2, value3);
	fclose(fp);
	return true;
}

bool IsKeyFile(const char *name)
{
	std::string path;
	FILE *fp;

	path = gModulePath + '/' + name;

	fp = fopen(path.c_str(), "rb");

	if (fp == NULL)
		return false;

	fclose(fp);
	return true;
}

long GetFileSizeLong(const char *path)
{
	long len;
	FILE *fp;

	len = 0;

	fp = fopen(path, "rb");
	if (fp == NULL)
		return -1;

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fclose(fp);
	return len;
}

bool ErrorLog(const char *string, int value)
{
	std::string path;
	FILE *fp;

	path = gModulePath + "/error.log";

	if (GetFileSizeLong(path.c_str()) > 0x19000)	// Purge the error log if it gets too big, I guess
		remove(path.c_str());

	fp = fopen(path.c_str(), "a+");
	if (fp == NULL)
		return false;

	fprintf(fp, "%s,%d\n", string, value);
	fclose(fp);
	return true;
}

bool IsShiftJIS(unsigned char c)
{
	if (c >= 0x81 && c <= 0x9F)
		return true;

	if (c >= 0xE0 && c <= 0xEF)
		return true;

	return false;
}

static const char* const extra_text = "(C)Pixel";

bool IsEnableBitmap(const char *path)
{
	FILE *fp;
	long len;
	char str[16];

	len = (long)strlen(extra_text);

	fp = fopen(path, "rb");

	if (fp == NULL)
		return false;

	fseek(fp, len * -1, SEEK_END);
	fread(str, 1, len, fp);
	fclose(fp);

	if (memcmp(str, extra_text, len))
		return false;

	return true;
}
