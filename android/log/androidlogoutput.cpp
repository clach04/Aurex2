/*
 * androidlogoutput.cpp - Log output class (android system log)
 *
 * $Id$
 *
 * Copyright (C) 2011, 2012 Fumihiko MACHIDA <machida@users.sourceforge.jp>
 * All rights reserved.
 *
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 */


#include <stdio.h>
#include "androidlogoutput.h"

AndroidLogOutput::AndroidLogOutput(void)
{
}


AndroidLogOutput::~AndroidLogOutput(void)
{
}

void AndroidLogOutput::output(char const *buf)
{
	__android_log_write(ANDROID_LOG_INFO, "androaurex", buf);
}