// -*-c++-*-
/* $Id: bdffontsets.h,v 1.3 2004/05/07 08:25:03 fumi Exp $
 */

#ifndef BDFFONTSETS_H
#define BDFFONTSETS_H

class BdfFont;

/** ʣ���� BDF �ե������������륯�饹
 * �Ѿ����ƻ��Ѥ���
 */
class BdfFontSets{
  public:
	BdfFontSets(void);
	virtual ~BdfFontSets(void);

	virtual BdfFont *getFontData(unsigned short code)=0;
};

#endif
