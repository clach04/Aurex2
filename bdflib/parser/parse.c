/*
 * parse.c - To parse bdf font. (to call function bison generated)
 *
 * $Id: parse.c,v 1.4 2004/05/07 08:25:03 fumi Exp $
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

#include "bdffontset.h"


static int convert(unsigned char *buf, unsigned char *out, int width, int height);
static unsigned char convert_2byte(unsigned char a, unsigned char b);


void yyparse(void);

static BdfFontSet *global_bdffontset;
static int global_height;


extern FILE *yyin;

/** BDF�ե�����ѡ����ؿ�
 * @param bdffontset ������Хå��˻��Ѥ��롢�����Υ��饹
 * @param filename BDF�ե�����̾
 * @param height BDF�ե���Ȥι⤵(pixel)��BDF�ե���Ȥι⤵�Ȱ��פ��Ƥ��ʤ����ϡ��ڼΤơ�0­����
 * @retval -1 ����
 * @retval 0 ����
 */
int c_parse(BdfFontSet *bdffontset, char const *filename, int height)
{
	global_bdffontset=bdffontset;
	global_height=height;

	yyin=fopen(filename, "rb");
	if(yyin == NULL)
		return -1;

	global_bdffontset=bdffontset;

	yyparse();

	fclose(yyin);

	return 0;
}
/** �ѡ�������ƤФ��ؿ�
 * @param code ʸ��������(JIS)
 * @param width ��
 * @param buf bdf ��ʸ���Υӥå�
 * @retval �ʤ�
 */
void kakuno(unsigned short code, int width, char *buf)
{
	unsigned char out[256]="";
	int len;

	len=convert((unsigned char *)buf, out, width, global_height);
	global_bdffontset->add(code, width, out, len);
}

/** bdf ��ʸ���ΥӥåȤ��Ѵ���Ԥʤ�
 * @param buf ���ϡʥǥ�ߥ��ϲ���(0x0a)��
 * @param out ����
 * @param width ��
 * @param height �⤵
 * @retval int out�λ��Ѥ���Ĺ��
 */
static int convert(unsigned char *buf, unsigned char *out, int width, int height)
{
	int i;
	int byte_per_line=(width+7)/8;

	for(i=0; i < height && *buf;)
	{
		if(*buf == 0x0a)
		{
			buf++;
			i++;
		}
		else if(*(buf+1) == 0x0a)
		{
			*out=convert_2byte(*buf, '0');
			out++;
			buf+=2;
			i++;
		}
		else
		{
			*out=convert_2byte(*buf, *(buf+1));
			out++;
			buf+=2;
		}
	}

	return byte_per_line*height;
}

/** 16�ʿ�2ʸ���� unsined char ���Ѵ�����
 * @param a ��� 4bit��ʸ����
 * @param b ���� 4bit��ʸ����
 * @retval �Ѵ�����8bit
 */
static unsigned char convert_2byte(unsigned char a, unsigned char b)
{
	char temp[4];
	int i, val;

	temp[0]=a;
	temp[1]=b;
	temp[2]=0;
	return (unsigned char)strtoul(temp, NULL, 16);
}

/*
int main(void)
{
	yyparse();
}

*/

