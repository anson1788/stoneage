#define sprVERSION	3				//SPR ?  ??????
#include "../systeminc/version.h"
#include 	"../systeminc/system.h"
#include 	"../systeminc/loadsprbin.h"
#include 	"../systeminc/anim_tbl.h"

#include	"../oft/vg410.h"
#include	"../oft/work.h"

#define ERRPRINT(a);

SPRITEDATA SpriteData[mxSPRITE];
extern int nextMaxAdrnID;
BOOL InitSprBinFileOpen(char *SPR_BIN, char *SPRADRN_BIN)
{
	SPRADRN _spradrn;
	ANIM_HEADER sprhead;
	FRAMELIST sprdata;
	FILE *fp1, *fp2;
	int j;
	unsigned int k;
	if ((fp1 = fopen(SPR_BIN, "rb")) == NULL)
		return FALSE;
	if ((fp2 = fopen(SPRADRN_BIN, "rb")) == NULL)
		return FALSE;
	static BOOL Sprflg = TRUE;
	static BOOL Sprflg1 = TRUE;
	if (Sprflg){
		Sprflg = FALSE;
		memset(SpriteData, 0, sizeof(SpriteData));
	}
	int no = 0;
	while (TRUE){
		fread(&_spradrn, 1, sizeof(_spradrn), fp2);
		if (feof(fp2) != 0)
			break;
		no = _spradrn.sprNo - SPRSTART;
		if (no > mxSPRITE) break;
		SpriteData[no].animSize = _spradrn.animSize;
		SpriteData[no].ptAnimlist = (ANIMLIST*)calloc(SpriteData[no].animSize, sizeof(ANIMLIST));
		fseek(fp1, _spradrn.offset, SEEK_SET);
		for (j = 0; j < SpriteData[no].animSize; j++){
			fread(&sprhead, 1, 12, fp1);
			SpriteData[no].ptAnimlist[j].dir = sprhead.dir;
			SpriteData[no].ptAnimlist[j].no = sprhead.no;
			if (!sprhead.frameCnt) SpriteData[no].ptAnimlist[j].dtAnim = 0;
			else SpriteData[no].ptAnimlist[j].dtAnim = sprhead.dtAnim / sprhead.frameCnt / 16;
			SpriteData[no].ptAnimlist[j].frameCnt = sprhead.frameCnt;
			SpriteData[no].ptAnimlist[j].ptFramelist = (FRAMELIST*)calloc(SpriteData[no].ptAnimlist[j].frameCnt, sizeof(FRAMELIST));
			for (k = 0; k < SpriteData[no].ptAnimlist[j].frameCnt; k++){
				fread(&sprdata, 1, 10, fp1);
				SpriteData[no].ptAnimlist[j].ptFramelist[k].BmpNo = sprdata.BmpNo + nextMaxAdrnID;
				SpriteData[no].ptAnimlist[j].ptFramelist[k].PosX = sprdata.PosX;
				SpriteData[no].ptAnimlist[j].ptFramelist[k].PosY = sprdata.PosY;
				SpriteData[no].ptAnimlist[j].ptFramelist[k].SoundNo = sprdata.SoundNo;
			}
		}
		if (no == 1059) SpriteData[1059].ptAnimlist[80].ptFramelist[8].BmpNo = 283639;
		if (no == 1059) SpriteData[1059].ptAnimlist[93].ptFramelist[8].BmpNo = 283640;
		if (no == 1283) SpriteData[1283].ptAnimlist[7].ptFramelist[6].SoundNo = 10100;
		if (no == 1283) SpriteData[1283].ptAnimlist[49].ptFramelist[6].SoundNo = 10100;
		if (no == 1404) SpriteData[1404].ptAnimlist[0].ptFramelist[6].BmpNo = 284436;
		if (no == 1409) SpriteData[1409].ptAnimlist[0].ptFramelist[6].BmpNo = 284476;
		if (no == 373)
		for (int kk = 0; kk < 8; kk++) {
			SpriteData[373].ptAnimlist[kk * 7].ptFramelist[8].SoundNo = 254;
			SpriteData[373].ptAnimlist[kk * 7].ptFramelist[10].SoundNo = 254;
			SpriteData[373].ptAnimlist[kk * 7].ptFramelist[15].SoundNo = 250;
		}
		if (no == 102) SpriteData[102].ptAnimlist[82].ptFramelist[0].BmpNo = 126235;
		if (no == 102) SpriteData[102].ptAnimlist[83].ptFramelist[0].BmpNo = 126236;
		if (no == 102) SpriteData[102].ptAnimlist[83].ptFramelist[1].BmpNo = 126237;
		if (no == 102) SpriteData[102].ptAnimlist[84].ptFramelist[1].BmpNo = 126238;
		if (no == 1058) SpriteData[1058].ptAnimlist[80].ptFramelist[8].BmpNo = 232475;
		if (no == 1058) SpriteData[1058].ptAnimlist[93].ptFramelist[8].BmpNo = 232476;

		if (no == 260) SpriteData[260].ptAnimlist[21].ptFramelist[5].SoundNo = 10001;
		if (no == 502) SpriteData[502].animSize = 1;
		if (no == 502) SpriteData[502].ptAnimlist = (ANIMLIST*)calloc(SpriteData[502].animSize, sizeof(ANIMLIST));
		if (no == 502) SpriteData[502].ptAnimlist[0].dir = 0;
		if (no == 502) SpriteData[502].ptAnimlist[0].no = 0;
		if (no == 502) SpriteData[502].ptAnimlist[0].dtAnim = 4;
		if (no == 502) SpriteData[502].ptAnimlist[0].frameCnt = 3;
		if (no == 502) SpriteData[502].ptAnimlist[0].ptFramelist = (FRAMELIST*)calloc(SpriteData[502].ptAnimlist[0].frameCnt, sizeof(FRAMELIST));
		int iTemp = 8590;
		if (no == 382){
			for (int kkk = 0; kkk < 8; kkk++){
				SpriteData[382].ptAnimlist[7 * kkk].frameCnt = 14;
				free(SpriteData[382].ptAnimlist[7 * kkk].ptFramelist);
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist = (FRAMELIST*)calloc(SpriteData[382].ptAnimlist[7 * kkk].frameCnt, sizeof(FRAMELIST));
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[4].SoundNo = SpriteData[381].ptAnimlist[0].ptFramelist[4].SoundNo;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[9].SoundNo = SpriteData[381].ptAnimlist[0].ptFramelist[9].SoundNo;
				SpriteData[382].ptAnimlist[7 * kkk].dtAnim = SpriteData[381].ptAnimlist[0].dtAnim;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[0].BmpNo = 124604 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[1].BmpNo = 124605 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[2].BmpNo = 124606 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[3].BmpNo = 124607 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[4].BmpNo = 124608 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[5].BmpNo = 124609 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[6].BmpNo = 124610 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[7].BmpNo = 124611 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[8].BmpNo = 124607 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[9].BmpNo = 124608 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[10].BmpNo = 124609 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[11].BmpNo = 124610 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[12].BmpNo = 124611 + kkk * 29;
				SpriteData[382].ptAnimlist[7 * kkk].ptFramelist[13].BmpNo = 124604 + kkk * 29;
			}
		}
		if (no == 502)
		for (k = 0; k < SpriteData[502].ptAnimlist[0].frameCnt; k++){
			SpriteData[502].ptAnimlist[0].ptFramelist[k].BmpNo = iTemp++;
			SpriteData[502].ptAnimlist[0].ptFramelist[k].PosX = 0;
			SpriteData[502].ptAnimlist[0].ptFramelist[k].PosY = 0;
			SpriteData[502].ptAnimlist[0].ptFramelist[k].SoundNo = 0;
		}


		//Change fix 投掷动作没有设定SoundNo,战斗时会当
		if (no >= 1965 && no < 1987) {
			//for(int kk=1965;kk<1987;kk++)
			//{
			SpriteData[no].ptAnimlist[9].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[22].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[35].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[48].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[61].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[74].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[87].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[100].ptFramelist[4].SoundNo = 10006;
			//}
		}
		if (no >= 1988 && no < 1990) {
			//for(int kk=1988;kk<1990;kk++)
			//{
			SpriteData[no].ptAnimlist[9].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[22].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[35].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[48].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[61].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[74].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[87].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[100].ptFramelist[4].SoundNo = 10006;
			//}
		}

		if (no >= 3347 && no <= 3370)
		{		
			SpriteData[no].ptAnimlist[9].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[22].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[35].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[48].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[61].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[74].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[87].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[100].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[52].ptFramelist[4].SoundNo = 10006;
			SpriteData[no].ptAnimlist[39].ptFramelist[5].SoundNo = 10006;
		}
	}
	fclose(fp1);
	fclose(fp2);
	if (Sprflg1){
		Sprflg1 = FALSE;
	}
	return TRUE;
}
