// -*-c++-*-
// $Id: cmlcdpix.h,v 1.3 2006/02/16 18:19:40 fumi Exp $

#ifndef CMLCDPIX_H
#define CMLCDPIX_H

#include "cmlcd.h"
#include "lcdblrv.h"
class LcdBlRv;

class CMLcdPix
{
  protected:
	int expansion;
	int realWidth;
	int realHeight;
/*
 * �������¤Ӥ����꤫�顢(y, x) �ˤ����
 * 4 byte �������¤�Ǥ���ʤ��Ȥ��ޤ������ޤ���
 * # unsigned long ���ΰ���ݤ���Ф����Τ����ʤ���
 * # pack �黻���Ƥ��뤿��
 */
	unsigned char now[240][320];

	unsigned char **fgPixelData;

  protected:
	int *vectorFromReal;
	int *vectorToReal;

  public:
	CMLcdPix(int expansion);
	virtual ~CMLcdPix(void);

	virtual void pset(int x, int y, int c);
	int  pget(int x, int y);

	virtual void psetReal(int x, int y, int c) = 0;
	virtual int  pgetReal(int x, int y) = 0;
	virtual void updatePixel(int start_x, int start_y, enum UpdateRegion updateRegion) = 0;

	unsigned char const *getPixelArray(int x, int y);

	int getRealWidth102(void) const;
	int getRealWidth320(void) const;
	int getRealHeight64(void) const;
	int getRealHeight240(void) const;

/**
 * �º�ɸ���� 320x240 �˼�������
 * ��ɸ��������
 */
	inline int convertFromReal(int xy) const
	{
		return vectorFromReal[ xy ];
	}

/**
 * 320x240 ����º�ɸ�˼�������
 * ��ɸ��������
 */
	inline int convertToReal(int xy) const
	{
		return vectorToReal[ xy ];
	}

	void setPreviousPix(CMLcdPix *pix);
};

#endif
