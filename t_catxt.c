#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include"cattext.h"
#include"test.h"

int main(int argc,char *argv[]){
	printf("%d\n",argc);
	printf("%s\n",argv[0]);
	
	testptr=fopen("testOutput.txt","a");
	
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
	test_print_ALL_Tag(TEXT_BUFFER);
	testptr=fopen("testOutput2.txt","a");
	test_print_ALL_notTag(TEXT_BUFFER);
	return 0;
}