#ifndef __INTEGER_H__
#define __INTEGER_H__
/* Define the properties of Integer type group */
typedef struct _glimpse_integer_properties{
	/*Describe the size of Integer */
	enum{
		GlimpseInteger8,	/* 8 bit */
		GlimpseInteger16,	/* 16 bit */
		GlimpseInteger32,	/* 32 bit */
		GlimpseInteger64,	/* 64 bit */
		GlimpseIntegerVariant /* Big Integer */
	} Size:7;
	/* Incidates wether the integer is signed */
	int Signed:1;
	/* The format of representation */
	enum{
		GlimpseIntegerDec,	/* Decimal */
		GlimpseIntegerBin,	/* Binary */
		GlimpseIntegerOct,	/* Octal */
		GlimpseIntegerHex,	/* Hexadecimal */
		GlimpseIntegerIP	/* IP Address like 127.0.0.1 */
	} Representation:8;
	/* Indicates the suffix of the integer, e.g. 0x1234, leading = 0x */
	char* Leading;
} GlimpseIntegerProperties_t;
#endif
