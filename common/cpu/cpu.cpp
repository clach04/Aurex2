/*
 * cpu.cpp - Panasonic MN10200 CPU emulation module
 *
 * $Id: cpu.cpp,v 1.14 2004/11/13 18:35:47 fumi Exp $
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

#include "cpu.h"
#include "memorycpu.h"
#include "interruption.h"

/** ���󥹥ȥ饯��
 * @param _thread ���Υ��󥹥��󥹤������������å�
 * @param pc �ץ���५����
 * @param sp �����å��ݥ��󥿡�pc �� push �� a3 ������������
 * @param interruption �����ߥ��饹
 * @param mem ���ꥢ���������饹��CPU �ѡ�
 */
Cpu::Cpu(unsigned long pc, unsigned long sp, Interruption *interruption, MemoryForCpu *mem)
{
	memset(&regs, 0, sizeof(Register));

	this->mem = mem;
	this->interruption = interruption;

	regs.pc=pc;
	regs.a[3]=sp;

	regs.a[3]-=4;
	mem->write24(regs.a[3], regs.pc);

}
/** �ǥ��ȥ饯��
 */
Cpu::~Cpu(void)
{
// interruption �� Thread �Ǻ������Ƥ��뤬�����Ǻ������ɬ�פ���
	delete interruption;
}


/** ���Υ쥸�����ξ��֤���������
 * @param str ��������Хåե��Υݥ���
 */
void Cpu::convRegsToString(char *str)
{
	regs.toString(str);
}


/** ���Υ쥸�����ξ��֤�ɸ�२�顼���Ϥ˽��Ϥ���
 */
void Cpu::printRegs(void)
{
	char str[1024];
	convRegsToString(str);
	fputs(str, stderr);
}
/**
 * ���ߥ�졼�Ȥ��롣
 * Register ����¸�Ǥ��ʤ��㳰
 * ���ߤǤ� Memory �Ϥ�������ޤ��ơ������� �㳰�� throw ����ʥ���������ˡ�
 *
 */
int Cpu::emulate(void)
{
	int n;

	try {
		n = emulate_core();
	}
	catch(OddAccessException &e) {
		throw OddAccessException(&regs, e.getEffectiveAddress());
	}

	return n;
}
