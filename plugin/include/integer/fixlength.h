/* fixlength.h -   
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
	
#ifndef __FIXLENGTH_H__
#define __FIXLENGTH_H__
const char* glimpse_integer_fixlength_hex_i8(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_hex_u8(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_hex_i16(const char* str, void* ret,void *data, void *thread_data);
const char* glimpse_integer_fixlength_hex_u16(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_hex_i32(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_hex_u32(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_hex_i64(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_hex_u64(const char* str, void* ret, void* data, void *thread_data);

const char* glimpse_integer_fixlength_dec_i8(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_dec_u8(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_dec_i16(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_dec_u16(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_dec_i32(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_dec_u32(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_dec_i64(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_dec_u64(const char* str, void* ret, void* data, void *thread_data);

const char* glimpse_integer_fixlength_oct_i8(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_oct_u8(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_oct_i16(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_oct_u16(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_oct_i32(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_oct_u32(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_oct_i64(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_oct_u64(const char* str, void* ret, void* data, void *thread_data);

const char* glimpse_integer_fixlength_bin_i8(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_bin_u8(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_bin_i16(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_bin_u16(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_bin_i32(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_bin_u32(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_bin_i64(const char* str, void* ret, void* data, void *thread_data);
const char* glimpse_integer_fixlength_bin_u64(const char* str, void* ret, void* data, void *thread_data);

#endif
