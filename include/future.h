#ifndef __FUTURE_H__
#define __FUTURE_H__
/* 
 * future features contains the future that currently 
 * does not support yet
 */
#ifdef FUTURE_FEATURES
#	define STRING_SEPERATOR_SUPPORT /* TODO: support to a string seperator */ 
#endif
/*
 * experimental features is features that default does
 * not compile to the source code
 */
#ifdef EXPERIMENTAL_FEATURES
#	define THREAD_SAFE			/* make Glimpse thread-safe, but might slow the parser down */
#	define HANDLER_STACK		/* this make Glimpse record the parser stack, may slow the parser down */
#endif

/* default features */

#ifndef NO_DEFAULT_FEATURES
#	define LAZY_INSTANCE	/* makes the data instance object does not free the memery after its death, 
							 * hoping this will reduce the time of calling instance new/free */
#endif
/*
 * deprecated features is features that does not use any more 
 */
#ifdef DEPRECATED_FEATURES
#	define CHAR_HASH_TABLE
#endif

#endif 
