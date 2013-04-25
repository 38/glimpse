/* alias.c -   
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
	
#include <TypeAPI.h>
void alias_types()
{
	Alias("integer{size:32, signed:true, format:dec}","int32d");
	Alias("integer{size:32, signed:false, format:dec}","uint32d");
	Alias("integer{size:32, signed:true, format:hex}","int32h");
	Alias("integer{size:32, signed:false, format:hex}","uint32h");

	Alias("integer{size:64, signed:true, format:dec}","int64d");
	Alias("integer{size:64, signed:false, format:dec}","uint64d");
	Alias("integer{size:64, signed:true, format:hex}","int64h");
	Alias("integer{size:64, signed:false, format:hex}","uint64h");
}
