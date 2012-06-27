// -*-c++-*-
// $Id: systimer.h,v 1.5 2004/10/31 13:41:17 fumi Exp $

#ifndef SYSTIMER_H
#define SYSTIMER_H

/* ������ȥ����󥿥��ޡ�����
 * 0: ���åȤ���Ƥ��ʤ�
 * 1: ���ȥå�
 * 2: ������ȥ��������
 */
#define STS_RESET 0
#define STS_STOP 1
#define STS_RUN 2

class MemoryForInside;
class RupAddress;
class KeyBuffer;

class SysTimer
{
  private:
	RupAddress *systemAddress;
	KeyBuffer *keybuffer;

	MemoryForInside *mem;

  public:
	SysTimer(RupAddress *systemAddress, KeyBuffer *keybuffer);
	~SysTimer(void);

	void decrement(void);

  private:
	int readHour(void);
	int readMin(void);
	int readSec(void);
	int readCo(void);

	void writeHour(int hour);
	void writeMin(int min);
	void writeSec(int sec);
	void writeCo(int co);
};

#endif
