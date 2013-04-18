#ifndef __FUTURE_H__
#define __FUTURE_H__
#ifdef FUTURE_FEATURES
#	define STRING_SEPERATOR_SUPPORT /* TODO: support to a string seperator */ 
#	define THREAD_SAFE				/* TODO: thread safe */
#endif
#ifdef DEPRECATED_FEATURES
#	define CHAR_HASH_TABLE
#endif
#endif 
