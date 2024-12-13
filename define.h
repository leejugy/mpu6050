#ifndef __DEF__
#define __DEF__

#include <stdio.h>

#define err_p(fmt, ...) printf("[\x1b[31m%s\x1b[0m]" fmt "err\n", __FUNCTION__,##__VA_ARGS__)
#define want_p(fmt, ...) printf("[\x1b[33m%s\x1b[0m]" fmt "\n", __FUNCTION__,##__VA_ARGS__)

#endif