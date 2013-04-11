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
const char* glimpse_integer_fixlength_hex_i8(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int8_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while((*str>='a' && *str<='f') || (*str>='A' && *str<='F') ||
          (*str>='0' && *str<='9'))
    {
        *((int8_t*)ret) = ((*((int8_t*)ret))<<4) + (*str>='a' && *str<='f')?*str-'a'+10:((*str>='A' && *str<='F')?*str-'A'+10:*str-'0');
        str ++;
    }
    if(sign<0) *((int8_t*)ret) = -*((int8_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_hex_u8(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint8_t*)ret) = 0;
    while((*str>='a' && *str<='f') || (*str>='A' && *str<='F') ||
          (*str>='0' && *str<='9'))
    {
        *((uint8_t*)ret) = ((*((uint8_t*)ret))<<4) + (*str>='a' && *str<='f')?*str-'a'+10:((*str>='A' && *str<='F')?*str-'A'+10:*str-'0');
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_hex_i16(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int16_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while((*str>='a' && *str<='f') || (*str>='A' && *str<='F') ||
          (*str>='0' && *str<='9'))
    {
        *((int16_t*)ret) = ((*((int16_t*)ret))<<4) + (*str>='a' && *str<='f')?*str-'a'+10:((*str>='A' && *str<='F')?*str-'A'+10:*str-'0');
        str ++;
    }
    if(sign<0) *((int16_t*)ret) = -*((int16_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_hex_u16(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint16_t*)ret) = 0;
    while((*str>='a' && *str<='f') || (*str>='A' && *str<='F') ||
          (*str>='0' && *str<='9'))
    {
        *((uint16_t*)ret) = ((*((uint16_t*)ret))<<4) + (*str>='a' && *str<='f')?*str-'a'+10:((*str>='A' && *str<='F')?*str-'A'+10:*str-'0');
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_hex_i32(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int32_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while((*str>='a' && *str<='f') || (*str>='A' && *str<='F') ||
          (*str>='0' && *str<='9'))
    {
        *((int32_t*)ret) = ((*((int32_t*)ret))<<4) + (*str>='a' && *str<='f')?*str-'a'+10:((*str>='A' && *str<='F')?*str-'A'+10:*str-'0');
        str ++;
    }
    if(sign<0) *((int32_t*)ret) = -*((int32_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_hex_u32(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint32_t*)ret) = 0;
    while((*str>='a' && *str<='f') || (*str>='A' && *str<='F') ||
          (*str>='0' && *str<='9'))
    {
        *((uint32_t*)ret) = ((*((uint32_t*)ret))<<4) + (*str>='a' && *str<='f')?*str-'a'+10:((*str>='A' && *str<='F')?*str-'A'+10:*str-'0');
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_hex_i64(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int64_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while((*str>='a' && *str<='f') || (*str>='A' && *str<='F') ||
          (*str>='0' && *str<='9'))
    {
        *((int64_t*)ret) = ((*((int64_t*)ret))<<4) + (*str>='a' && *str<='f')?*str-'a'+10:((*str>='A' && *str<='F')?*str-'A'+10:*str-'0');
        str ++;
    }
    if(sign<0) *((int64_t*)ret) = -*((int64_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_hex_u64(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint64_t*)ret) = 0;
    while((*str>='a' && *str<='f') || (*str>='A' && *str<='F') ||
          (*str>='0' && *str<='9'))
    {
        *((uint64_t*)ret) = ((*((uint64_t*)ret))<<4) + (*str>='a' && *str<='f')?*str-'a'+10:((*str>='A' && *str<='F')?*str-'A'+10:*str-'0');
        str ++;
    }
    return str;
}

const char* glimpse_integer_fixlength_dec_i8(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int8_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str>='0' && *str<='9')
    {
        *((int8_t*)ret) = ((*((int8_t*)ret))*10) + *str-'0';
        str ++;
    }
    if(sign<0) *((int8_t*)ret) = -*((int8_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_dec_u8(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint8_t*)ret) = 0;
    while(*str>='0' && *str<='9')
    {
        *((uint8_t*)ret) = ((*((uint8_t*)ret))*10) + *str-'0';
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_dec_i16(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int16_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str>='0' && *str<='9')
    {
        *((int16_t*)ret) = ((*((int16_t*)ret))*10) + *str-'0';
        str ++;
    }
    if(sign<0) *((int16_t*)ret) = -*((int16_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_dec_u16(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint16_t*)ret) = 0;
    while(*str>='0' && *str<='9')
    {
        *((uint16_t*)ret) = ((*((uint16_t*)ret))*10) + *str-'0';
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_dec_i32(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int32_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str>='0' && *str<='9')
    {
        *((int32_t*)ret) = ((*((int32_t*)ret))*10) + *str-'0';
        str ++;
    }
    if(sign<0) *((int32_t*)ret) = -*((int32_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_dec_u32(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint32_t*)ret) = 0;
    while(*str>='0' && *str<='9')
    {
        *((uint32_t*)ret) = ((*((uint32_t*)ret))*10) + *str-'0';
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_dec_i64(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int64_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str>='0' && *str<='9')
    {
        *((int64_t*)ret) = ((*((int64_t*)ret))*10) + *str-'0';
        str ++;
    }
    if(sign<0) *((int64_t*)ret) = -*((int64_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_dec_u64(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint64_t*)ret) = 0;
    while(*str>='0' && *str<='9')
    {
        *((uint64_t*)ret) = ((*((uint64_t*)ret))*10) + *str-'0';
        str ++;
    }
    return str;
}

const char* glimpse_integer_fixlength_oct_i8(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int8_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str>='0' && *str<='7')
    {
        *((int8_t*)ret) = ((*((int8_t*)ret))<<3) + *str-'0';
        str ++;
    }
    if(sign<0) *((int8_t*)ret) = -*((int8_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_oct_u8(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint8_t*)ret) = 0;
    while(*str>='0' && *str<='7')
    {
        *((uint8_t*)ret) = ((*((uint8_t*)ret))<<3) + *str-'0';
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_oct_i16(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int16_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str>='0' && *str<='7')
    {
        *((int16_t*)ret) = ((*((int16_t*)ret))<<3) + *str-'0';
        str ++;
    }
    if(sign<0) *((int16_t*)ret) = -*((int16_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_oct_u16(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint16_t*)ret) = 0;
    while(*str>='0' && *str<='7')
    {
        *((uint16_t*)ret) = ((*((uint16_t*)ret))<<3) + *str-'0';
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_oct_i32(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int32_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str>='0' && *str<='7')
    {
        *((int32_t*)ret) = ((*((int32_t*)ret))<<3) + *str-'0';
        str ++;
    }
    if(sign<0) *((int32_t*)ret) = -*((int32_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_oct_u32(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint32_t*)ret) = 0;
    while(*str>='0' && *str<='7')
    {
        *((uint32_t*)ret) = ((*((uint32_t*)ret))<<3) + *str-'0';
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_oct_i64(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int64_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str>='0' && *str<='7')
    {
        *((int64_t*)ret) = ((*((int64_t*)ret))<<3) + *str-'0';
        str ++;
    }
    if(sign<0) *((int64_t*)ret) = -*((int64_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_oct_u64(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint64_t*)ret) = 0;
    while(*str>='0' && *str<='7')
    {
        *((uint64_t*)ret) = ((*((uint64_t*)ret))<<3) + *str-'0';
        str ++;
    }
    return str;
}

const char* glimpse_integer_fixlength_bin_i8(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int8_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str=='0' || *str=='1')
    {
        *((int8_t*)ret) = ((*((int8_t*)ret))<<1) + *str-'0';
        str ++;
    }
    if(sign<0) *((int8_t*)ret) = -*((int8_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_bin_u8(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint8_t*)ret) = 0;
    while(*str=='0' || *str=='1')
    {
        *((uint8_t*)ret) = ((*((uint8_t*)ret))<<1) + *str-'0';
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_bin_i16(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int16_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str=='0' || *str=='1')
    {
        *((int16_t*)ret) = ((*((int16_t*)ret))<<1) + *str-'0';
        str ++;
    }
    if(sign<0) *((int16_t*)ret) = -*((int16_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_bin_u16(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint16_t*)ret) = 0;
    while(*str=='0' || *str=='1')
    {
        *((uint16_t*)ret) = ((*((uint16_t*)ret))<<1) + *str-'0';
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_bin_i32(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int32_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str=='0' || *str=='1')
    {
        *((int32_t*)ret) = ((*((int32_t*)ret))<<1) + *str-'0';
        str ++;
    }
    if(sign<0) *((int32_t*)ret) = -*((int32_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_bin_u32(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint32_t*)ret) = 0;
    while(*str=='0' || *str=='1')
    {
        *((uint32_t*)ret) = ((*((uint32_t*)ret))<<1) + *str-'0';
        str ++;
    }
    return str;
}
const char* glimpse_integer_fixlength_bin_i64(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    int sign = 1;
    *((int64_t*)ret) = 0;
    while(*str != 0 && *str == '-')
    {
        sign = -sign;
        str++;
    }
    while(*str=='0' || *str=='1')
    {
        *((int64_t*)ret) = ((*((int64_t*)ret))<<1) + *str-'0';
        str ++;
    }
    if(sign<0) *((int64_t*)ret) = -*((int64_t*)ret);
    return str;
}
const char* glimpse_integer_fixlength_bin_u64(const char* str, void* ret, void* data)
{
	if(NULL == (str = _glimpse_integer_fixlength_parse_header(str, (GlimpseIntegerProperties_t*) data))) return NULL;
    *((uint64_t*)ret) = 0;
    while(*str=='0' || *str=='1')
    {
        *((uint64_t*)ret) = ((*((uint64_t*)ret))<<1) + *str-'0';
        str ++;
    }
    return str;
}
