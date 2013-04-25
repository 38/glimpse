/* retval.h - error values 
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
	
#ifndef __GLIMPSE_RETVAL_H__
#define __GLIMPSE_RETVAL_H__
#include <glimpse/future.h>
#include <glimpse/def.h>
#define GLIMPSE_ESUCCESS 0
#define GLIMPSE_EINVAILDARG -1
#define GLIMPSE_ETOOMANYAPI -2
#define GLIMPSE_ETOOMANYTG -2
#define GLIMPSE_EMEMORYLIMIT -2
#define GLIMPSE_ENOTFOUND -3
#define GLIMPSE_ESYMNOTFOUND -4
#define GLIMPSE_EUNKNOWN -5
#define GLIMPSE_EMALFORMEDAPI -6
#define GLIMPSE_EINVALIDVERCODE -7
#define GLIMPSE_EDEPEND -8
#define GLIMPSE_EVERSION -9
#define GLIMPSE_EMAILFORMEDTG -10
#define GLIMPSE_ENOSYMBOL -11
#define GLIMPSE_EDUPSYMBOL -12
#define GLIMPSE_ELISTFULL -13
#endif
