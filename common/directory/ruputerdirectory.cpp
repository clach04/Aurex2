/*
 * ruputerdirectory.cpp - Ruputer directory module
 *
 * $Id: ruputerdirectory.cpp,v 1.16 2005/01/14 04:09:40 fumi Exp $
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
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

#include "dirstacksjis.h"

#include "ruputerdirectory.h"
#include "directoryconverter.h"
#include "codingsystem.h"
#include "ruputer.h"

/**
 * Ruputer �����פΥǥ��쥯�ȥ�
 * DosDirectory ��Ѿ����Ƥ���
 *
 * @param directory Directory ���饹��Ѿ��������󥹥��󥹡ʥ����ƥ��¸��
 *
 */
RuputerDirectory::RuputerDirectory(Directory *localDirectory)
{
	this->localDirectory = localDirectory;
	drive='b';

	coding = SHIFT_JIS;
}

RuputerDirectory::RuputerDirectory(Directory *localDirectory, char const *path) : DosDirectory(path)
{
	this->localDirectory = localDirectory;
	drive='b';

	coding = SHIFT_JIS;
}
/** Ruputer �λĤ�path
 * @param src "b:\system\ini\..."�����л���� �䡢"foo"�����л���� ��
 * @param restpath ��һ���
 * @retval 0 ����
 * @retval 1 �۾�
 *
 * ��������
 *  src �����Хѥ����褿���ϡ�restpath �ˤ� b:\ �����������
 *        ���Хѥ����褿���ϡ�restpath �ˤ� (Ruputer �����ȥǥ��쥯�ȥ�) + src ����Ǽ�����
 *
 */
int RuputerDirectory::getRestPath(char const *src, char *restpath) throw(DirectoryException)
{
	DirStack *rest;

	if(*src == '\\')
	{
// ���Хѥ�
		rest=new DirStackSjis(src+1, '\\');
	}
	else if(*(src+1) == ':' && *(src+2) == '\\')
	{
// ���Хѥ��ʥɥ饤���դ���
		rest=new DirStackSjis(src+3, '\\');
	}
	else if(*(src+1) == ':' && *(src+2) == 0)
	{
// ���Хѥ��ʥɥ饤�֤Τߡ�
		*restpath=0;
		return 0;
	}
	else
	{
// ���Хѥ��ʤ���¾��
#if 1
		char str[1024];

		getdir(str);
		strcat(str, "\\");
		strcat(str, src);

#ifdef DEBUG
		fprintf(stderr, "!! RuputerDirectory::rest relative path str = (%s)\n", str);
		fflush(stderr);
#endif

		rest=new DirStackSjis(str+3, '\\');
#else
		rest=new DirStackSjis(src, '\\');
#endif

	}

	rest->normalize();
	rest->toFullPath(restpath, '\\');

	delete rest;

	return 0;
}


int RuputerDirectory::chdir(char const *path)
{
// �ۥ��Ȥˤ���ǥ��쥯�ȥ�Ǥʤ��� chdir �Բġ������������λ��ͤ�ɤ����Ȥϻפ���
// ���Τ��ᡢ���ۥǥ��쥯�ȥ����ѤǤʤ����Բ�
	try {
		DirectoryConverter c(this, localDirectory);
		char dir[1024];
		c.convert(path, dir);

#ifdef DEBUG
		fprintf(stderr, "!! RuputerDirectory::chdir path (%s) -> local path (%s) \n", path, dir);
		fflush(stderr);
#endif
		struct stat st;
		if(stat(dir, &st) == -1)
			return 1;

#ifdef MSVC
		if( ( st.st_mode & _S_IFDIR ) == 0 )
			return 1;
#else
		if(!S_ISDIR(st.st_mode))
			return 1;
#endif
	}
	catch( DirectoryConverterException &e ) {
// ���ۥǥ��쥯�ȥ�̤����Τ��ᡢchdir �Բ�
#ifdef DEBUG
		fprintf(stderr, "!! RuputerDirectory::chdir catch Exception");
		fflush(stderr);
#endif
		return 1;
	}

	DosDirectory::chdir(path);

	return 0;
}



void RuputerDirectory::getReplacePath(char *dst)
{
	strcpy(dst, "b:");
}


bool RuputerDirectory::isSetupDirectory(void)
{
	return true; // ������ꤵ��Ƥ���Ȥ���
}
