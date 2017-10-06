#include<stdlib.h>
#define Aa 'a'-'A'
char *buf;

int tagLens(char *tag){
	int i=1;
	if(tag[0]=='<'){
		while(tag[i]!='\0'){
			if(tag[i]=='<')return 0;
			if(tag[i]=='>')return i+1;
			i++;
		}
		return 0;
	}
	else{
		return 0;
	}
}

 char* cutTagIn(char *str){
	 int TAGLEN;
	 if(!(TAGLEN=tagLens(str))) return NULL;
	 int i=0;
	 buf=(char*)malloc(sizeof(char)*TAGLEN+1);
	 for(i=0;i<TAGLEN+1;i++) buf[i]='\0';
	 i=0;
	 for(i=0;i<TAGLEN;i++){
		 buf[i]=str[i];
	 }
	 return buf;
 }
 
 void freeBuf(void){
	 free(buf);
 }

int isEndTag(char *tag){
	int i=0;
	if(tag[0]=='<'){
		while(tag[++i]==' ');
		if(tag[i]=='/')return 1;
		else return 0;
	}
	else{
		return 0;
	}
}

int isLetter(char ch){
	if((ch<'A')||((ch>'Z')&&(ch<'a'))||(ch>'z'))return 0;
	else return 1;
}

char lowerCase(char ch){
	if(isLetter(ch))return ch;
	else return (ch<='Z')?ch+Aa:ch;
}

int isThisEndTag(char *tag,char *tagEnd){
	int i=1;
	int j=1;
	if(tagLens(tag)&&tagLens(tagEnd))
		if(isEndTag(tagEnd)){
			while(tag[i]==' ')i++;
			while(tagEnd[j]!='/')j++;
			j++;
			while(tagEnd[j]==' ')j++;
			while((tag[i]!=' ')&&(tag[i]!='>')&&(tagEnd[j]!=' ')&&(tagEnd[j]!=' ')){
				if(lowerCase(tag[i])!=lowerCase(tagEnd[j])) return 0;
				i++;j++;
			}
			return 1;
		}
		else return 0;
	else return 0;
}