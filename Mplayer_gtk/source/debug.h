#ifndef		__DEBUG_H
#define		__DEBUG_H

#include <stdio.h>

#define pr_debug(format, arg...)			printf("DEBUG: ""[%s:%d]"format, __func__, __LINE__, ##arg)
#define pr_err(format, arg...)			printf("ERR: ""%s:"format, __func__, ##arg)

#endif