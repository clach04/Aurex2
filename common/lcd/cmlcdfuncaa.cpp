/*
 * cmlcdfuncaa.cpp - common drawing module for Lcd
 *
 * $Id: cmlcdfuncaa.cpp,v 1.8 2006/02/19 13:04:39 fumi Exp $
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
#include <assert.h>

#include "cmlcdfunc.h"
#include "cmlcdfuncaa.h"

#include "cmlcdpix.h"

#ifdef DOT_ANTI_ALIASES
#include "cmlcdpixaa.h"
#include "cmlcdpixaa_noai.h"
#endif

#include "cmlcdpixdefault.h"
#include "cmlcdpixdefault_noai.h"

#include "sdlcd.h"
#include "sdlcdfont.h"
#include "ruputer.h"
#include "memory.h"


/** ����LCD����롼���󥳥󥹥ȥ饯��
 */
CMLcdFuncAa::CMLcdFuncAa(Ruputer *ruputer, int expansion, bool after_image, int ai_value) : CMLcdFuncDefault(ruputer, expansion, after_image, ai_value)
{
#ifdef DOT_ANTI_ALIASES

	delete pix;

	if( after_image )
		pix = new CMLcdPixAA(expansion, ai_value);
	else
		pix = new CMLcdPixAA_Noai(expansion);
#endif
}

/** ����LCD����롼���� �ǥ��ȥ饯��
 */
CMLcdFuncAa::~CMLcdFuncAa(void)
{
}

/** LCD���������Ť����ꤹ���public��
 * @param x x ��ɸ
 * @param y y ��ɸ
 * @param color 0:�� to 255:��
 * @param logic  gv_line �Ϥ� logic �ϲ��� 2 bit�˳�Ǽ���졢
 * gv_kput �Ϥ� logic �� 2bit �����եȤ��Ƴ�Ǽ�����
 */
void CMLcdFuncAa::setPointReal(int x, int y, int color, int logic)
{
	int c;

	switch(logic)
	{
	case 0:	/* ��� */
		pix->psetReal(x, y, color);
		break;
	case 1: /* xor */
		c = pix->pgetReal(x, y);
		c=c ^ color;
		pix->psetReal(x, y, c);
		break;
	case 4: /* string or */
	case 2:	/* or */
		c = pix->pgetReal(x, y);
//		c=c | color;
		c += c + color;
		if( c > 255 )
			c = 255;
		pix->psetReal(x, y, c);
		break;
	case 3:	/* and */
		c = pix->pgetReal(x, y);
		c=c & color;
		pix->psetReal(x, y, c);
		break;

	case 8: /* ȿž��� */
		color = 255 - color;
		pix->psetReal(x, y, color);
		break;

	case 0xc: /* ȿž OR */
		c = pix->pgetReal(x, y);
		c = ( 255 - color ) + c;
		if( c > 255 )
			c = 255;
		pix->psetReal(x, y, c);
		break;
	}
}

/** �������
 * @param x1 x1 ��ɸ
 * @param y1 y1 ��ɸ
 * @param x2 x2 ��ɸ
 * @param y2 y2 ��ɸ
 * @param style 8bit ��������ɽ����0xff ��*����*����
 * @param logic �����������
 */
void CMLcdFuncAa::drawLine(int x1, int y1, int x2, int y2, unsigned char ucstyle, int logic)
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
 *
 * Todo
 * �����������θ��������������ꥢ�� ���ˤ���������������
 */
