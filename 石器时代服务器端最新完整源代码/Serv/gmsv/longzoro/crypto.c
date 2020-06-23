#include  <stdio.h>
#include  <dirent.h>   
#include  <string.h>  
#include  <sys/types.h>   
#include  <sys/stat.h> 
#include  "autil.h"
#include  "util.h"

//char cryptokey[]=DEFAULTTABLE;
char cryptokey[]="aBcDeFgHiJkLmNoPqRsTuVwXyZ";

void CryptoKey(char* usrkey)   
{   
	int cryptokeylen = strlen(cryptokey);
	int i;

  for(i=0;i<strlen(usrkey);i++)   
  {   
  	if(usrkey[i]=='\n')continue;
  	usrkey[i]++;
  	if(usrkey[i]==cryptokey[i%cryptokeylen])continue;
  	if((usrkey[i]^cryptokey[i%cryptokeylen]) == '\n')continue;
    usrkey[i] ^= cryptokey[i%cryptokeylen];
  }
}

void DecryptKey(char* usrkey)   
{   
	int cryptokeylen = strlen(cryptokey);
	int i;
  for(i=0;i<strlen(usrkey);i++)   
  {   
  	
  	if(usrkey[i]=='\n')continue;
  	
  	if(usrkey[i]==cryptokey[i%cryptokeylen]){
  		usrkey[i]--;
  		continue;
  	}
  	if((usrkey[i]^cryptokey[i%cryptokeylen]) == '\n'){
  		usrkey[i]--;
  		continue;
  	}
    usrkey[i] ^= cryptokey[i%cryptokeylen];
    usrkey[i]--;
  }   
}

void List(char *path)
{
	struct dirent* ent = NULL;
	char filename[256];
	DIR *pDir;
	pDir=opendir(path);
	while(NULL != (ent=readdir(pDir)))
	{
		if(ent->d_name[0] == '.')continue;
		if (ent->d_type==8){
		  char line[1024];
		  if( ent->d_name == NULL
        ||strlen( ent->d_name ) < 1
        ||ent->d_name[strlen(ent->d_name)-1] == '~'
        ||ent->d_name[0] == '#'
        ||strcmptail( ent->d_name, ".bak" ) == 0 
      	||strcmp( path, "attmagic.bin" ) == 0)continue;
        
		  sprintf(filename, "%s/%s", path, ent->d_name);
			FILE * f1 = fopen(filename,"r");

			sprintf(filename, "allblues/%s/%s.allblues", path, ent->d_name);
			remove( filename);
			FILE * f2 = fopen(filename,"a+");
		 	while( fgets( line, sizeof( line ), f1 ) ){
		 		CryptoKey(line);
		 		fputs(line,f2);
			}
		 	fclose(f1);
		 	fclose(f2);
		}else{
			if( strcmp( path, "data" ) == 0 
				&& (strcmp( ent->d_name, "autopk" ) == 0 
					|| strcmp( ent->d_name, "map" ) == 0))continue;
			sprintf(filename, "allblues/%s/%s", path, ent->d_name);
			if(opendir(filename)==NULL){
				if(mkdir(filename, 0x777) == 0){
					printf("建立加密文件夹 %s\n", filename);
				}
			}
			sprintf(filename, "%s/%s", path, ent->d_name);
			List(filename);
		}
	}
}
