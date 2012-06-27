/*
 * cmlcdfuncdefault.cpp - common drawing module for Lcd
 *
 * $Id: cmlcdfuncdefault.cpp,v 1.7 2006/01/21 18:04:34 fumi Exp $
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
#include <stdlib.h>
#include <string.h>

#include "cmlcdfunc.h"
#include "cmlcdfuncdefault.h"

#include "cmlcdpix.h"
#include "cmlcdpixdefault.h"
#include "cmlcdpixdefault_noai.h"

#include "sdlcd.h"
#include "sdlcdfont.h"
#include "ruputer.h"
#include "memory.h"



/** ����LCD����롼���󥳥󥹥ȥ饯��
 */
CMLcdFuncDefault::CMLcdFuncDefault(Ruputer *ruputer, int expansion, bool after_image, int ai_value) : CMLcdFunc(ruputer, expansion)
{
	if( after_image )
		pix = new CMLcdPixDefault(expansion, ai_value);
	else
		pix = new CMLcdPixDefault_Noai(expansion);
}

/** ����LCD����롼���� �ǥ��ȥ饯��
 */
CMLcdFuncDefault::~CMLcdFuncDefault(void)
{
	delete pix;
}

/** �������
 * @param x1 x1 ��ɸ
 * @param y1 y1 ��ɸ
 * @param x2 x2 ��ɸ
 * @param y2 y2 ��ɸ
 * @param style 8bit ��������ɽ����0xff ��*����*����
 * @param logic �����������
 */
void CMLcdFuncDefault::drawLine(int x1, int y1, int x2, int y2, unsigned char ucstyle, int logic)
{
	unsigned long style;

	style=((unsigned long)ucstyle << 24)
		| ((unsigned long)ucstyle << 16)
		| ((unsigned long)ucstyle <<  8)
		|  (unsigned long)ucstyle;

	drawLine(x1, y1, x2, y2, style, logic);
}

/** ������� style unsinged long ��
 * @param x1 x1 ��ɸ
 * @param y1 y1 ��ɸ
 * @param x2 x2 ��ɸ
 * @param y2 y2 ��ɸ
 * @param style 32bit ��������ɽ����0xffffffffUL ��*����*����
 * @param logic �����������
 */
void CMLcdFuncDefault::drawLine(int x1, int y1, int x2, int y2, unsigned long style, int logic)
{
	int i;
	int x, y;
	int dx, dy;
	int sx, sy;
	int a, e;
	int color[32];

#ifdef DEBUG
	fprintf(stderr, "drawLine (%d, %d)-(%d, %d), styel %x, logic %x\n", x1, y1, x2, y2, style, logic);
#endif

	for(i=0; i < 32; i++, style>>=1)
		color[i]=style & 1;

/* �����ʥ� int */
#define sgni(x) ((x) > 0 ? 1 : -1)

	dx=abs(x2-x1);
	dy=abs(y2-y1);
	sx=sgni(x2-x1);
	sy=sgni(y2-y1);

	if(dx > dy)
	{
		x=x1;
		y=y1;
		e=-dx;
		a=2*dy;
		for(i=0; i <= dx; i++)
		{
			setPoint(x, y, color[i & 0x1f], logic);

			e+=a;
			if(e >= 0)
			{
				y+=sy;
				e-=2*dx;
			}

			x+=sx;
		}
	}
	else
	{
		x=x1;
		y=y1;
		e=-dy;
		a=2*dx;

		for(i=0; i <= dy; i++)
		{
			setPoint(x, y, color[i & 0x1f], logic);

			e+=a;
			if(e >= 0)
			{
				x+=sx;
				e-=2*dy;
			}

			y+=sy;
		}
	}
}


/** �ͳѤ�����
 * @param x1 x1 ��ɸ
 * @param y1 y1 ��ɸ
 * @param x2 x2 ��ɸ
 * @param y2 y2 ��ɸ
 * @param style 8bit ��������ɽ���Ǥ��롣0xff ��*����*����
 * @param logic �����������
 */
void CMLcdFuncDefault::drawRect(int x1, int y1, int x2, int y2, unsigned char style, int logic)
{
	unsigned long ulstyle;

	ulstyle=((unsigned long)style << 24) | ((unsigned long)style << 16) | ((unsigned long)style << 8) | style;

	drawLine(x1, y1, x2, y1, ulstyle, logic);
	drawLine(x2, y1, x2, y2, ulstyle, logic);
	drawLine(x2, y2, x1, y2, ulstyle, logic);
	drawLine(x1, y2, x1, y1, ulstyle, logic);
}

/** �ɤ��٤��ͳѤ�����
 * @param x1 x1 ��ɸ
 * @param y1 y1 ��ɸ
 * @param x2 x2 ��ɸ
 * @param y2 y2 ��ɸ
 * @param style 32bit ��
 * @param logic �����������
 */
void CMLcdFuncDefault::fillRect(int x1, int y1, int x2, int y2, unsigned long style, int logic)
{
	int ymin, ymax;
	int y;

	ymin=min(y1, y2);
	ymax=max(y1, y2);
	for(y=ymin; y < ymax; y++)
		drawLine(x1, y, x2, y, style, logic);
}

/** ʸ��������
 * @param x x ��ɸ��ʸ���κ���κ�ɸ��
 * @param y y ��ɸ
 * @param fontwidth ʸ����
 * @param fontheight ʸ����
 * @param str ʸ����ؤΥݥ���
 * @param prop_flag �ץ�ݡ�����ʥ�ե���Ȥ��ݤ���̤���ѡ�
 * @param gap ʸ���֤Υ���åסʥɥåȡ�
 * @param logic �����������
 */