void CMLcdFuncAa::drawLine(int x1, int y1, int x2, int y2, unsigned long style, int logic)
{
	int i, j;
	int x, y;
	int dx, dy;
	int sx, sy;
	int a, e;
	int color[32];

	x1 = pix->convertToReal( x1 );
	y1 = pix->convertToReal( y1 );
	x2 = pix->convertToReal( x2 );
	y2 = pix->convertToReal( y2 );

#ifdef DEBUG
	fprintf(stderr, "drawLine (%d, %d)-(%d, %d), styel %x, logic %x\n", x1, y1, x2, y2, style, logic);
	fflush(stderr);
#endif

	for(i=0; i < 32; i++, style>>=1)
		color[i]=(style & 1 ? 255 : 0);

/* �����ʥ� int */
#define sgni(x) ((x) > 0 ? 1 : -1)

	dx=abs(x2-x1);
	dy=abs(y2-y1);
	sx=sgni(x2-x1);
	sy=sgni(y2-y1);

	if( dx == 0 )
	{
		y = y1;
		for( i = 0 ; i <= dy; i++ )
		{
			for(j = 0; j < expansion; j++)
				setPointReal(x1 + j, y, color[i & 0x1f], logic);
			y += sy;
		}
	}
	else if( dy == 0 )
	{
		x = x1;
		for( i = 0 ; i <= dx; i++ )
		{
			for(j = 0; j < expansion; j++)
				setPointReal(x, y1 + j, color[i & 0x1f], logic);
			x += sx;
		}
	}
	else if(dx > dy)
	{
		x=x1;
		y=y1;
		e=-dx;
		a=2*dy;
		for(i=0; i <= dx; i++)
		{
			int c;

			c = - color[i & 0x1f] * e / 2 / dx;

			setPointReal(x, y,      c, logic);
			if( c == 0 )
			{
				for(j = 1; j < expansion; j++)
					setPointReal(x, y + sy * j,   0, logic);

				setPointReal(x, y + sy * j, 0, logic);
			}
			else
			{
				for(j = 1; j < expansion; j++)
					setPointReal(x, y + sy * j,   255, logic);

				setPointReal(x, y + sy * j, 255 - c, logic);
			}

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
			int c;

			c = - color[i & 0x1f] * e / 2 / dy;

			setPointReal(x, y,      c, logic);

			if( c == 0 )
			{
				for(j = 1; j < expansion; j++)
					setPointReal(x + sx * j, y,   0, logic);

				setPointReal(x + sx * j, y, 0, logic);
			}
			else
			{
				for(j = 1; j < expansion; j++)
					setPointReal(x + sx * j, y,   255, logic);

				setPointReal(x + sx * j, y, 255 - c, logic);
			}

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
void CMLcdFuncAa::drawRect(int x1, int y1, int x2, int y2, unsigned char style, int logic)
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
void CMLcdFuncAa::fillRect(int x1, int y1, int x2, int y2, unsigned long style, int logic)
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
void CMLcdFuncAa::drawStringSub(int x, int y, int fontwidth, int fontheight, char const *str, int prop_flag, int gap, int logic, int latin)
{
	SDLcdFont *font;
	int width;
	unsigned char **p;

	SDLcd *sdlcd = ruputer->getSDLcd();

	if( sdlcd->hasFontAa() )
	{
		font = sdlcd->createSDLcdFontAa( pix->convertToReal( fontwidth ), pix->convertToReal( fontheight ), prop_flag, gap, latin);
		if( font == NULL )
			return;

		p = font->drawString(str, &width);
		if( p == NULL )
		{
			delete font;
			return;
		}

		subDrawStringReal(p, pix->convertToReal( x ), pix->convertToReal( y ), width, pix->convertToReal( fontheight ), logic);
		font->freeBuffer(p);

		delete font;
	}
	else
	{
		CMLcdFuncDefault::drawStringSub(x, y, fontwidth, fontheight, str, prop_flag, gap, logic, latin);
	}
}


/** ʸ���������󥰤����Хåե����顢LCD�Хåե��إ��ԡ�����
 * @param buf 1ʸ���������󥰤����Хåե�
 * @param x x ��ɸ��ʸ���κ���κ�ɸ��
 * @param y y ��ɸ
 * @param w ���褷����
 * @param h ʸ����
 * @param logic �����������
 */
void CMLcdFuncAa::subDrawStringReal(unsigned char **buf, int x, int y, int w, int h, int logic)
{
	int i, j;

	int logic2;

	logic2 = logic << 2;

	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i++)
		{
			setPointReal(x + i, y + j, buf[j][i], logic2);
		}
	}
}



/** ȿž
 * @param x1 x1 ��ɸ
 * @param y1 y1 ��ɸ
 * @param x2 x2 ��ɸ
 * @param y2 y2 ��ɸ
 */
void CMLcdFuncAa::reverseRect(int x1, int y1, int x2, int y2)
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

#if 1
	dx = pix->convertToReal( dx );
	dy = pix->convertToReal( dy );
	x  = pix->convertToReal( x );
	y  = pix->convertToReal( y );

	dx += expansion - 1;
	dy += expansion - 1;
#endif

	for(i=0; i <= dy; i++)
	{
		for(j=0; j <= dx; j++)
		{
#if 1
			pix->psetReal(x + j, y + i, 255 - pix->pgetReal(x + j, y + i));

#else
			pix->pset(x + j, y + i, pix->pget(x + j, y + i) ^ 1);
#endif
		}
	}
}

