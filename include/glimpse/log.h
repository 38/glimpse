/* log.h -   
 *
 * Copyright 2013 Hao Hou <ghost89413@gmail.com>
 * 
 * This file is part of Glimpse, a fast, flexible key-value scanner.
 * 
 * Glimpse is free software: you can redistribute it and/or modify it under the terms 
 * of the GNU General Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any later version.
 *
 * Glimpse is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Glimpse. 
 * If not, see http://www.gnu.org/licenses/.
 *
 */
	
#ifndef __GLIMPSE_LOG_H__
#define __GLIMPSE_LOG_H__
#include <glimpse/future.h>
#include <glimpse/def.h>
typedef enum {
	FATAL,
	ERROR,
	WARNING,
	NOTICE,
	INFO,
	TRACE,
	DEBUG
} ErrorLevel;
void glimpse_log_write(ErrorLevel level, const char* file, const char* function,int line, const char* fmt,...);
#define GLIMPSE_LOG(level,fmt,arg...) do{\
	glimpse_log_write(level,__FILE__,__FUNCTION__,__LINE__,fmt, ##arg);\
}while(0)
#ifndef LOG_LEVEL
	#define LOG_LEVEL 6
#endif
#if LOG_LEVEL >= 0
#	define GLIMPSE_LOG_FATAL(fmt,arg...) GLIMPSE_LOG(FATAL,fmt,##arg)
#else
#	define GLIMPSE_LOG_FATAL(...)
#endif

#if LOG_LEVEL >= 1
#	define GLIMPSE_LOG_ERROR(fmt,arg...) GLIMPSE_LOG(ERROR,fmt,##arg)
#else
#	define GLIMPSE_LOG_ERROR(...)
#endif

#if LOG_LEVEL >= 2
#	define GLIMPSE_LOG_WARNING(fmt,arg...) GLIMPSE_LOG(WARNING,fmt,##arg)
#else
#	define GLIMPSE_LOG_WARNING(...)
#endif

#if LOG_LEVEL >= 3
#	define GLIMPSE_LOG_NOTICE(fmt,arg...) GLIMPSE_LOG(NOTICE,fmt,##arg)
#else
#	define GLIMPSE_LOG_NOTICE(...)
#endif

#if LOG_LEVEL >= 4
#	define GLIMPSE_LOG_INFO(fmt,arg...) GLIMPSE_LOG(INFO,fmt,##arg)
#else
#	define GLIMPSE_LOG_INFO(...)
#endif

#if LOG_LEVEL >= 5
#	define GLIMPSE_LOG_TRACE(fmt,arg...) GLIMPSE_LOG(TRACE,fmt,##arg)
#else
#	define GLIMPSE_LOG_TRACE(...)
#endif

#if LOG_LEVEL >= 6
#	define GLIMPSE_LOG_DEBUG(fmt,arg...) GLIMPSE_LOG(DEBUG,fmt,##arg)
#else
#	define GLIMPSE_LOG_DEBUG(...)
#endif
#endif
