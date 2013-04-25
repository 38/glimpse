/* parser.c -   
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
	
#include <parser.h>
#include <log.h>
/* TODO: how to determin when the scanner stop , use a handler stack , so parser can track back the stack, obtain infomation of ancestors
 * algorithm for termination:
 * there is a set of string that cause the parser stop parsing text, called stop-set.
 * if the parent is sublog, stop-set = {parent's field seprator} + parent's stop-set
 * if the parent is vector, stop-set = {parent's vector seprator} + parent's stop-set
 * for the parser in the bottom of handler stack, stop-set = {"\0"}
 * 
 * A externed version of KMP algorithm is needed for this problem. 
 * The automa should be cached in memory, and hashed in a hash table.
 * The hash function should caculated as the parse function called, 
 * so the time comsuption of hash function is reasonable 
 */
/*const char* glimpse_parser_parse(GlimpseParseTree_t* tree, const char* text, GlimpseDataInstance_t* storage)
{
}*/