/** �õ�
 * @param x1 x1 ��ɸ
 * @param y1 y1 ��ɸ
 * @param x2 x2 ��ɸ
 * @param y2 y2 ��ɸ
 */
void CMLcdFuncAa::clearRect(int x1, int y1, int x2, int y2)
{
	int i, j;
	int dx, dy;
	int x, y;

	dx=abs(x1-x2);
	dy=abs(y1-y2);
	x=min(x1, x2);
	y=min(y1, y2);

#if 1
	dx = pix->convertToReal( dx );
	dy = pix->convertToReal( dy );
	x  = pix->convertToReal( x );
	y  = pix->convertToReal( y );

	dx += expansion - 1;
	dy += expansion - 1;
#endif

	for(i=0; i <= dy; i++)
	{
		for(j=0; j <= dx; j++)
		{
#if 1
			pix->psetReal(x + j, y + i, 0);
#else
			pix->pset(x + j, y + i, 0);
	//			now[y+i][x+j]=0;
#endif
		}
	}
}


/** �ʱߤ�����
 * ��¼��ɧ�����أø���ˤ��ǿ����르�ꥺ���ŵ�٤��
 * ��¼�������꤬�Ȥ��������ޤ���
 *
 */
void CMLcdFuncAa::drawEllipse(int xc, int yc, int rx, int ry, int logic)
{
	int x, x1, y, y1, r;

	xc = pix->convertToReal( xc );
	yc = pix->convertToReal( yc );
	rx = pix->convertToReal( rx );
	ry = pix->convertToReal( ry );

#ifdef DEBUG
	fprintf(stderr, "\
!!  CMLcdFuncAa::drawEllipse start\n\
  xc: %d\n\
  yc: %d\n\
  rx: %d\n\
  ry: %d\n\
!!  CMLcdFuncAa::drawEllipse end\n", xc, yc, rx, ry);
	fflush(stderr);
#endif

	if (rx > ry) {
		x = r = rx;  y = 0;
		while (x >= y) {
			x1 = (int)((long)x * ry / rx);
			y1 = (int)((long)y * ry / rx);
			setPointReal(xc + x, yc + y1, 255, logic);
			setPointReal(xc + x, yc - y1, 255, logic);
			setPointReal(xc - x, yc + y1, 255, logic);
			setPointReal(xc - x, yc - y1, 255, logic);
			setPointReal(xc + y, yc + x1, 255, logic);
			setPointReal(xc + y, yc - x1, 255, logic);
			setPointReal(xc - y, yc + x1, 255, logic);
			setPointReal(xc - y, yc - x1, 255, logic);
			if ((r -= (y++ << 1) - 1) < 0)
				r += (x-- - 1) << 1;
		}
	} else {
		x = r = ry;  y = 0;
		while (x >= y) {
			x1 = (int)((long)x * rx / ry);
			y1 = (int)((long)y * rx / ry);
			setPointReal(xc + x1, yc + y, 255, logic);
			setPointReal(xc + x1, yc - y, 255, logic);
			setPointReal(xc - x1, yc + y, 255, logic);
			setPointReal(xc - x1, yc - y, 255, logic);
			setPointReal(xc + y1, yc + x, 255, logic);
			setPointReal(xc + y1, yc - x, 255, logic);
			setPointReal(xc - y1, yc + x, 255, logic);
			setPointReal(xc - y1, yc - x, 255, logic);
			if ((r -= (y++ << 1) - 1) < 0)
				r += (x-- - 1) << 1;
		}
	}
}

unsigned char const *CMLcdFuncAa::getPixelArray(int x, int y)
{
	return pix->getPixelArray(x, y);
}
