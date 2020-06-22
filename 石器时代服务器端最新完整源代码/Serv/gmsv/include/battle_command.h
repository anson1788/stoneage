#ifndef __BATTLE_COMMAND_H__
#define __BATTLE_COMMAND_H__

void BattleCommandDispach(
	int fd,
	char *command
);

void BattleEncountOut(
	int charaindex
);


#define BATTLE_CommandSend( charaindex, pszCommand) _BATTLE_CommandSend( charaindex, pszCommand,__FILE__, __LINE__)
BOOL _BATTLE_CommandSend( int charaindex, char *pszCommand,char *file ,int line );

BOOL BATTLE_MakeCharaString(
	int battleindex,
	char *pszCommand,	// Æ½ÅÒ·ÂÛÍÕı¡õÊ÷  Çë  ÛÆ
	int	size 			// ÈÓÄÌÊõ
);

void BATTLE_CharSendAll( int battleindex );
void BATTLE_CharSendOne( int battleindex,int mycharaindex );

BOOL	BATTLE_PetDefaultCommand( int petindex );

void BATTLE_ActSettingSend( int battleindex );

#endif
