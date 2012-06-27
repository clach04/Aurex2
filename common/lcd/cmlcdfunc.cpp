/*
 * cmlcdfunc.cpp - common drawing module for 
 *
 * $Id: cmlcdfunc.cpp,v 1.5 2006/01/22 15:35:28 fumi Exp $
 *
 * Copyright (C) 2004, 2005, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cmlcdpix.h"
#include "cmlcdfunc.h"
#include "memory.h"

#include "option.h"


/** ����LCD����롼���󥳥󥹥ȥ饯��
 */
CMLcdFunc::CMLcdFunc(Ruputer *ruputer, int expansion)
{
	this->expansion = expansion;
	this->ruputer = ruputer;
	this->pix = NULL;
}

/** ����LCD����롼���� �ǥ��ȥ饯��
 */
CMLcdFunc::~CMLcdFunc(void)
{
}

/*
 */
CMLcdPix *CMLcdFunc::getCMLcdPix(void)
{
	return pix;
}


/** �ԥ�����λıǽ���
 */
void CMLcdFunc::updatePixel(int x, int y, enum UpdateRegion reg)
{
	pix->updatePixel(x, y, reg);
}


/** LCD���������Ť����ꤹ���public��
 * @param x x ��ɸ
 * @param y y ��ɸ
 * @param color 0:��/1:��
 * @param logic  gv_line �Ϥ� logic �ϲ��� 2 bit�˳�Ǽ���졢
 * gv_kput �Ϥ� logic �� 2bit �����եȤ��Ƴ�Ǽ�����
 */
void CMLcdFunc::setPoint(int x, int y, int color, int logic)
{
	int c;

	switch(logic)
	{
	case 0:	/* ��� */
		pix->pset(x, y, color);
		break;
	case 1: /* xor */
		c = pix->pget(x, y);
		c=c ^ color;
		pix->pset(x, y, c);
		break;
	case 4: /* string or */
	case 2:	/* or */
		c = pix->pget(x, y);
		c=c | color;
		pix->pset(x, y, c);
		break;
	case 3:	/* and */
		c = pix->pget(x, y);
		c=c & color;
		pix->pset(x, y, c);
		break;

	case 8: /* ȿž��� */
		color = color ^ 1;
		pix->pset(x, y, color);
		break;

	case 0xc: /* ȿž OR */
		c = pix->pget(x, y);
		c = c | ( color ^ 1 );
		pix->pset(x, y, c);
		break;
	}
}

int CMLcdFunc::getPoint(int x, int y)
{
	return pix->pget( x, y );
}


/** MMP�ǡ����μ���
 * @param x1 x1 ��ɸ
 * @param y1 y1 ��ɸ
 * @param x2 x2 ��ɸ
 * @param y2 y2 ��ɸ
 * @param buf �����ǡ�������Ƭ�� width, height ���դ���
 */
void CMLcdFunc::getMMPData(int x1, int y1, int x2, int y2, unsigned char *buf)
{
	int width, height;
	int x, y;
	int c, i;

	MemoryForInside *mem;

	mem=new MemoryForInside(buf);

	width=abs(x2-x1)+1;
	height=abs(y2-y1)+1;

	mem->write16(0, width);
	mem->write16(2, height);

#ifdef DEBUG
	fprintf(stderr, "!! getMMP (%d, %d)-(%d, %d), %d, %d %p\n", x1, y1, x2, y2, width, height, buf);
	fflush(stderr);
#endif

	buf+=4;
	for(y=0; y < height; y+=8)
	{
		for(x=0; x < width; x++, buf++)
		{
			*buf=0;
			for(i=0; i < 8 && y+i < height; i++)
			{
				c = pix->pget(x1 + x, y1 + y + i);
				if(c)
					*buf|=1 << i;
			}
		}
	}

	delete mem;
}

/** MMP�ǡ�����ɽ��
 * @param start_x x ��ɸ
 * @param start_y y ��ɸ
 * @param buf ɽ���ǡ�������Ƭ�� width, height ���դ��Ƥ��뤳�ȡ�
 * @param logic �����黻�λ���
 */
void CMLcdFunc::putMMPData(int start_x, int start_y, unsigned char *buf, int logic)
{
	int width, height;
	MemoryForInside *mem;


	mem=new MemoryForInside(buf);

	width=mem->read16_s(0);
	height=mem->read16_s(2);

#ifdef DEBUG
	fprintf(stderr, "!! putMMP (%d, %d), %d, %d %p\n", start_x, start_y, width, height, buf);
	fflush(stderr);
#endif

	putMMPDataWithoutWH(start_x, start_y, width, height, buf+4, logic);

	delete mem;
}

/** MMP�ǡ�����ɽ���ʥХåե��� width, height ��̵�����ȡ�
 * @param start_x x ��ɸ
 * @param start_y y ��ɸ
 * @param width ��
 * @param height �⤵
 * @param buf ɽ���ǡ�������Ƭ�� width, height ���դ��Ƥ��ʤ����ȡ�
 * @param logic �����黻�λ���
 */
void CMLcdFunc::putMMPDataWithoutWH(int start_x, int start_y, int width, int height, unsigned char *buf, int logic)
{
	int x, y;
	int i;
	int end_x;

	end_x=start_x+width;
	for(y=0; y < height; y+=8)
	{
		x=start_x;
		for(; x < end_x; x++, buf++)
		{
			for(i=0; i < 8 && y+i < height; i++)
				setPoint(x, start_y+y+i, (*buf >> i) & 1, logic);
		}
	}
}



/** ʸ�����������������ºݤ������ drawStringSub ��ƤӽФ���������
 * ����ñ�ˡ�Latin ���б����ȹͤ����ɤ��Ǥ���
 *
 * @param x x ��ɸ��ʸ���κ���κ�ɸ��
 * @param y y ��ɸ
 * @param fontwidth ʸ����
 * @param fontheight ʸ����
 * @param str ʸ����ؤΥݥ���
 * @param prop_flag �ץ�ݡ�����ʥ�ե���Ȥ��ݤ���̤���ѡ�
 * @param gap ʸ���֤Υ���åסʥɥåȡ�
 * @param logic �����������
 */
void CMLcdFunc::drawString(int x, int y, int fontwidth, int fontheight, char const *str, int prop_flag, int gap, int logic)
{
	drawStringSub(x, y, fontwidth, fontheight, str, prop_flag, gap, logic, ::option->get_useIso8859_1() );
}
