/*
 * psdos_findfile.cpp - psdos function emulation for findfile
 *
 * $Id: psdos_findfile.cpp,v 1.7 2006/02/19 13:04:40 fumi Exp $
 *
 * Copyright (C) 2004, 2005 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
#include <sys/stat.h>

#include "x86reg.h"
#include "psdos.h"
#include "memory.h"

#include "directoryconverter.h"
#include "ruputerdirectory.h"
#include "ruputer.h"
#include "thread.h"

#include "sdfindfile.h"



/*
 * ds: serch path segment address
 * dx:             offset address
 * cx: attribute
 *
 *   bit��0:���ɤ߽Ф����ѥե�����
 *   bit��1:���ԲĻ�ե�����
 *   bit��2:�������ƥ�ե�����
 *   bit��3:���ܥ�塼��ե�����
 *   bit��4:���ǥ��쥯�ȥ�
 *   bit��5:������������
 *
 * success cflag = 0
 *          ax = 0
 */
void DosIntr::dos_find_first(X86Reg *x86reg)
{
	unsigned long addr;
	char const *pathname;
	char local_path[MAX_PATH];
	int res;

	addr=convToLinearAddress(x86reg->read_DS(), x86reg->read_DX());
	pathname=(char const *)Memory::getMemoryPointer(addr);

	try{
		DirectoryConverter c(rupdir, localdir);
		res=c.convert(pathname, local_path);
	}
	catch( DirectoryConverterException &e ) {
		x86reg->write_AX(3);
		x86reg->write_CFLAG(1);
		return;
	}

	if(res != 0)
	{
		x86reg->write_AX(3); //?
		x86reg->write_CFLAG(1);
		return;
	}

	struct dta_t *dta_area=(struct dta_t *)Memory::getMemoryPointer(ruputer->getDtaAddress());
// ��äĤ�
// ToDo Ruputer ���ץꥱ��������clear���줿����󥰥�󥰤��롣
	if(dta_area->sdff != NULL)
	{
// �꡼�����롣
// DAWATCH ���饢�顼�������������ΤǤȤꤢ���������ȥ�����
//		delete dta_area->sdff;
// ������ (���ɥ쥹 . dta) ������������ɬ�פ��ꡣ

		dta_area->sdff=NULL;
	}

#ifdef DEBUG
	fprintf(stderr, "!! dos_find_first path (%s) conv path (%s)\n", pathname, local_path);
	fflush(stderr);
#endif


	dta_area->sdff = ruputer->createSDFindFile(dta_area, local_path, x86reg->read_CX());
	if( dta_area->sdff == NULL )
	{
		x86reg->write_AX(2); // file is not found
		x86reg->write_CFLAG(1);
		return;
	}

	int ret;
	ret=dta_area->sdff->findFirst();
	if(ret != 0)
	{
// error
		x86reg->write_AX(ret);
		x86reg->write_CFLAG(1);
		return;
	}

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_find_first found (%s)\n", dta_area->filename);
	fflush(stderr);
#endif

	x86reg->write_AX(0);
	x86reg->write_CFLAG(0);
}


void DosIntr::dos_find_next(X86Reg *x86reg)
{
	struct dta_t *dta_area=(struct dta_t *)Memory::getMemoryPointer(ruputer->getDtaAddress());
// ��äĤ�
	if(dta_area->sdff == NULL)
	{
// error
		x86reg->write_AX(3);
		x86reg->write_CFLAG(1);
		return;
	}

	int ret;
	ret=dta_area->sdff->findNext();
	if(ret != 0)
	{
// error
		x86reg->write_AX(ret);
		x86reg->write_CFLAG(1);
		return;
	}

#ifdef DEBUG
	fprintf(stderr, "!! DosIntr::dos_find_next  found (%s)\n", dta_area->filename);
	fflush(stderr);
#endif

	x86reg->write_AX(0);
	x86reg->write_CFLAG(0);
}
