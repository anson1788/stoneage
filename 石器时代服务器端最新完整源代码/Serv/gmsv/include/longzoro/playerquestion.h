#ifndef __PLAYER_QUESTION_ONLIEN_H__
#define __PLAYER_QUESTION_ONLIEN_H__

#ifdef _PLAYER_QUESTION_ONLIEN
typedef struct PLAYER_QUESTION_t{
	char question[128];
	char result[128];
	int type;
	int value;
}PlayerQuestion_t;
BOOL PlayQuestionOnline(int charaindex, char *messageeraseescape);

#endif


#endif


