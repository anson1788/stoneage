#ifndef __CHARACTERS_H__
#define __CHARACTERS_H__
#ifdef _CHARADATA_SAVE_SQL
#include "char_base.h"

typedef struct tagChardata
{
	char CHAR_list_String[STRINGBUFSIZ];
	char CHAR_list_info1_String[STRINGBUFSIZ];
	char CHAR_list_info2_String[STRINGBUFSIZ];
	char CHAR_list_count_String[STRINGBUFSIZ];
	char CHAR_list_info3_String[STRINGBUFSIZ];
	char CHAR_list_event_String[STRINGBUFSIZ];
	char CHAR_list_info4_String[STRINGBUFSIZ];
	char CHAR_list_attackmagic_String[STRINGBUFSIZ];
	char CHAR_list_info5_String[STRINGBUFSIZ];
	char CHAR_list_profession_String[STRINGBUFSIZ];
	char CHAR_list_info6_String[STRINGBUFSIZ];
	char CHAR_list_info_String[STRINGBUFSIZ];
	char CHAR_list_flg_String[STRINGBUFSIZ];
	char CHAR_list_skill_String[STRINGBUFSIZ];
	char CHAR_list_item_String[STRINGBUFSIZ*4];
	char CHAR_list_title_String[STRINGBUFSIZ];
	char CHAR_list_mail_String[STRINGBUFSIZ];
}Charadata;

void CHAR_CharadataSaveSQL(Char* ch);
#endif
#endif
