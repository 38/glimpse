/* integer.h -   
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
	
#ifndef __INTEGER_H__
#define __INTEGER_H__
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif
/* Define the properties of Integer type group */
typedef struct _glimpse_integer_properties{
	/*Describe the size of Integer */
	enum{
		GlimpseInteger8,	/* 8 bit */
		GlimpseInteger16,	/* 16 bit */
		GlimpseInteger32,	/* 32 bit */
		GlimpseInteger64,	/* 64 bit */
		GlimpseIntegerVariant, /* Big Integer */
		/* you should add some new value for this property here */
		GlimpseIntegerSizeCount /* how many possible values */
		/* DO NOT add any element here */
	} Size:7;
	/* Incidates wether the integer is signed */
	int Signed:1;
	/* The format of representation */
	enum{
		GlimpseIntegerDec,	/* Decimal */
		GlimpseIntegerBin,	/* Binary */
		GlimpseIntegerOct,	/* Octal */
		GlimpseIntegerHex,	/* Hexadecimal */
		/* you should add some new value for this property here */
		GlimpseIntegerRepresentationCount
		/* DO NOT add any element here */
	} Representation:8;
	/* Indicates the suffix of the integer, e.g. 0x1234, leading = 0x */
	char* Leading;
	/* Indicates the string of signed integer which located between the sign and the value part */
	char* LeadingAfterSign;
} GlimpseIntegerProperties_t;
#ifdef __cplusplus
}
#endif
#endif
