/*
 * filerinibase.cpp - filer.ini base class
 *
 * $Id: filerinibase.cpp,v 1.5 2005/01/08 15:51:53 fumi Exp $
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
#include "filerinibase.h"
#include "extlist.h"

/** ���󥹥ȥ饯��
 */
FilerIniBase::FilerIniBase(void)
{
}

/** �ǥ��ȥ饯��
 */
FilerIniBase::~FilerIniBase(void)
{
}

/** <��ĥ��, �б�����exf̾> ���ɲä��롣
 * @param ext ��ĥ�ҡ�3ʸ���ʲ���
 * @param exf Ruputer �ѥ��μ¹�exf�Υե�ѥ�
 */
void FilerIniBase::add(char const *ext, char const *exf)
{
#ifdef DEBUG
	fprintf(stderr, "add < (%s), (%s) >\n", ext, exf);
	fflush(stderr);
#endif

	char normalize_ext[256];
	normalizeExt(ext, normalize_ext);

	assoc.insert(pair<string, string>(normalize_ext, exf));
}


/** ��ĥ�Ҥ��顢�¹Ԥ��٤�exf�Υե�ѥ���Ruputer�ˤ�����
 * @param ext ��ĥ��
 * @retval !=NULL �¹Ԥ���exf�Υե�ѥ�
 * @retval NULL ��ĥ�Ҥ���Ͽ����Ƥ��ʤ�
 */
char const *FilerIniBase::getExecutableFileNameByExt(char const *ext)
{
	char normalize_ext[256];
	normalizeExt(ext, normalize_ext);

	map<string, string>::iterator i;
	i = assoc.find( normalize_ext );
	if(i == assoc.end())
		return NULL;

// ����ä�����פʤΤ����顣
	return i->second.c_str();
}

/**
 * ��ʸ������������Ԥʤ�
 *
 */
void FilerIniBase::normalizeExt(char const *in, char *out)
{
	while( *in )
	{
		if( *in >= 'A' && *in <= 'Z' )
			*out = *in - 'A' + 'a';
		else
			*out = *in;

		in++;
		out++;
	}
	*out = 0;
}



ExtList *FilerIniBase::getExtList(void)
{
	ExtList *extlist = new ExtList();

	map<string, string>::iterator i;
	i = assoc.begin();
	while( i != assoc.end() )
	{
// string ���ɲä������������⡩
		extlist->add( i->first.c_str() );

		i++;
	}

	return extlist;
}
