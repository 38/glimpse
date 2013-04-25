/* fixlength.c -   
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
	
#include <integer/integer.h>
#include <integer/fixlength.h>
#include <stdint.h>
#include <stdlib.h>
static inline const char* _glimpse_integer_fixlength_parse_header(const char* str, GlimpseIntegerProperties_t* properties)
{
	const char *p;
	const char *q;
	if(NULL == properties->Leading) return str;
	for(p = properties->Leading, q = str; *p && *q && *p == *q; p ++, q ++);
	if(0 == *p) return q;
	return NULL;
}
static inline const char* _glimpse_integer_fixlength_parse_header_after_sign(const char* str, GlimpseIntegerProperties_t* properties)
{
	const char *p;
	const char *q;
	if(NULL == properties->LeadingAfterSign) return str;
	for(p = properties->LeadingAfterSign, q = str; *p && *q && *p == *q; p ++, q ++);
	if(0 == *p) return q;
	return NULL;
}
/* Hex Parser */
#define SIGNED_HEX_PARSER(len) \
const char* glimpse_integer_fixlength_hex_i##len(const char* str, void* ret, void* data, void* thread_data)\
{\
	const char* tmp;\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
    int sign = 1;\
    *((int##len##_t*)ret) = 0;\
    while(*str != 0 && *str == '-')\
    {\
        sign = -sign;\
        str++;\
    }\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header_after_sign(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
    while((*str>='a' && *str<='f') || (*str>='A' && *str<='F') ||\
          (*str>='0' && *str<='9'))\
    {\
        *((int##len##_t*)ret) = ((*((int##len##_t*)ret))<<4) + ((*str>='a' && *str<='f')?*str-'a'+10:((*str>='A' && *str<='F')?*str-'A'+10:*str-'0'));\
        str ++;\
    }\
    if(sign<0) *((int##len##_t*)ret) = -*((int##len##_t*)ret);\
    return str;\
}
#define USIGNED_HEX_PARSER(len) \
const char* glimpse_integer_fixlength_hex_u##len(const char* str, void* ret, void* data, void* thread_data)\
{\
	const char* tmp;\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
    *((uint##len##_t*)ret) = 0;\
    while((*str>='a' && *str<='f') || (*str>='A' && *str<='F') ||\
          (*str>='0' && *str<='9'))\
    {\
        *((uint##len##_t*)ret) = ((*((uint##len##_t*)ret))<<4) + ((*str>='a' && *str<='f')?*str-'a'+10:((*str>='A' && *str<='F')?*str-'A'+10:*str-'0'));\
        str ++;\
    }\
    return str;\
}

#define HEX_PARSER(len) \
	SIGNED_HEX_PARSER(len)\
	USIGNED_HEX_PARSER(len)

/* dec parser */
#define SIGNED_DEC_PARSER(len) \
const char* glimpse_integer_fixlength_dec_i##len(const char* str, void* ret, void* data, void* thread_data)\
{\
	const char* tmp;\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
	int sign = 1;\
    *((int##len##_t*)ret) = 0;\
    while(*str != 0 && *str == '-')\
    {\
        sign = -sign;\
        str++;\
    }\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header_after_sign(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
    while(*str>='0' && *str<='9')\
    {\
        *((int##len##_t*)ret) = ((*((int##len##_t*)ret))*10) + *str-'0';\
        str ++;\
    }\
    if(sign<0) *((int##len##_t*)ret) = -*((int##len##_t*)ret);\
    return str;\
}
#define UNSIGNED_DEC_PARSER(len) \
const char* glimpse_integer_fixlength_dec_u##len(const char* str, void* ret, void* data, void* thread_data)\
{\
	const char* tmp;\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
    *((uint##len##_t*)ret) = 0;\
    while(*str>='0' && *str<='9')\
    {\
        *((uint##len##_t*)ret) = ((*((uint##len##_t*)ret))*10) + *str-'0';\
        str ++;\
    }\
    return str;\
}
#define DEC_PARSER(len)\
	UNSIGNED_DEC_PARSER(len)\
	SIGNED_DEC_PARSER(len)

/* oct parser */
#define SIGNED_OCT_PARSER(len)\
const char* glimpse_integer_fixlength_oct_i##len(const char* str, void* ret, void* data, void* thread_data)\
{\
	const char* tmp;\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
    int sign = 1;\
    *((int##len##_t*)ret) = 0;\
    while(*str != 0 && *str == '-')\
    {\
        sign = -sign;\
        str++;\
    }\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header_after_sign(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
    while(*str>='0' && *str<='7')\
    {\
        *((int##len##_t*)ret) = ((*((int##len##_t*)ret))<<3) + *str-'0';\
        str ++;\
    }\
    if(sign<0) *((int##len##_t*)ret) = -*((int##len##_t*)ret);\
    return str;\
}
#define UNSIGNED_OCT_PARSER(len)\
const char* glimpse_integer_fixlength_oct_u##len(const char* str, void* ret, void* data, void* thread_data)\
{\
	const char* tmp;\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
    *((uint##len##_t*)ret) = 0;\
    while(*str>='0' && *str<='7')\
    {\
        *((uint##len##_t*)ret) = ((*((uint##len##_t*)ret))<<3) + *str-'0';\
        str ++;\
    }\
    return str;\
}
#define OCT_PARSER(len)\
	UNSIGNED_OCT_PARSER(len)\
	SIGNED_OCT_PARSER(len)

/* Bin parser */
#define SIGNED_BIN_PARSER(len) \
const char* glimpse_integer_fixlength_bin_i##len(const char* str, void* ret, void* data, void* thread_data)\
{\
	const char* tmp;\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
    int sign = 1;\
    *((int##len##_t*)ret) = 0;\
    while(*str != 0 && *str == '-')\
    {\
        sign = -sign;\
        str++;\
    }\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header_after_sign(str, (GlimpseIntegerProperties_t*) data))) return str;\
	str = tmp;\
    while(*str=='0' || *str=='1')\
    {\
        *((int##len##_t*)ret) = ((*((int##len##_t*)ret))<<1) + *str-'0';\
        str ++;\
    }\
    if(sign<0) *((int##len##_t*)ret) = -*((int##len##_t*)ret);\
    return str;\
}
#define UNSIGNED_BIN_PARSER(len) \
const char* glimpse_integer_fixlength_bin_u##len(const char* str, void* ret, void* data, void* thread_data)\
{\
	const char* tmp;\
	if(NULL == (tmp = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;\
	str = tmp;\
    *((uint##len##_t*)ret) = 0;\
    while(*str=='0' || *str=='1')\
    {\
        *((uint##len##_t*)ret) = ((*((uint##len##_t*)ret))<<1) + *str-'0';\
        str ++;\
    }\
    return str;\
}
#define BIN_PARSER(len) \
	UNSIGNED_BIN_PARSER(len)\
	SIGNED_BIN_PARSER(len)
HEX_PARSER(8);
HEX_PARSER(16);
HEX_PARSER(32);
HEX_PARSER(64);
DEC_PARSER(8);
DEC_PARSER(16);
DEC_PARSER(32);
DEC_PARSER(64);
OCT_PARSER(8);
OCT_PARSER(16);
OCT_PARSER(32);
OCT_PARSER(64);
BIN_PARSER(8);
BIN_PARSER(16);
BIN_PARSER(32);
BIN_PARSER(64);