void CMLcdFuncDefault::drawStringSub(int x, int y, int fontwidth, int fontheight, char const *str, int prop_flag, int gap, int logic, int latin)
{
	SDLcdFont *font;
	int width;
	unsigned char **p;

	SDLcd *sdlcd = ruputer->getSDLcd();

	font = sdlcd->createSDLcdFont(fontwidth, fontheight, prop_flag, gap, latin);
	if(font == NULL)
		return;

	p=font->drawString(str, &width);
	if(p == NULL)
	{
		delete font;
		return;
	}

#ifdef DEBUG
	fprintf(stderr, "\
-------------- CMLcdFuncDefault::drawString ---------------------------\n\
 x: %d\n\
 y: %d\n\
 width: %d\n\
 fwidth:  %d\n\
 fheight: %d\n\
 logic: %x\n\
-------------------------------------------------------------\n",
			x, y, width, fontwidth, fontheight, logic);
	fflush(stderr);
#endif

	subDrawString(p, x, y, width, fontheight, logic);
	font->freeBuffer(p);

	delete font;
}


/** ʸ���������󥰤����Хåե����顢LCD�Хåե��إ��ԡ�����
 * @param buf 1ʸ���������󥰤����Хåե�
 * @param x x ��ɸ��ʸ���κ���κ�ɸ��
 * @param y y ��ɸ
 * @param w ���褷����
 * @param h ʸ����
 * @param logic �����������
 */
void CMLcdFuncDefault::subDrawString(unsigned char **buf, int x, int y, int w, int h, int logic)
{
	int i, j;

/*
	fprintf(stderr, "!! cmlcd subdrawString %p (%d, %d), %d, %d, %d", buf, x, y, w, h, logic);
	fflush(stderr);
*/
	int logic2;

	logic2=logic << 2;

	for(i=0; i < h; i++)
	{
		for(j=0; j < w; j++)
		{
			if(buf[i][j] == 0)
				setPoint(x+j, y+i, 1, logic2);
			else
				setPoint(x+j, y+i, 0, logic2);
		}
	}
}

/** ȿž
 * @param x1 x1 ��ɸ
 * @param y1 y1 ��ɸ
 * @param x2 x2 ��ɸ
 * @param y2 y2 ��ɸ
 */
void CMLcdFuncDefault::reverseRect(int x1, int y1, int x2, int y2)
{
	int i, j;
	int dx, dy;
	int x, y;

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

	dx=abs(x1-x2);
	dy=abs(y1-y2);
	x=min(x1, x2);
	y=min(y1, y2);

	for(i=0; i <= dy; i++)
	{
		for(j=0; j <= dx; j++)
		{
			pix->pset(x + j, y + i, pix->pget(x + j, y + i) ^ 1);
		}
	}
}

/** �õ�
 * @param x1 x1 ��ɸ
 * @param y1 y1 ��ɸ
 * @param x2 x2 ��ɸ
 * @param y2 y2 ��ɸ
 */
void CMLcdFuncDefault::clearRect(int x1, int y1, int x2, int y2)
{
	int i, j;
	int dx, dy;
	int x, y;

	dx=abs(x1-x2);
	dy=abs(y1-y2);
	x=min(x1, x2);
	y=min(y1, y2);

	for(i=0; i <= dy; i++)
	{
		for(j=0; j <= dx; j++)
		{
			pix->pset(x + j, y + i, 0);
		}
	}
}

/** �ʱߤ�����
 * ��¼��ɧ�����أø���ˤ��ǿ����르�ꥺ���ŵ�٤��
 * ��¼�������꤬�Ȥ��������ޤ���
 *
 */
void CMLcdFuncDefault::drawEllipse(int xc, int yc, int rx, int ry, int logic)
{
	int x, x1, y, y1, r;

	if (rx > ry) {
		x = r = rx;  y = 0;
		while (x >= y) {
			x1 = (int)((long)x * ry / rx);
			y1 = (int)((long)y * ry / rx);
			setPoint(xc + x, yc + y1, 1, logic);
			setPoint(xc + x, yc - y1, 1, logic);
			setPoint(xc - x, yc + y1, 1, logic);
			setPoint(xc - x, yc - y1, 1, logic);
			setPoint(xc + y, yc + x1, 1, logic);
			setPoint(xc + y, yc - x1, 1, logic);
			setPoint(xc - y, yc + x1, 1, logic);
			setPoint(xc - y, yc - x1, 1, logic);
			if ((r -= (y++ << 1) - 1) < 0)
				r += (x-- - 1) << 1;
		}
	} else {
		x = r = ry;  y = 0;
		while (x >= y) {
			x1 = (int)((long)x * rx / ry);
			y1 = (int)((long)y * rx / ry);
			setPoint(xc + x1, yc + y, 1, logic);
			setPoint(xc + x1, yc - y, 1, logic);
			setPoint(xc - x1, yc + y, 1, logic);
			setPoint(xc - x1, yc - y, 1, logic);
			setPoint(xc + y1, yc + x, 1, logic);
			setPoint(xc + y1, yc - x, 1, logic);
			setPoint(xc - y1, yc + x, 1, logic);
			setPoint(xc - y1, yc - x, 1, logic);
			if ((r -= (y++ << 1) - 1) < 0)
				r += (x-- - 1) << 1;
		}
	}
}

unsigned char const *CMLcdFuncDefault::getPixelArray(int x, int y)
{
	return pix->getPixelArray(x, y);
}
