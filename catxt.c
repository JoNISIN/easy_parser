#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include"taglibc.h"

FILE *textptr;
int FILE_LNE=0;
int BUFFER_USED=0;
char *TEXT_BUFFER=NULL;

typedef struct cbuffer{
	char c;
	struct cbuffer *next;
} *buffchar;

buffchar cbuff = NULL;
buffchar bufhead = NULL;

int textOpen(char name[]){
	free(textptr);
	textptr=fopen(name,"r");
	if(textptr!=NULL) return 1;
	else return 0;
}

/*--------------------------------------------------------------*/
//先算字元數  再取buffer
int textLen(){
	char c;
	if(textptr==NULL){
		puts("Flie NULL pointer");
	}
	else{
		while(c=fgetc(textptr)!=EOF) FILE_LNE++;
	}
	return FILE_LNE;
}

void setbuffer(int len){
	if(textptr==NULL){
		puts("Flie NULL pointer");
	}
	else{
		TEXT_BUFFER=(char*)malloc(sizeof(char)*len);
	}
}


/*舊範例
if(textOpen(argv[1])){
		puts("Open success");
		//printf("%d character in this text\n",textLen());
		//puts("Set the Buffer for index...");
		//setbuffer(FILE_LNE);
		//puts("Buffer on set");
		printf("We have %d character in buffer ->\n",set_text());
		//puts(TEXT_BUFFER);
	}
*/


/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
//鏈結BUFFER 相關函數
int set_link_buffer(void){
	char c;
	cbuff=(buffchar)malloc(sizeof(struct cbuffer));
	bufhead=cbuff;
	while((c=fgetc(textptr))!=EOF){
		cbuff->c=c;
		cbuff->next=(buffchar)malloc(sizeof(struct cbuffer));
		cbuff=cbuff->next;
		BUFFER_USED++;
	}
	cbuff->c=EOF;
return BUFFER_USED;
}

void print_buffer(void){
	cbuff=bufhead;
	while(cbuff->c!=EOF) {
		printf("%c",cbuff->c);
		cbuff=cbuff->next;
	}
	puts("");
}

void clean_buffer(void){
	cbuff=bufhead;
	while(cbuff->c!=EOF) {
		bufhead=cbuff;
		cbuff=cbuff->next;
		free(bufhead);
	}
	bufhead=NULL;
	cbuff=NULL;
	BUFFER_USED=0;
}

int get_buffer(void){
	if(BUFFER_USED<=0) {
		puts("No Buffer");
		return -1;
	}
	setbuffer(BUFFER_USED+1);
	cbuff=bufhead;
	BUFFER_USED=-1;
	TEXT_BUFFER[++BUFFER_USED]=cbuff->c;
	
	while(cbuff->c!=EOF){
		cbuff=cbuff->next;
		TEXT_BUFFER[++BUFFER_USED]=cbuff->c;
		FILE_LNE++;
	}
	TEXT_BUFFER[BUFFER_USED]='\0';
	clean_buffer();
	return FILE_LNE;
}

/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
//超文本文件分析   直接擷取記憶體  不用操作硬碟
//相關函數已include  taglibc.h

typedef struct{
	int pos_head;
	int pos_tail;
} position;

typedef struct{
	char *headptr;
	char *tailptr;
} POSPTR;

typedef struct tags{
	position pos;
	POSPTR ptr;
	int isEnd;
	char *tagname;
} TAG;

typedef struct Hytree{
	TAG *this;
	TAG *parent;
	TAG *child;
	TAG *next;
} *HYPERNODE;

int howManyTags(char *buffer){  //僅供參考
	int i=0;
	int header=0;
	int tailer=0;
	while(buffer[i]!='\0'){
		if(buffer[i]=='<')header++;
		if(buffer[i]=='>')tailer++;
		i++;
	}
	return (header>tailer)?tailer:header;
}

/*-------------------------------------------------------------*/
//文件分析
//字元搜尋   字詞搜尋(待更新)   件尾PTR (可能可以直接重寫整個lib)

char *TEXT_TAIL=NULL;

int setTextTail(void){
	if(TEXT_BUFFER==NULL)return 0;
	TEXT_TAIL=TEXT_BUFFER;
	while((*TEXT_TAIL)!='\0') TEXT_TAIL++;
	return 1;
}

void freeText(void){
	free(textptr);
	TEXT_TAIL=NULL;
}

char* findThisChar(char ch,char *fromPtr){
	if((fromPtr==NULL)||(fromPtr==TEXT_TAIL)) return NULL;
	if((*fromPtr)==ch) return fromPtr;
	else return findThisChar(ch,fromPtr+1);
}

//待修正
int hasSameWord(char *str1,char *str2){
	int i=0;
	while(str1[i]!='\0'){
		if(lowerCase(str1[i])!=lowerCase(str2[i])) return 0;
		i++;
	}
	if(isLetter(str2[i])) return 0;
	else return i;
}

char* findThisWord(char *ch,char *rangeL,char *rangeR){
	int i=0;
	while((rangeL=findThisChar(ch[0],rangeL))<=rangeR){
			if(hasSameWord(ch,rangeL)){
				return rangeL;
			}
			else return findThisWord(ch,rangeL+1,rangeR);
		}
	return NULL;
}

