/*
 * filerinimgr.cpp - handling (system) filer.ini and (user) aurex.ini
 *
 * $Id: filerinimgr.cpp,v 1.6 2005/01/06 14:21:33 fumi Exp $
 *
 * Copyright (C) 2004, 2012 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include <string.h>

#include "filerini.h"
#include "ufilerini.h"
#include "filerinimgr.h"

#include "extlist.h"

#include <string>
#include <map>
using namespace std;

/** ���󥹥ȥ饯��
 */
FilerIniMgr::FilerIniMgr(FilerIni *fini, UFilerIni *uini)
{
	this->filer = fini;
	this->ufiler = uini;
}

/** �ǥ��ȥ饯��
 */
FilerIniMgr::~FilerIniMgr(void)
{
//	delete ufiler;
//	delete filer;
}
#if 0
/** filer.ini ����ɤ����̤������
 * @param rootdir ���ۥ롼�ȥǥ��쥯�ȥ�
 * @retval 0 ����
 * @retval 1 ����
 */
int FilerIniMgr::loadFilerIni(Directory *rootdir)
{
	return 0;
}
/** �桼������� filer.ini ����ɤ���
 * @param dir �ǥ��쥯�ȥꡣ�ʻ��꤬���ꡢfilename �����ФǤ���С����Υѥ����Фǳ�����
 * @param filename �ե�����̾
 * @retval 0 ����
 * @retval 1 ����
 */
int FilerIniMgr::loadUFilerIni(Directory *dir, char const *filename)
{
	if(dir)
	{
		char fullFileName[1024];
		dir->getFullFileName(filename, fullFileName);
		ufiler=new UFilerIni(fullFileName);
	}
	else
		ufiler=new UFilerIni(filename);

	return 0;
}

#endif



/** *�ե�����̾*���顢�¹Ԥ��٤�exf�Υե�ѥ���Ruputer�ˤ�����
 * @param filename �ե�����̾
 * @retval !=NULL �¹Ԥ���exf�Υե�ѥ�
 * @retval NULL ��ĥ�Ҥ���Ͽ����Ƥ��ʤ�
 */
char const *FilerIniMgr::getExecutableFileName(char const *filename)
{
	char *ext;
	ext=strrchr(filename, '.');
	if(ext == NULL)
		return NULL;

	ext++;

	char const *p;
	if( ufiler )
	{
		p = ufiler->getExecutableFileNameByExt(ext);
		if(p != NULL)
			return p;
	}
	if( filer )
		return filer->getExecutableFileNameByExt(ext);

	return NULL;
}

/**
 * ��Ͽ�Ѥߤγ�ĥ�ҥꥹ�Ȥ��������
 * @param out "txt,adr.." ����޶��ڤ�γ�ĥ�ҥꥹ��
 *
 * Todo �����β��������ʤ���
 */
void FilerIniMgr::getExtList(char *out)
{
//	strcpy(out, "txt,adr,tdo");
//	strcpy(out, "txt,tdo,adr");
	ExtList *a = NULL;
	ExtList *b = NULL;

	if( ufiler )
		a = ufiler->getExtList();
	if( filer  )
		b = filer->getExtList();


	if( a == NULL )
	{
		if( b  == NULL )
		{
			*out = 0;
			return;
		}
		b->getExtListString( out );
	}
	else
	{
		a->add(b);
		a->getExtListString( out );
	}

	delete a;
	delete b;
}
