#pragma once
#include <string.h>
inline errno_t memset_s(void *dest, rsize_t  destsz, int ch, rsize_t count)
{
	if (nullptr == dest || destsz > RSIZE_MAX || count > RSIZE_MAX)
	{
		return EINVAL;
	}
	errno_t res = 0;
	if (count > destsz)
	{
		count = destsz;
		res = ERANGE;
	}

	memset(dest, ch, count);
	//std::uninitialized_fill_n((unsigned char*)dest, count, (unsigned char)ch);
	
	return res;
}