//跳過註解 10.06新增<--待修正
int isEx(char *ptr){
	int i=0;
	if((ptr[0]=='<')&&(ptr[1]=='!')&&(ptr[2]=='-')&&(ptr[3]=='-')){
		return 1;
	}
	else return 0;
}
//跳過JS
int isScript(char *ptr){
	int i=1;
	if(ptr[0]!='<') return 0;
	while(ptr[i]==' ') i++;
	if(hasSameWord("script",ptr+i)) return 1;
	else return 0;
}
//跳過meta
int isMeta(char *ptr){
	int i=1;
	if(ptr[0]!='<') return 0;
	while(ptr[i]==' ') i++;
	if(hasSameWord("meta",ptr+i)) return 1;
	else return 0;
}

int isSet(char *ptr){
	int i=1;
	if(ptr[0]=='<'&&(ptr[1]=='!')&&(ptr[2]!='-')) return 1;
	else return 0;
}

int isNoScript(char *ptr){
	int i=1;
	if(ptr[0]!='<') return 0;
	while(ptr[i]==' ') i++;
	if(hasSameWord("noscript",ptr+i)) return 1;
	else return 0;
}

//跳過link
int isLink(char *ptr){
	int i=1;
	if(ptr[0]!='<') return 0;
	while(ptr[i]==' ') i++;
	if(hasSameWord("link",ptr+i)) return 1;
	else return 0;
}

int isStyle(char *ptr){
	int i=1;
	if(ptr[0]!='<') return 0;
	while(ptr[i]==' ') i++;
	if(hasSameWord("style",ptr+i)) return 1;
	else return 0;
}

//判斷函數->isWhat

int isWhat(char *ptr){
	if(isEx(ptr)) return 1;
	else if(isScript(ptr)) return 2;
	else if(isMeta(ptr)) return 3;
	else if(isSet(ptr)) return 4;
	else if(isNoScript(ptr)) return 5;
	else if(isLink(ptr)) return 6;
	else if(isStyle(ptr)) return 7;
	else return 0;
}

//跳過非HTML架構函數(帶擴充)

char* toStructTag(char *ptr){
	int ISWHAT;
	if((ptr==NULL))return NULL;
	ptr=findThisChar('<',ptr);
	while((ISWHAT=isWhat(ptr))){
		switch(ISWHAT){
			case 1:
				ptr=findThisWord("-->",ptr,TEXT_TAIL)+3;
				break;
			case 2:
				while(1){
					ptr=findThisChar('<',ptr+1);
					if(isThisEndTag("<script>",ptr)) {
						ptr=findThisWord(">",ptr,TEXT_TAIL);
						break;
					}
				}
				break;
			case 3:
				ptr=findThisWord(">",ptr,TEXT_TAIL);
				break;
			case 4:
				ptr=findThisWord(">",ptr,TEXT_TAIL);
				break;
			case 5:
				while(1){
					ptr=findThisChar('<',ptr+1);
					if(isThisEndTag("<noscript>",ptr)) {
						ptr=findThisWord(">",ptr,TEXT_TAIL);
						break;
					}
				}
				break;
			case 6:
				ptr=findThisWord(">",ptr,TEXT_TAIL);
				break;
			case 7:
				while(1){
					ptr=findThisChar('<',ptr+1);
					if(isThisEndTag("<style>",ptr)) {
						ptr=findThisWord(">",ptr,TEXT_TAIL);
						break;
					}
				}
				break;
		}
		ptr=findThisChar('<',ptr);
	}
	return ptr;
}

/*-------------------------------------------------------------*/
//詞彙處理
//已include  taglibc.h

int lenforWord(char *word){
	int i=0;
	while(word[i+1]!='\0'){
		i++;
	}
	return i+1;
}

int touchWord(char *pos1,char *target,int wordlens,int pos){
	if(pos==wordlens) return 1;
	if(pos1[0]==target[0]) return touchWord(pos1+1,target+1,wordlens,pos+1);
	else return 0;
}

/*-------------------------------------------------------------*/

//遞迴搜尋

//測試用函數
void print_ALL_Tag(char *nowPos){
	int len;
	
	if((nowPos==TEXT_TAIL)||(nowPos==NULL))return;
	nowPos=toStructTag(nowPos);
	if((len=tagLens(nowPos))){
		printf("%s",cutTagIn(nowPos));
		freeBuf();
		print_ALL_Tag(nowPos+len);
	}
	else print_ALL_Tag(nowPos+1);
}


/*-------------------------------------------------------------*/

int main(int argc,char *argv[]){
	printf("%d\n",argc);
	printf("%s\n",argv[0]);
	
	if(argc==1){
		puts("NULL file name");
		return 1;
	}
	
	if(textOpen(argv[1])){
		puts("Open success");
		printf("We have %d character in buffer ->\n",set_link_buffer());
		//print_buffer();
		get_buffer();
		//printf("set file lens %d and->\n%s\n",FILE_LNE,TEXT_BUFFER);
		printf("I think we have %d tag in this hyperText\n",howManyTags(TEXT_BUFFER));
		printf("Test Word length -> lens has %d lens\n",lenforWord("lens"));
		if(touchWord("123 ","123",3,0)) printf("Good\n");
		else printf("fail");
	}
	else{
	puts("Open fail, please Enter true file name");
	}
	if(setTextTail()) printf("Your TEXT has a TAIL~ \"%c\" \n",*(TEXT_TAIL-1));
	int aa;
	if((aa=hasSameWord("abc","abc d"))) printf("We are same word in %d\n",aa);
	else puts("we are not");
	print_ALL_Tag(TEXT_BUFFER);
	return 0;
}