#ifndef __RESOURCE_H__
#define __RESOURCE_H__


#include <cstddef>

const unsigned char* FindResource(const char *name, const char *type, size_t *size);

#endif // __RESOURCE_H__