/*
 * ufilerini.cpp - user defined filer.ini (aurex.ini ?)
 *
 * $Id: ufilerini.cpp,v 1.6 2005/07/27 18:53:28 fumi Exp $
 *
 * Copyright (C) 2004 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
#include <string.h>
#include <ctype.h>
#include "ufilerini.h"

/** UFilerIni
 * filer.ini �����ؤ���Ϥ��륯�饹
 * �������ؤ���ե�����Υե����ޥåȤϰʲ��Τ褦�ˤʤäƤ���
 * ��԰�쥳���ɤǡ���������ɽ���ǽ񤯤ȡ�
 *   ^\*\.ext[:space:]+/Ruputer/Exf/File/Name$
 *
 * ex.
 * |*.txt \foo\bar.exf
 * |*.adr \foo\hoge.exf
 */

/** ���󥹥ȥ饯��
 */
UFilerIni::UFilerIni(void)
{
// do nothing
}


/** �ǥ��ȥ饯��
 */
UFilerIni::~UFilerIni(void)
{
}


/** ����
 * @param filename ���Ϥ���ե�����̾
 */
void UFilerIni::load(char const *filename)
{
	FILE *fp;

	fp=fopen(filename, "r");
	if(fp == NULL)
		return;

	char buf[1024];
	while(fgets(buf, sizeof(buf), fp) != NULL)
	{
		if(*buf == '#')
			continue;

		if(*buf == '*' && *(buf+1) == '.')
		{
			char *space;
			space=strchr(buf, ' ');
			if(space == NULL)
				continue;

			if(space-buf == 5)
			{
				while(isspace(*space))
					*space++=0;
				char *tail;
				tail=strchr(space, 0);
				tail--;
				while(tail >= space && (*tail == 0x0a || *tail == 0x0d))
					*tail--=0;

				add(buf+2, space);
			}
		}
	}

	fclose(fp);
}
