FILE *testptr;

//測試用函數
void test_print_ALL_Tag(char *nowPos){
	int len;
	if((nowPos==TEXT_TAIL)||(nowPos==NULL))return;
	nowPos=toStructTag(nowPos);
	if((len=tagLens(nowPos))){
		printf("%s",cutTagIn(nowPos));
		fprintf(testptr,"%s",cutTagIn(nowPos));
		freeBuf();
		test_print_ALL_Tag(nowPos+len);
	}
	else test_print_ALL_Tag(nowPos+1);
}

void test_print_ALL_notTag(char *nowPos){
	int len;
	if((nowPos==TEXT_TAIL)||(nowPos==NULL))return;
	if((*nowPos)=='<'){
		nowPos=toStructTag(nowPos);
		nowPos=findThisChar('>',nowPos);//之前誤用findThisWord(char *ch,char *rangeL,char *rangeR)出錯
		test_print_ALL_notTag(nowPos+1);
	}
	else{
		while((*nowPos)!='<'){
			fputc(*nowPos,testptr);
			nowPos++;
		}
		test_print_ALL_notTag(nowPos);
	}
}


/*-------------------------------------------------------------*/
