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
#	define THREAD_SAFE				/* TODO: thread safe */
#endif
/*
 * deprecated features is features that does not use any more 
 */
#ifdef DEPRECATED_FEATURES
#	define CHAR_HASH_TABLE
#endif
#endif 
