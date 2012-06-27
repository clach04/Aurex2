/*
 * optionlist.cpp - keep option list as option
 *
 * $Id: optionlist.cpp,v 1.8 2006/02/16 18:19:40 fumi Exp $
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
#include <ctype.h>
#ifdef DEBUG
#include <unistd.h>
#endif

#include <assert.h>

#include <string>
#include <iterator>
#include <list>
#include <map>
using namespace std;

#include "optionlist.h"

static void removeHeadAndTailSpace(char **str)
{

	char *p;

	p=*str;
// ��������
	while(isspace(*p))
		p++;
	*str=p;

	if( **str == 0 )
		return;

// �����
	p=strchr(*str, 0);
	p--;
	while(isspace(*p))
		*p--=0;
}


OptionList::OptionList(void)
{
}


OptionList::~OptionList(void)
{
}

/** ���ץ������ɲä���
 * @param key ���ץ����Υ������
 * @param value ������
 */
void OptionList::addOption(char const *key, char const *value)
{
	assert( key );
//	assert( value );


#ifdef DEBUG
	fprintf(stderr, "add option key: %s , value:%s\n", key, value);
#endif

	options.insert(pair<string, string>(key, value));
}


/** ���ץ�������¸����
 * �ѹ�����Ƥʤ������¸���ʤ���̤������
 * �¹���˥��ץ�����ѹ�����ʤ����Ȥ���������Ƥ��������¸���ʤ���̤������
 * �����������ץ����Υե����뤬¸�ߤ��ʤ����Ϻ��������̤������
 *
 * ������̵���˥ե�����񤭽Ф�
 * @param filename �ե�����̾
 */
bool OptionList::save(char const *filename)
{
	FILE *fp;

#ifdef DEBUG
	{
		char dir[1024];
		getcwd(dir, sizeof(dir));
		fprintf(stderr, "save %s / %s\n", dir, filename);
	}
#endif

	fp=fopen(filename, "w");
	if(fp == NULL)
		return false;

	map<string, string>::iterator i;
	i=options.begin();
	while(i != options.end())
	{
		fprintf(fp, "%s=%s\n", i->first.c_str(), i->second.c_str());
		i++;
	}

	fclose(fp);
	return true;
}
/** ���ץ�������ɤ���
 * @param filename ���ɤ���ե�����̾
 * @retval true ����
 * @retval false �۾�
 */
bool OptionList::load(char const *filename)
{
	FILE *fp;

	fp=fopen(filename, "r");
	if(fp == NULL)
		return false;

	{
		char str[512];
		char *key, *eq, *value;

		while(fgets(str, sizeof(str), fp) != NULL)
		{
			if(*str == '#')
				continue;

			eq=strchr(str, '=');
			if(eq == NULL)
			{
		// error ?
				continue;
			}

			*eq=0;
			value=eq+1;
			key=str;
			removeHeadAndTailSpace(&key);
			removeHeadAndTailSpace(&value);

			addOption(key, value);
		}
		fclose(fp);

	}
	return true;
}

/** ���ɻ��Υ��ץ�����*����*�Υ��ץ����Ʊ�����ɤ���Ƚ�Ǥ���ʸ���̤������
 * @retval 1 Ʊ��
 * @retval 0 �㤦
 */
int OptionList::equalLoadOption(void)
{
	return 1;
}

/** ���ץ���������
 * @param key �����ʥ��ץ����̾��
 * @retval bool ������
 */
char const *OptionList::getOptionValue(char const *key)
{
	map<string, string>::iterator i;
	string str(key);

	i = options.find(str);
// ���Ĥ���ʤ��ä�
	if(i == options.end())
		return NULL;

	return i->second.c_str();
}

void OptionList::setOptionValue(char const *key, char const *value)
{
	map<string, string>::iterator i;
	string str(key);

	i = options.find(str);
// ���Ĥ���ʤ��ä����ɲä��롩
	if(i == options.end())
		return;

	i->second.assign(value);
}
