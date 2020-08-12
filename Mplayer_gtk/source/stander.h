#ifndef	__STANDER_H
#define	__STANDER_H

#include <type.h>
static inline void prefetch(void *a ) { }

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

#define ARRAY_SIZE(_ary)		(sizeof(_ary)/sizeof(_ary[0]))

#endif
