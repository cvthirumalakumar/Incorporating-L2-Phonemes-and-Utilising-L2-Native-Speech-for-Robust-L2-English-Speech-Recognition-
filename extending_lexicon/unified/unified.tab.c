/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 62 "unified.y"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include "utf8.c"
#include "utf8.h"
//#include <fstream.h>

enum LANG {
    MALAYALAM=1,TAMIL,TELUGU,KANNADA,HINDI,BENGALI,GUJARATHI,ODIYA,ENGLISH
}currLang;

#define ROW 128 //utf8 lang size max
#define COL 2
char *symbolTable[ROW][COL];
char *syllableList[100]={}; //max syllable limit.
int DEBUG=0, parseLevel=0, pruiningFlag=0, syllTagFlag=0, fliteHTSFlag=0, doubleParseSyl=0; directParseFlag=0;//parselevel - 0-mono 1-syll  //syldictFlag=0,
//wordpronunciation   wordpronunciationunified   commonunified
char *rootPath="./";
char *commonFile = "common", *outputFile, *outputPruneFile="rag_pho";//
//char phoneDelimiter ='*',syllableDelimiter = '#';
int writeFormat=0, LangSpecificCorrectionFlag=1; //0 - phone; 1 - syllable

char *wordCopy, *syllabifiedWord, *phonifiedWord, *unicodeWord, *syllabifiedWordOut, *outputText;
int langId=0,isSouth=0,syllableCount=0;

/* all phone list */
enum listSize{
 VOWELSSIZE=18 ,CONSONANTSSIZE=25, SEMIVOWELSSIZE=13
}lists;

char * VOWELS[VOWELSSIZE] = {"a","e","i","o","u","aa","mq","aa","ii", "uu","rq",
                              "au","ee","ei","ou","oo","ax","ai"};
char * CONSONANTS[CONSONANTSSIZE]={"k","kh","g","gh","ng","c","ch","j","jh","nj","tx",
                                    "txh","dx","dxh","nx","t","th","d","dh","n","p",
                                    "ph","b","bh","m"};
char * SEMIVOWELS[SEMIVOWELSSIZE] ={"y","r","l","w","sh","sx","zh","s","h","lx","rx",
                                    "f","dxq"};
char * VOWELMODIFERS[] = {};

/* all phone list end */

void yyerror(const char * str){
   fprintf(stderr,"Error: %s\n",str);
}
int yywrap(){
	return 1;
}

//Check if symbol is in symbolTable
int CheckSymbol(char * input){
	register int i=0;
	for(;i<ROW;i++)
	{
		if(strcmp(symbolTable[i][1],input)==NULL)return 1;
	}
	return 0;
}

// //Replace substr in string
// char * ReplaceString(const char *s, char ch, const char *repl) {
//     int count = 0;
//     const char *t;
//     for(t=s; *t; t++)
//         count += (*t == ch);
//     size_t rlen = strlen(repl);
//     char *res = malloc(strlen(s) + (rlen-1)*count + 1);
//     char *ptr = res;
//     for(t=s; *t; t++) {
//         if(*t == ch) {
//             memcpy(ptr, repl, rlen);
//             ptr += rlen;
//         } else {
//             *ptr++ = *t;
//         }
//     }
//     *ptr = 0;
//     return res;
// }

//Remove unwanted chars in UTF8 word initially -- make lang correction if necessary
char* RemoveUnwanted(char * input){
   char * output,output1;
   output=input;
   register int i=0;
   char c2[100];

   char * removeList[18]={"!",";",":","@","#","$","%","^","&","*",",",".","/","'","’","”","“","।"};
   for(;i<18;i++){
   	strcpy(c2,removeList[i]);
   	if(strstr(output, c2) != NULL)output =Utf8ReplaceAll(c2,"",output);
   }

   //invisible char correction -not working
	if(strstr(output, "﻿") != NULL)output =Utf8ReplaceAll("﻿","",output);

   //hindi correction
	if(strstr(output, "ऩ") != NULL)output =Utf8ReplaceAll("ऩ","ऩ",output);
   if(strstr(output, "ऱ") != NULL)output =Utf8ReplaceAll("ऱ","ऱ",output);
   if(strstr(output, "क़") != NULL)output =Utf8ReplaceAll("क़","क़",output);
   if(strstr(output, "ख़") != NULL)output =Utf8ReplaceAll("ख़","ख़",output);
   if(strstr(output, "ग़") != NULL)output =Utf8ReplaceAll("ग़","ग़",output);
   if(strstr(output, "ज़") != NULL)output =Utf8ReplaceAll("ज़","ज़",output);
   if(strstr(output, "ड़") != NULL)output =Utf8ReplaceAll("ड़","ड़",output);
   if(strstr(output, "ढ़") != NULL)output =Utf8ReplaceAll("ढ़","ढ़",output);
   if(strstr(output, "ढ़") != NULL)output =Utf8ReplaceAll("ढ़","ढ़",output);

   if(strstr(output, "फ़") != NULL)output =Utf8ReplaceAll("फ़","फ़",output);
   if(strstr(output, "य़") != NULL)output =Utf8ReplaceAll("य़","य़",output);
   if(strstr(output, "ऴ") != NULL)output =Utf8ReplaceAll("ऴ","ऴ",output);

   //tamil correction
   if(strstr(output, "ொ") != NULL)output =Utf8ReplaceAll("ொ","ொ",output);
   if(strstr(output, "ோ") != NULL)output =Utf8ReplaceAll("ோ","ோ",output);

   //malayalam correction
   if(strstr(output, "ൊ") != NULL)output =Utf8ReplaceAll("ൊ","ൊ",output);
   if(strstr(output, "ോ") != NULL)output =Utf8ReplaceAll("ോ","ോ",output);
   if(strstr(output, "ല്‍‌") != NULL)output =Utf8ReplaceAll("ല്‍‌","ൽ",output);
   if(strstr(output, "ള്‍") != NULL)output =Utf8ReplaceAll("ള്‍","ൾ",output);
   if(strstr(output, "ര്‍") != NULL)output =Utf8ReplaceAll("ര്‍","ർ",output);
   if(strstr(output, "ന്‍") != NULL)output =Utf8ReplaceAll("ന്‍","ൻ",output);
   if(strstr(output, "ണ്‍") != NULL)output =Utf8ReplaceAll("ണ്‍","ൺ",output);

   // printf("cleared checks\n");
   return	output;

}

//set the language id of the word
void SetlangId(int id){
   if(id>=3328 && id<=3455)currLang = MALAYALAM; //malayalam
   else if(id>=2944 && id<=3055)currLang =TAMIL; //tamil
   else if(id>=3202 && id<=3311)currLang =KANNADA; //KANNADA
   else if(id>=3072 && id<=3198)currLang =TELUGU; //telugu
   else if(id>=2304 && id<=2431)currLang =HINDI; //hindi
   else if(id>=2432 && id<=2559)currLang =BENGALI; //BENGALI
   else if(id>=2688 && id<=2815)currLang =GUJARATHI; //gujarathi
   else if(id>=2816 && id<=2943)currLang =ODIYA; //odia
   else if(id>=64 && id<=123)currLang = ENGLISH; //english
   langId = currLang;
   if(langId < 5)isSouth=1;
   if(langId == 0){
   	printf("UNKNOWN language  with id %d:(\n",id);
   	exit(0);
   }
   return 1;
}

//get the dict file name
char* GetFile(int langId,int type){//1-dict; 2-lang rules
   char * fileName;
   fileName= malloc(100*sizeof(char));
   strcpy(fileName, rootPath);
   if( type == 0 ){//common
   		strcat(fileName,commonFile);
   		return fileName;
   }
   if(type == 1)strcat(fileName,"dict/");
   else if(type == 2)strcat(fileName,"rules/");

   if(langId == MALAYALAM)strcat(fileName, "malayalam");
   else if(langId == TAMIL)strcat(fileName, "tamil");
   else if(langId == KANNADA)strcat(fileName, "kannada");
   else if(langId == TELUGU)strcat(fileName, "telugu");
   else if(langId == HINDI)strcat(fileName, "hindi");
   else if(langId == BENGALI)strcat(fileName, "bengali");
   else if(langId == GUJARATHI)strcat(fileName, "gujarathi");
   else if(langId == ODIYA)strcat(fileName, "odiya");
   else if(langId == ENGLISH)strcat(fileName, "english");

   if(type == 1)strcat(fileName,".dict");
   else if(type == 2)strcat(fileName,".rules");
   return fileName;
}

//set language features
int SetlanguageFeat(char* input){
   FILE *inFile;
   inFile = fopen(GetFile(0,0), "r");
   if (!inFile) {
      printf("Couldn't open common file for reading\n");
      return 0;
   }

   char line[200];
   char * line1;
   int i = -1;
   setlocale(LC_ALL, "");
   wchar_t * str1;
   char * firstLet;
   	str1 = (wchar_t*)(malloc(1000*sizeof(wchar_t)*strlen(input)));//giving more space -optimise
   	mbstowcs(str1,input,strlen(input)+1);
   int j;
   int len = wcslen(str1);
   if(len==0)len=1;

   //printf(" string : %ls\n", str1);
   for(j=0;j<len;j++)
   {
      if(str1[j]<8204)
      { // for invisible char skipping
         firstLet = str1[j];
         break;
      }
   }
	 //firstLet = str1[0];
	 //printf(" %s  len : %d\n",input,len);

   SetlangId((int)firstLet);

	wchar_t *buf=0;
	line[200];
	i = -1;
	int ii=0;
	while(fgets(line, 200, inFile))
	{
      // unicode  english
      if(i>=127)break; //end of commonfile
            i++;
      buf = strtok(line, "\t\n");
      //loop to select the language
      buf = strtok(NULL, "\t\n");
      if(buf) symbolTable[i][1] = strdup(buf);
      for(ii=0;ii<langId;ii++)
      {
         buf = strtok(NULL, "\t\n");
      }
      if(buf)symbolTable[i][0] =strdup(buf);
	}
	fclose(inFile);
   return 1;
}

//Appened two strings
char * AppendString(char * str1,char * str2 ){
   char * new_str ;
   if((new_str = malloc(strlen(str1)+strlen(str2)+1)) != NULL)
   {
      new_str[0] = '\0';   // ensures the memory is an empty string
      strcat(new_str,str1);
      strcat(new_str,str2);
   }
   else
   {
      printf("malloc failed!\n");
      // exit?
   }
   return new_str;
}

//concatenate two strings
char * ConcatenateString(char *str1, char *str2){
   register int i=0;
   char * str3 = malloc(1+(strlen(str1)+strlen(str2))) ;
   while(*str1)
   {
      str3[i++]=*str1++;
   }
   while(*str2)
   {
      str3[i++]=*str2++;
   }
   str3[i]='\0';
   return str3;
}

//UTF-8 to CPS mapping
char* ConvertToSymbols(char * input){
   char* output;
   setlocale(LC_ALL, "");
   wchar_t * str1;
	str1 = (wchar_t*)(malloc(3*sizeof(wchar_t)*strlen(input)));
	unicodeWord = (wchar_t*)(malloc(3*sizeof(wchar_t)*strlen(input)));
	syllabifiedWord = (wchar_t*)(malloc(100*sizeof(wchar_t)*strlen(input)));
   mbstowcs(str1,input,strlen(input)+1);
   register int j;
   int len = wcslen(str1);
   //
   // for(j=0;j<len;j++)
   // {
   //    printf("%d\n",(int)str1[j]);
   //    //if((int)str1[j] == 8205)str1[j] ='#';
   // }
   // printf("1 %ls\n",str1 );
   // if(strstr(str1, "#") != NULL)str1 = Utf8ReplaceAll("#","",str1);
   // printf("2 %ls\n",str1 );


   char uni[100];
   strcpy(syllabifiedWord,"&");

   for(j=0;j<len;j++){
      if((int)str1[j] < 8204 ){ //invisible char correction && ((int)str1[j] != 8204)
         sprintf(uni, "%d", (int)str1[j]%128);
         //unicodeWord = ConcatenateString(ConcatenateString(unicodeWord,"&"),uni);
         syllabifiedWord = ConcatenateString(ConcatenateString(syllabifiedWord,"&"),symbolTable[(int)str1[j]%128][1]);//getsymbol

      }
   }
   unicodeWord++;
   syllabifiedWord++;
   output = syllabifiedWord;
   return output;
}

//check the word in Dict
int CheckDictionary(char * input){
   FILE *output;
   char buffer[2000];
   wchar_t *buf=0;
   char * fileName=strdup(GetFile(langId,1));
   if(DEBUG)printf("dict : %s\n",fileName );
   output = fopen(fileName,"r");
   if(!output){
      if(DEBUG)printf("Dict not found\n");
      if(langId == ENGLISH) exit(1);
      return 0;
   }
   if(langId == ENGLISH){
   		int ii=0;
   		while(input[ii] != '\0'){
   			input[ii] = (int)input[ii]<97?tolower(input[ii]):input[ii];
   			ii++;
   		}
   }
   while (fgets(buffer,2000,output)!=NULL){
      buf = strtok(buffer, "\t\n");
      if(DEBUG)printf("word : %s\n",buf);
      if(strcmp(input,buf)==NULL){
         if(DEBUG)printf("match found\n" );
         buf = strtok(NULL, "\t\n");
         if(writeFormat == 1)WriteFile(buf);
         if(DEBUG)printf("Syllables : %s\n",buf);
         buf = strtok(NULL, "\t\n");
         if(writeFormat == 0)WriteFile(buf);
         if(DEBUG)printf("monophones : %s\n",buf);
         if(langId == ENGLISH) exit(1);
         return 1;
      }
   }
   fclose(output);
   // enum LANG corrLang;
   // corrLang = ENGLISH;
   return 0;
}


//Correct if there is a vowel in the middle
char* MiddleVowel(char * phone){
    register int i=0,j=0;
	char c1[100],c2[100];


    for(i=0;i<CONSONANTSSIZE;i++){
      for(j=0;j<VOWELSSIZE;j++){
      	strcpy(c1,"&"); strcpy(c2,"&");
        strcat(c1,CONSONANTS[i]);strcat(c2,CONSONANTS[i]);
        strcat(c1,"&"); strcat(c2,"&av&");
		strcat(c1,VOWELS[j]); strcat(c2,VOWELS[j]);        
		strcat(c1,"&"); strcat(c2,"&");
		         //if(DEBUG)printf("%s %s\n",c1,c2 );
      	if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
      	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
      }
	}
	

    for(i=0;i<SEMIVOWELSSIZE;i++){
      for(j=0;j<VOWELSSIZE;j++){
      	strcpy(c1,"&"); strcpy(c2,"&");
        strcat(c1,SEMIVOWELS[i]);strcat(c2,SEMIVOWELS[i]);
        strcat(c1,"&"); strcat(c2,"&av&");
		strcat(c1,VOWELS[j]); strcat(c2,VOWELS[j]);        
		strcat(c1,"&"); strcat(c2,"&");
		         //if(DEBUG)printf("%s %s\n",c1,c2 );
      	if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
      	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
      }
	}
  return phone;
}

//Syllabilfy the words
char* Syllabilfy(char * phone){

	if(strstr(phone, "&&") != NULL)phone =Utf8ReplaceAll("&&","&",phone);

	if(strstr(phone, "&eu&") != NULL)phone =Utf8ReplaceAll("&eu&","@",phone);
	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@","&eu&#&",phone);
   if(strstr(phone, "&euv&") != NULL)phone =Utf8ReplaceAll("&euv&","@",phone);
	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@","&euv&#&",phone);
	if(strstr(phone, "&avq") != NULL)phone =Utf8ReplaceAll("&avq","&q&av",phone);
	if(strstr(phone, "&av&") != NULL)phone =Utf8ReplaceAll("&av&","@",phone);
	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@","&av&#&",phone);
	if(strstr(phone, "&q") != NULL)phone =Utf8ReplaceAll("&q","@",phone);
	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@","&q&#",phone);

	register int i=0;
	char c1[100],c2[100];
	char * removeList[45]={"&nwv&","&nnv&","&rwv&","&lwv&","&lnv&","&aav&","&iiv&","&uuv&","&rqv&","&eev&",
                           "&eiv&","&ouv&","&axv&","&oov&","&aiv&","&auv&","&aev&",
                           "&nnx&","&nxx&","&rrx&","&llx&","&lxx&",
									"&aa&","&iv&","&ov&","&mq&","&aa&","&ii&","&uu&","&rq&","&au&","&ee&",
                           "&ei&","&ou&","&oo&","&ax&","&ai&","&ev&","&uv&","&ae&",
									"&a&","&e&","&i&","&o&","&u&"};

	for(;i<45;i++){
      strcpy(c1,removeList[i]);
      strcpy(c2,c1);
      strcat(c2,"#&");
      if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
      if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
	}

  if(strstr(phone, "&#&hq&") != NULL)phone =Utf8ReplaceAll("&#&hq&","&hq&#&",phone);

  //for vowel in between correction

  char * pureVowelList[5] ={"&a&","&e&","&i&","&o&","&u&"};
  for(i=0;i<5;i++){
    strcpy(c1,"&#");
    strcat(c1,pureVowelList[i]);
    if(strstr(phone, pureVowelList[i]) != NULL)phone =Utf8ReplaceAll(pureVowelList[i],"@",phone);
    if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c1,phone);
  }

  char * consonantList[39]={"k","kh","g","gh","ng","c","ch","j","jh","nj","tx","txh","dx","dxh",
                                                   "nx","t","th","d","dh","n","p","ph","b","bh","m","y",
                                                   "r","l","w","sh","sx","zh","y","s","h","lx","rx","f","dxq"};

  // &eu&#&r&eu&#& syllabification correction

  for(i=0;i<39;i++){
    strcpy(c1,"&eu&#&");
    strcat(c1,consonantList[i]);
    strcat(c1,"&euv&#&");
    strcpy(c2,"&eu&");
    strcat(c2,consonantList[i]);
    strcat(c2,"&av&#&");
    if(strstr(phone, c1) != NULL)phone = Utf8ReplaceAll(c1,"@",phone);
    if(strstr(phone, "@") != NULL)phone = Utf8ReplaceAll("@",c2,phone);
  }

  for(i=0;i<39;i++){
    strcpy(c1,"&euv&#&");
    strcat(c1,consonantList[i]);
    strcat(c1,"&euv&#&");
    strcpy(c2,"&euv&");
    strcat(c2,consonantList[i]);
    strcat(c2,"&av&#&");
    if(strstr(phone, c1) != NULL)phone = Utf8ReplaceAll(c1,"@",phone);
    if(strstr(phone, "@") != NULL)phone = Utf8ReplaceAll("@",c2,phone);
  }
  if(strstr(phone, "&eu&") != NULL)phone =Utf8ReplaceAll("&eu&","@",phone);
  if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@","&eu&#&",phone);

  return phone;
}



//cant use this as break syllable rules
char* DoubleModifierCorrection(char * phone){
   //&iv&#&aav&  DoubleModifierCorrection
	register int i=0,j=0;
	char c1[100],c2[100];
	char * doubleModifierList[21]={"&nwv&","&nnv&","&rwv&","&lwv&","&lnv&","&aav&","&iiv&","&uuv&","&rqv&","&eev&",
                           "&eiv&","&ouv&","&axv&","&oov&","&aiv&","&auv&","&aev&",
									"&iv&","&ov&","&ev&","&uv&"};
	for(i=0;i<21;i++){
      for(j=0;j<21;j++){
         strcpy(c1,doubleModifierList[i]);
      	strcpy(c2,doubleModifierList[i]);
         strcat(c1,"#");
         strcat(c1,doubleModifierList[j]);
      	strcat(c2,doubleModifierList[j]);
      	strcat(c2,"#&");
         //if(DEBUG)printf("%s %s\n",c1,c2 );
      	if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
      	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
      }
	}

   if(strstr(phone, "&#&hq&") != NULL)phone =Utf8ReplaceAll("&#&hq&","&hq&#&",phone);
   if(strstr(phone, "&&") != NULL)phone =Utf8ReplaceAll("&&","&",phone);
   return phone;
}

//for eu&C&C&V
char* SchwaDoubleConsonent(char * phone){
   register int i=0,j=0,k=0;
   char c1[100],c2[100];
   char * consonentList[39]={"k","kh","lx","rx","g","gh","ng","c","ch","j","jh","nj","tx","txh","dx","dxh","nx","t","th","d","dh","n","p","ph","b","bh","m","y","r","l","w","sh","sx","zh","y","s","h","f","dxq"};
   char * vowelList[42]={"av&","nwv&","nnv&","rwv&","lwv&","lnv&","aav&","iiv&","uuv&","rqv&","eev&","eiv&","ouv&",
                                                    "axv&","oov&","aiv&","nnx&","nxx&","rrx&","llx&","lxx&",
                                                    "aa&","iv&","ov&","mq&","aa&","ii&","uu&","rq&",
                                                    "ee&","ei&","ou&","oo&","ax&","ai&","ev&","uv&",
                                                    "a&","e&","i&","o&","u&"};
   for(i=0;i<39;i++){
      for(j=0;j<39;j++){
         for(k=0;k<42;k++){
            strcpy(c1,"&euv&");
            strcat(c1,consonentList[i]);
            strcpy(c2,c1);
            strcat(c1,"&");
            strcat(c2,"&av&");
            strcat(c1,consonentList[j]);
            strcat(c2,consonentList[j]);
            strcat(c2,"&");
            strcat(c1,"&");
            strcat(c1,vowelList[k]);
            strcat(c2,vowelList[k]);

            //if(DEBUG)printf("%s\t%s\n",c1,c2);
            if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
            if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
         }
      }
   }
  //if(strstr(phone, "#") != NULL)phone =Utf8ReplaceAll("#","",phone);
  if(strstr(phone, "$") != NULL)phone =Utf8ReplaceAll("$","",phone);
  return phone;
}

//halant specific correction for aryan langs
char* SchwaSpecificCorrection(char * phone){
   register int i=0,j;
   char c1[100],c2[100],c3[100],c4[100];
   char * schwaList[38]={"k","kh","g","gh","ng","c","ch","j","jh","nj","tx","txh","dx","dxh",
                                                    "nx","t","th","d","dh","n","p","ph","b","bh","m","y",
                                                    "r","l","s","w","sh","sx","zh","h","lx","rx","f","dxq"};
   char * vowelList[42]={"av&","nwv&","nnv&","rwv&","lwv&","lnv&","aav&","iiv&","uuv&","rqv&","eev&","eiv&","ouv&",
                                                    "axv&","oov&","aiv&","nnx&","nxx&","rrx&","llx&","lxx&",
                                                    "aa&","iv&","ov&","mq&","aa&","ii&","uu&","rq&",
                                                    "ee&","ei&","ou&","oo&","ax&","ai&","ev&","uv&",
                                                    "a&","e&","i&","o&","u&"};



   if(DEBUG)printf("%d\n", strlen(phone));
   int len=strlen(phone);
   phone[len]='!';
   phone[len+1]='\0';
   if(DEBUG)printf("phone cur%s\n",phone);
   // for end correction &av&t&aav&
   for(i=0;i<38;i++){
      for(j=1;j<42;j++){//dont want av
         strcpy(c1,"&av&");
         strcat(c1,schwaList[i]);
         strcat(c1,"&");
         strcat(c1,vowelList[j]);
         strcat(c1,"!");
         strcpy(c2,"&euv&");
         strcat(c2,schwaList[i]);
         strcat(c2,"&");
         strcat(c2,vowelList[j]);
         strcat(c2,"!");
         if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
         if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
      }
      strcat(c1,schwaList[i]);
   }
   if(strstr(phone, "!") != NULL)phone =Utf8ReplaceAll("!","",phone);

    for(i=0;i<38;i++){
      strcpy(c1,"&av&");
      strcat(c1,schwaList[i]);
      strcat(c1,"&av&");
      strcpy(c2,"&euv$&");
      strcat(c2,schwaList[i]);
      strcat(c2,"&av$&");
      if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
      if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
    }
    if(DEBUG)printf("inside schwa%s\n", phone);
   //  for(j=0;j<42;j++)
   //  {
   //     strcpy(c4,"&eu&");
   //     strcat(c4,c3);
   //     strcat(c4,"$");
   //  	strcat(c4,vowelList[j]);
   //  	strcpy(c2,c1);
   //  	strcat(c2,vowelList[j]);
   //    for(;i<39;i++)
   //    {
   //       strcpy(c1,"&av&");
   //       strcat(c1,schwaList[i]);
   //       strcat(c1,"&");
   //       strcpy(c3,"&");
   //       strcat(c3,schwaList[i]);
   //       strcat(c3,"&");
   //        //if(DEBUG)printf("%s\t%s\n",c2,c4);
   //     	if(strstr(phone, c2) != NULL)phone =Utf8ReplaceAll(c2,"@",phone);
   //     	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c4,phone);
   //     }
   //  }

   for(i=0;i<38;i++){
      strcpy(c1,"&av&");
      strcat(c1,schwaList[i]);
      strcat(c1,"&");
      strcpy(c3,"&");
      strcat(c3,schwaList[i]);
      strcat(c3,"&");
      for(j=0;j<41;j++){
         strcpy(c4,"&euv&");
         strcat(c4,c3);
         strcat(c4,"$");
      	strcat(c4,vowelList[j]);
      	strcpy(c2,c1);
      	strcat(c2,vowelList[j]);
         //if(DEBUG)printf("%s\t%s\n",c2,c4);
      	if(strstr(phone, c2) != NULL)phone =Utf8ReplaceAll(c2,"@",phone);
      	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c4,phone);
      }
   }
   //if(strstr(phone, "#") != NULL)phone =Utf8ReplaceAll("#","",phone);
   if(strstr(phone, "$") != NULL)phone =Utf8ReplaceAll("$","",phone);


   //&q&w&eu&
   for(i=0;i<39;i++){
      strcpy(c1,"&q&");
      strcat(c1,schwaList[i]);
      strcat(c1,"&euv&");
      strcpy(c2,"&q&");
      strcat(c2,schwaList[i]);
      strcat(c2,"&av&");
      if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
      if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
   }

   // for(i=0;i<39;i++){
   //    strcpy(c1,"&eu&");
   //    strcat(c1,schwaList[i]);
   //    strcat(c1,"&eu&");
   //    strcpy(c2,"&eu&");
   //    strcat(c2,schwaList[i]);
   //    strcat(c2,"&av&");
   //    if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
   //    if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
   //
   // }

   return phone;
}

//checnk if vowel is in input. 'q' special case, 'rq' special case
int CheckVowel(char * input, int q, int rq){
   if(strstr(input,"a")!=NULL)return 1;
   if(strstr(input,"e")!=NULL)return 1;
   if(strstr(input,"i")!=NULL)return 1;
   if(strstr(input,"o")!=NULL)return 1;
   if(strstr(input,"u")!=NULL)return 1;
   if(q)if(strstr(input,"q")!=NULL)return 1;
   if(rq)if(strstr(input,"rq")!=NULL)return 1;
   return 0;
}

int Checkeuv(char * input){
   if(strstr(input,"euv")!=NULL)return 1;
   return 0;
}

//check single vowels
int CheckSingleVowel(char input,int q){
   if(input == 'a') return 1;
   if(input == 'e') return 1;
   if(input == 'i') return 1;
   if(input == 'o') return 1;
   if(input == 'u') return 1;
   if(q)if(input == 'q') return 1;
   return 0;
}

//get the type of phone in the position
int GetPhoneType(char * input, int pos){
   char * phone =strdup(input);
   char * pch;
   int count=1,i=0;
   pch = strtok(phone,"&");
   while (pch != NULL && pch != "")
   {
      if(count++>=pos)break;
      pch = strtok (NULL, "&");
   }
   if(DEBUG)printf("str : %s %d\n ",pch,GetType(pch));
   return GetType(pch);
}

//get the type of given input
int GetType(char * input){
   int i=0;
   for(i=0;i<VOWELSSIZE;i++)if(strcmp(VOWELS[i],input)==NULL)return 1;
   for(i=0;i<CONSONANTSSIZE;i++)if(strcmp(CONSONANTS[i],input)==NULL)return 2;
   for(i=0;i<SEMIVOWELSSIZE;i++)if(strcmp(SEMIVOWELS[i],input)==NULL)return 3;
   return 0;
}

//check if chillaksharas are there --for malayalam
int CheckChillu(char * input){
   if(strstr(input,"nwv")!=NULL)return 1;
   if(strstr(input,"nnv")!=NULL)return 1;
   if(strstr(input,"rwv")!=NULL)return 1;
   if(strstr(input,"lwv")!=NULL)return 1;
   if(strstr(input,"lnv")!=NULL)return 1;
   return 0;
}

//get UTF-8 from CPS
char * GetUTF(char * input){
   register int i=0;
   for(;i<ROW;i++)
   {
   	if(strcmp(symbolTable[i][1],input)==0)
   	{
         return symbolTable[i][0];
   	}
   }
   return 0;
}

//verify the letter is english char -- CLS
int isEngLetter(char * p){
   if((int)p >= 97 && (int)p <= 122)return 1;
   else return 0;
}

//remove unwanted Symbols from word
char * CleanseWord(char * phone){
   register int i=0;
   while(phone[i] != '\0'){
      if(!(phone[i] =='&') && !isEngLetter(phone[i])){
         phone[i] = '#';
      }
      i++;
   }
   phone[i]='\0';
   //if(strstr(phone, "#") != NULL)phone =Utf8ReplaceAll("#","",phone);
   if(strstr(phone, "$") != NULL)phone =Utf8ReplaceAll("$","",phone);
   if(strstr(phone, "&&") != NULL)phone =Utf8ReplaceAll("&&","&",phone);
   return phone;
}

//correct the geminate syllabification ,isReverse --reverse correction
char* GeminateCorrection(char * phone ,int isReverse){
   register int i=0;
   char c1[100],c2[100];
   char * geminateList[39]={"k","kh","lx","rx","g","gh","ng","c","ch","j","jh","nj","tx","txh","dx","dxh","nx","t","th","d","dh","n","p","ph","b","bh","m","y","r","l","w","sh","sx","zh","y","s","h","f","dxq"};
   for(;i<39;i++){
      strcpy(c1,"&");
      strcat(c1,geminateList[i]);
      strcat(c1,"&eu&");//check for av also
      strcat(c1,geminateList[i]);
      strcat(c1,"&");
      strcpy(c2,"&");
      strcat(c2,geminateList[i]);
      strcat(c2,"&");
      strcat(c2,geminateList[i]);
      strcat(c2,"&");
      //if(DEBUG)printf("%s %s\n",c1 ,c2 );
      if(isReverse){
         if(strstr(phone, c2) != NULL)phone =Utf8ReplaceAll(c2,c1,phone);
      }else{
         if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,c2,phone);
      }
   }
   return phone;
}


int CountChars( char* s, char c ){
   return *s == '\0'? 0: CountChars( s + 1, c ) + (*s == c);
}

int bi=0;
char * leftStr[1000];
char * rightStr[1000];

void GenerateAllCombinations(int j,char * s,char * c[256][256],int isRight){
   int i=0;
   char *t;
   t=malloc(1000);
   if(c[j][0][0]=='#'){
      if(isRight){
         rightStr[bi]=malloc(30);
         strcpy(rightStr[bi],s);
         strcat(rightStr[bi],"&");
         bi++;
      }else{
         leftStr[bi]=malloc(30);
         strcpy(leftStr[bi],s);
         strcat(leftStr[bi],"&");
         bi++;
      }
   }else{
      for(i=0;c[j][i][0]!='#';i++){
         strcpy(t,s);
         strcat(t,"&");
         strcat(t,c[j][i]);
         GenerateAllCombinations(j+1,t,c,isRight);
      }
   }
   free(t);
}

void GenerateMatrix(char * combMatrix[256][256],char * regex){
   int row=0,col=0;
   combMatrix[0][0] = malloc(10);
   for(int i=0,item=0;i<strlen(regex);i++)
   {
      if(regex[i]=='&'){
         combMatrix[row][col+1] = malloc(10);
         strcpy(combMatrix[row][col+1],"#");
         row++;col=0;item=0;
         combMatrix[row][col] = malloc(10);
      }else if(regex[i]=='|'){
         col++;item=0;
         combMatrix[row][col] = malloc(10);
      }else{
         combMatrix[row][col][item++]=regex[i];
      }
      if(DEBUG)printf("%d %d %s\n",row,col,combMatrix[row][col]);
   }
   combMatrix[row][col+1] = malloc(10);
   strcpy(combMatrix[row][col+1],"#");
   combMatrix[row+1][0] = malloc(10);
   strcpy(combMatrix[row+1][0],"#");
}

char * CombinationCorrection(char * phone, char * left, char * right, int isReverse){
   char * leftComb[256][256];
   char * rightComb[256][256];
   GenerateMatrix(leftComb,left);
   GenerateMatrix(rightComb,right);
   bi=0;GenerateAllCombinations(0,"",leftComb,0);
   bi=0;GenerateAllCombinations(0,"",rightComb,1);

   int i=0;
   while(leftStr[i]!= NULL){
      if(isReverse){
         if(strstr(phone, rightStr[i]) != NULL)phone =Utf8ReplaceAll(rightStr[i],"!",phone);
         if(strstr(phone, "!") != NULL)phone =Utf8ReplaceAll("!",leftStr[i],phone);
      }else{
         if(DEBUG)printf("%s %s\n",leftStr[i],rightStr[i]);
         if(strstr(phone, leftStr[i]) != NULL)phone =Utf8ReplaceAll(leftStr[i],"!",phone);
         if(strstr(phone, "!") != NULL)phone =Utf8ReplaceAll("!",rightStr[i],phone);
      }
      i++;
   }
   for(int i=0;i<1000;i++){
      free(rightStr[i]);free(leftStr[i]);
   }
   for(int i=0;i<256;i++){
      for(int j=0;j<256;j++){
      free(leftComb[i][j]);free(rightComb[i][j]);
   }
   }
   return phone;
}

char * PositionCorrection(char * phone, char * left, char * right, int isReverse){
   register int i=0;
   char * c1,* c2;
   c1=malloc(100);
   c2=malloc(100);
   char * geminateList[39]={"k","kh","lx","rx","g","gh","ng","c","ch","j","jh","nj","tx","txh","dx","dxh","nx","t","th","d","dh","n","p","ph","b","bh","m","y","r","l","w","sh","sx","zh","y","s","h","f","dxq"};
   for(;i<39;i++){
      c1=strdup(left);
      c2=strdup(right);
      c1=Utf8ReplaceAll("@",geminateList[i],c1);
      c2=Utf8ReplaceAll("@",geminateList[i],c2);
      //if(DEBUG)printf("%s %s\n",c1 ,c2 );
      if(isReverse){
         if(strstr(phone, c2) != NULL)phone =Utf8ReplaceAll(c2,c1,phone);
      }else{
         if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,c2,phone);
      }
   }
   free(c1);free(c2);
   return phone;
}


//Language specific corrections
char * LangSpecificCorrection(char * phone, int langSpecFlag){
   //printf("Testinit\n");
   if(isSouth){
      if(strstr(phone, "&ei&") != NULL)phone =Utf8ReplaceAll("&ei&","&ai&",phone);
      if(strstr(phone, "&eiv&") != NULL)phone =Utf8ReplaceAll("&eiv&","&aiv&",phone);
   }else{
      if(strstr(phone, "&oo&") != NULL)phone =Utf8ReplaceAll("&oo&","&o&",phone);
      if(strstr(phone, "&oov&") != NULL)phone =Utf8ReplaceAll("&oov&","&ov&",phone);
   }
   // 'q' corrections for all langs
   //if(strstr(phone, "&a&hq&") != NULL)phone =Utf8ReplaceAll("&a&hq&","&hq&",phone);
   if(strstr(phone, "&q&") != NULL)phone =Utf8ReplaceAll("&q&","!",phone);
   if(strstr(phone, "!") != NULL)phone =Utf8ReplaceAll("!","&av&q&",phone);
   if(strstr(phone, "&a&av&") != NULL)phone =Utf8ReplaceAll("&a&av&","&a&",phone);
   if(strstr(phone, "&e&av&") != NULL)phone =Utf8ReplaceAll("&e&av&","&e&",phone);
   if(strstr(phone, "&i&av&") != NULL)phone =Utf8ReplaceAll("&i&av&","&i&",phone);
   if(strstr(phone, "&o&av&") != NULL)phone =Utf8ReplaceAll("&o&av&","&o&",phone);
   if(strstr(phone, "&u&av&") != NULL)phone =Utf8ReplaceAll("&u&av&","&u&",phone);
   if(strstr(phone, "&a&rqv&") != NULL)phone =Utf8ReplaceAll("&a&rqv&","&rq&",phone);
   if(strstr(phone, "&aa&av&") != NULL)phone =Utf8ReplaceAll("&aa&av&","&aa&",phone);
   if(strstr(phone, "&ae&av&") != NULL)phone =Utf8ReplaceAll("&ae&av&","&ae&",phone);
   if(strstr(phone, "&ax&av&") != NULL)phone =Utf8ReplaceAll("&ax&av&","&ax&",phone);
   if(strstr(phone, "&ee&av&") != NULL)phone =Utf8ReplaceAll("&ee&av&","&ee&",phone);
   if(strstr(phone, "&ii&av&") != NULL)phone =Utf8ReplaceAll("&ii&av&","&ii&",phone);
   if(strstr(phone, "&ai&av&") != NULL)phone =Utf8ReplaceAll("&ai&av&","&ai&",phone);
   if(strstr(phone, "&au&av&") != NULL)phone =Utf8ReplaceAll("&au&av&","&au&",phone);
   if(strstr(phone, "&oo&av&") != NULL)phone =Utf8ReplaceAll("&oo&av&","&oo&",phone);
   if(strstr(phone, "&uu&av&") != NULL)phone =Utf8ReplaceAll("&uu&av&","&uu&",phone);
   if(strstr(phone, "&rq&av&") != NULL)phone =Utf8ReplaceAll("&rq&av&","&rq&",phone);
   if(strstr(phone, "&av&av&") != NULL)phone =Utf8ReplaceAll("&av&av&","&av&",phone);
   if(strstr(phone, "&ev&av&") != NULL)phone =Utf8ReplaceAll("&ev&av&","&ev&",phone);
   if(strstr(phone, "&iv&av&") != NULL)phone =Utf8ReplaceAll("&iv&av&","&iv&",phone);
   if(strstr(phone, "&ov&av&") != NULL)phone =Utf8ReplaceAll("&ov&av&","&ov&",phone);
   if(strstr(phone, "&uv&av&") != NULL)phone =Utf8ReplaceAll("&uv&av&","&uv&",phone);
   if(strstr(phone, "&av&rqv&") != NULL)phone =Utf8ReplaceAll("&av&rqv&","&rqv&",phone);
   if(strstr(phone, "&aav&av&") != NULL)phone =Utf8ReplaceAll("&aav&av&","&aav&",phone);
   if(strstr(phone, "&aev&av&") != NULL)phone =Utf8ReplaceAll("&aev&av&","&aev&",phone);
   if(strstr(phone, "&auv&av&") != NULL)phone =Utf8ReplaceAll("&auv&av&","&auv&",phone);
   if(strstr(phone, "&axv&av&") != NULL)phone =Utf8ReplaceAll("&axv&av&","&axv&",phone);
   if(strstr(phone, "&aiv&av&") != NULL)phone =Utf8ReplaceAll("&aiv&av&","&aiv&",phone);
   if(strstr(phone, "&eev&av&") != NULL)phone =Utf8ReplaceAll("&eev&av&","&eev&",phone);
   if(strstr(phone, "&eiv&av&") != NULL)phone =Utf8ReplaceAll("&eiv&av&","&eiv&",phone);
   if(strstr(phone, "&iiv&av&") != NULL)phone =Utf8ReplaceAll("&iiv&av&","&iiv&",phone);
   if(strstr(phone, "&oov&av&") != NULL)phone =Utf8ReplaceAll("&oov&av&","&oov&",phone);
   if(strstr(phone, "&ouv&av&") != NULL)phone =Utf8ReplaceAll("&ouv&av&","&ouv&",phone);
   if(strstr(phone, "&uuv&av&") != NULL)phone =Utf8ReplaceAll("&uuv&av&","&uuv&",phone);
   if(strstr(phone, "&rqv&av&") != NULL)phone =Utf8ReplaceAll("&rqv&av&","&rqv&",phone);

   if(!langSpecFlag)return phone;

   char buffer[1000];
   char *buf;
   FILE *output;
   char * fileName=strdup(GetFile(langId,2));
   //if(DEBUG)printf("rules file : %s\n",fileName );
   output = fopen(fileName,"r");
   if(!output){
      if(DEBUG)printf("rules not found\n");
      return phone;
   }
   char left[1000], right[1000];
   //update head and tail in phone
   char * tempstr =strdup(phone);
   strcpy(phone,"^");
   strcat(phone,tempstr);
   strcat(phone,"$");
   //printf("test1");
   if(DEBUG)printf("phone : %s\n",phone );
   while (fgets(buffer,1000,output)!=NULL){
      if(strstr(buffer,"#")!=NULL || strlen(buffer)<5)continue;
      buf = strtok(buffer, "\t\n");
      strcpy(left,buf);
      buf = strtok(NULL, "\t\n");
      strcpy(right,buf);
      //if(writeFormat == 1)WriteFile(buf);
      //if(DEBUG)printf("left right : %s   %s\n",left ,right);
      //operate
      if(strstr(left,"|")!=NULL){
         char * a1=strdup(left);a1++;a1[strlen(a1)-1] = 0;
         char * a2=strdup(right);a2++;a2[strlen(a2)-1] = 0;
         if(DEBUG)printf("%s\t %s\n",a1,a2 );
         phone = CombinationCorrection(phone,a1,a2,0);
         free(a1);free(a2);
      }else if(strstr(left,"@")!=NULL){
         phone = PositionCorrection(phone,left,right,0);
      }else{
         if(strstr(phone, left) != NULL)phone =Utf8ReplaceAll(left,"!",phone);
         if(strstr(phone, "!") != NULL)phone =Utf8ReplaceAll("!",right,phone);
      }
   }
   //remove head and tail in phone
   if(strstr(phone, "^") != NULL)phone =Utf8ReplaceAll("^","",phone);
   if(strstr(phone, "$") != NULL)phone =Utf8ReplaceAll("$","",phone);
   //if(DEBUG)printf("phone : %s\n",phone );
   fclose(output);
   //return phone;

   int i=0,count=0;
   for(;i<strlen(phone);i++)//end correction
   {
      if(phone[i] == '&')count=i;
   }
   phone[count+1]='\0';
   //free(buf);free(tempstr);

   return phone;
}

//Reverse syllable correction for syllable parsing
char* SyllableReverseCorrection(char * phone, int langSpecFlag){
   if(isSouth){
      if(strstr(phone, "&ai&") != NULL)phone =Utf8ReplaceAll("&ai&","&ei&",phone);
      if(strstr(phone, "&aiv&") != NULL)phone =Utf8ReplaceAll("&aiv&","&eiv&",phone);
   }else{
      if(strstr(phone, "&o&") != NULL)phone =Utf8ReplaceAll("&o&","&oo&",phone);
      if(strstr(phone, "&ov&") != NULL)phone =Utf8ReplaceAll("&ov&","&oov&",phone);
   }
   if(!langSpecFlag)return phone;

   char buffer[1000];
   char *buf=0;
   FILE *output;
   char * fileName=strdup(GetFile(langId,2));
   //if(DEBUG)printf("rules file : %s\n",fileName );
   output = fopen(fileName,"r");
   if(!output){
      if(DEBUG)printf("rules not found\n");
      return phone;
   }
   char left[1000], right[1000];
   //update head and tail in phone
   char * tempstr =strdup(phone);
   strcpy(phone,"^");
   strcat(phone,tempstr);
   strcat(phone,"$");
   if(DEBUG)printf("before phone : %s\n",phone );
   while (fgets(buffer,1000,output)!=NULL){
      if(strstr(buffer,"#")!=NULL || strlen(buffer)<5)continue;
      buf = strtok(buffer, "\t\n");
      strcpy(left,buf);
      buf = strtok(NULL, "\t\n");
      strcpy(right,buf);
      //if(writeFormat == 1)WriteFile(buf);
      //if(DEBUG)printf("left right : %s   %s\n",left ,right);
      //operate
      if(strstr(left,"|")!=NULL){
         //printf("1\n");
         char * a1=strdup(left);a1++;a1[strlen(a1)-1] = 0;
         char * a2=strdup(right);a2++;a2[strlen(a2)-1] = 0;
         //if(DEBUG)printf("%s\t %s\n",a1,a2 );
         phone = CombinationCorrection(phone,a1,a2,1);
      }else if(strstr(left,"@")!=NULL){
         phone = PositionCorrection(phone,left,right,1);
      }else{
         //printf("3\n");
         if(strstr(phone, right) != NULL)phone =Utf8ReplaceAll(right,"!",phone);
         if(strstr(phone, "!") != NULL)phone =Utf8ReplaceAll("!",left,phone);
      }
   }
   //remove head and tail in phone
   if(strstr(phone, "^") != NULL)phone = Utf8ReplaceAll("^","",phone);
   if(strstr(phone, "$") != NULL)phone = Utf8ReplaceAll("$","",phone);
   if(DEBUG)printf("after phone : %s\n",phone );
   fclose(output);

   // enum LANG corrLang;
   // corrLang = TAMIL;
   // if(langId == corrLang) //tamil
   // {
   //    if(DEBUG)printf("Tamil reverse corrections\n" );
   //    if(strstr(phone, "&f&") != NULL)phone =Utf8ReplaceAll("&f&","&hq&p&",phone);
   // }
   // corrLang = MALAYALAM;
   // if(langId == corrLang) //malayalam
   // {
   //    if(DEBUG)printf("malayalam reverse corrections\n" );
   //    if(strstr(phone, "&au&") != NULL)phone =Utf8ReplaceAll("&au&","&ou&",phone);
   // }
   // corrLang = KANNADA;
   // if(langId == corrLang) //kannada
   // {
   //    if(DEBUG)printf("kannada reverse corrections\n" );
   //    if(strstr(phone, "&au&") != NULL)phone =Utf8ReplaceAll("&au&","&ou&",phone);
   // }
   // corrLang = TELUGU;
   // if(langId == corrLang) //kannada
   // {
   //    if(DEBUG)printf("telugu reverse corrections\n" );
   //    if(strstr(phone, "&au&") != NULL)phone =Utf8ReplaceAll("&au&","&ou&",phone);
   // }
   return phone;
}

//language specific syllable correction
int LangSyllableCorrection(char * input){
   if(strcmp(input,"&av&q&")==NULL)return 1;
   else return 0;
}

//split into syllable array
int SplitSyllables(char * input){
   char * pch;
   int i=0,len;

   if(writeFormat == 2){
   		register int i=0,j=0;
		char c1[100],c2[100];
		char * fullList[39]={"k","kh","lx","rx","g","gh","ng","c","ch","j","jh","nj","tx","txh","dx","dxh","nx","t","th","d","dh","n","p","ph","b","bh","m","y","r","l","w","sh","sx","zh","y","s","h","f","dxq"};

	    for(i=0;i<39;i++){
	      for(j=0;j<39;j++){
	      	strcpy(c1,"&"); strcpy(c2,"&");
	        strcat(c1,fullList[i]);strcat(c2,fullList[i]);
	        strcat(c1,"&"); strcat(c2,"&euv&#&");
			strcat(c1,fullList[j]); strcat(c2,fullList[j]);        
			strcat(c1,"&"); strcat(c2,"&");
			         //if(DEBUG)printf("%s %s\n",c1,c2 );
	      	if(strstr(input, c1) != NULL)input =Utf8ReplaceAll(c1,"@",input);
	      	if(strstr(input, "@") != NULL)input =Utf8ReplaceAll("@",c2,input);
	      }
		}
   }

   if(strstr(input, "&#&mq&") != NULL)input =Utf8ReplaceAll("&#&mq&","&mq&",input);
   if(strstr(input, "&#&q&") != NULL)input =Utf8ReplaceAll("&#&q&","&q&",input);


   //printf("aksh : %s\n", input);
   //exit(0);

   pch = strtok(input,"#");
   //round 1 split
   while (pch != NULL){
      if(strcmp(pch,"&")){
      	syllableList[i] = strdup(pch);
      	i++;
      }
      pch = strtok (NULL, "#");
   }
   len =i;
   if(DEBUG){
      for (i=0;i<len;i++){
      		printf("initStack : %s\n",syllableList[i] );
      }
   }
   //isSouth &&
	if(!CheckVowel(syllableList[len-1],1,0)&& !CheckChillu(syllableList[len-1])){//south specific av addition
      if(isSouth){
      	syllableList[len-1] = AppendString(syllableList[len-1],"&av&");
      }else{
         syllableList[len-1] = AppendString(syllableList[len-1],"&euv&");
      }
	}
   //round 2 correciton
	len = i;

	if(writeFormat == 2){
		   syllableCount=len;
		   writeFormat = 1;
		   return 1;
	}


   int euFlag=1;
	if(i>1)
   {

      for(i--;i>=0;i--)
      {
         if(LangSyllableCorrection(syllableList[i])){
            strcat(syllableList[i-1],syllableList[i]);
            syllableList[i]="";
         }
      	if(strstr(syllableList[i],"&eu&")!= NULL){
      		syllableList[i] =Utf8ReplaceAll("&eu&","!",syllableList[i]);
            euFlag=1;
      	}
         if(strstr(syllableList[i],"&euv&")!= NULL){
      		syllableList[i] =Utf8ReplaceAll("&euv&","!",syllableList[i]);
            euFlag=2;
      	}
      	if(!CheckVowel(syllableList[i],0,1)){
      		if(i-1>=0){
      			strcat(syllableList[i-1],syllableList[i]);
      			syllableList[i]="";
      		}else{
      			strcat(syllableList[i],syllableList[i+1]);
      			syllableList[i+1]="";
      		}
      	}
      	if(i-1>0){
      		if(euFlag==1)if(strstr(syllableList[i-1], "!") != NULL)syllableList[i-1] =Utf8ReplaceAll("!","&eu&",syllableList[i-1]);
            if(euFlag==2)if(strstr(syllableList[i-1], "!") != NULL)syllableList[i-1] =Utf8ReplaceAll("!","&euv&",syllableList[i-1]);
      		if(strstr(syllableList[i-1], "&&") != NULL)syllableList[i-1] =Utf8ReplaceAll("&&","&",syllableList[i-1]);
      	}
      	if(euFlag==1)if(strstr(syllableList[i], "!") != NULL)syllableList[i] =Utf8ReplaceAll("!","&eu&",syllableList[i]);
         if(euFlag==2)if(strstr(syllableList[i], "!") != NULL)syllableList[i] =Utf8ReplaceAll("!","&euv&",syllableList[i]);
      }
	}else{
		printf(" syll 0 %s\n", syllableList[0] );
		if((!CheckVowel(syllableList[0],1,0) &&  writeFormat !=3 )|| Checkeuv(syllableList[0])){
			
         strcat(syllableList[0],"&av");
		}
	}
	int j=0;
   //temp fix e1 to eu
   //if(strstr(syllableList[0], "&e1") != NULL)syllableList[0] =Utf8ReplaceAll("&e1","&euv&",syllableList[0]);

	if(DEBUG){
   	for (i=0;i<len;i++)
   	{
         printf("syllablifiedStack : %s\n",syllableList[i] );
   	}
	}
   //round 3 double syllable correction
   for (i=0;i<len;i++)
   {
      //corrections
      syllableList[i] =Utf8ReplaceAll("1","",syllableList[i]);
      //syllableList[i] =CleanseWord(syllableList[i]);
      if(DEBUG)printf("LenStack : %d\n",strlen(syllableList[i]));
      if(strlen(syllableList[i])>0){
         if(strstr(syllableList[i],"&eu&")!= NULL){
      		syllableList[i] =Utf8ReplaceAll("&eu&","!",syllableList[i]);
            euFlag=1;
      	}
         if(strstr(syllableList[i],"&euv&")!= NULL){
      		syllableList[i] =Utf8ReplaceAll("&euv&","!",syllableList[i]);
            euFlag=2;
      	}

         if(!CheckVowel(syllableList[i],0,1) && writeFormat !=3){
            if(DEBUG)printf("Stack : %s\n",syllableList[i] );
            strcat(syllableList[i],"&av");
         }
         if(strstr(syllableList[i],"!")!= NULL){
            if(euFlag==1)if(strstr(syllableList[i], "!") != NULL)syllableList[i] =Utf8ReplaceAll("!","&eu&",syllableList[i]);
            if(euFlag==2)if(strstr(syllableList[i], "!") != NULL)syllableList[i] =Utf8ReplaceAll("!","&euv&",syllableList[i]);
            syllableList[i] =Utf8ReplaceAll("!","eu",syllableList[i]);
         }
      }
      if(strstr(syllableList[i],"&&")!= NULL){
         syllableList[i] =Utf8ReplaceAll("&&","&",syllableList[i]);
      }
      syllableList[i] =GeminateCorrection(syllableList[i],1);
      //syllableList[i]=CleanseWord(syllableList[i]);

   }
   if(DEBUG){
   	for (i=0;i<len;i++)
   	{
   		printf("syllablifiedStack1 : %s\n",syllableList[i] );
   	}
   }
   if(DEBUG)printf("No of syllables : %d\n", len);
   syllableCount=len;

  	if(writeFormat == 3)writeFormat=0;
   return 1;

}

//make to write format
int WritetoFiles(){
   int i,j;
   if(DEBUG){
      for (i=0;i<syllableCount;i++)
      {
         //syllableList[i] =CleanseWord(syllableList[i]);
         printf("syllablifiedStackfinal : %s\n",syllableList[i] );
      }
   }
   char * pch;
   outputText = (char *)malloc(1000);//*strlen(syllableList[0]));
   // if(syldictFlag){
   //    syllableList[0] = Utf8ReplaceAll("&eu","!",syllableList[0]);
   //    if(!CheckVowel(syllableList[0],1,0)){
   //       strcat(syllableList[0],"&av&");
   //    }
   //    syllableList[0] =Utf8ReplaceAll("!","&eu",syllableList[0]);
   // }
   int validSyllable=0;
   for (i=0;i<syllableCount;i++)
   {
      if(strcmp(syllableList[i],""))validSyllable++;
   }

   if(DEBUG)printf("a correction%s\n", syllableList[0]);
   strcpy(outputText,"");
   if(writeFormat == 0)//phone
   {
      int syllablesPrint=0;
      for (i=0;i<syllableCount;i++)
      {
         strcat(outputText ,"(( ");
         pch = strtok(syllableList[i],"&");
         while (pch != NULL && pch != "")
         {
            if(DEBUG)printf("syl  output%s  %s\n", pch,outputText);
            j=1;
            //if(strcmp(pch,"av") == NULL || CheckSymbol(pch)){
            strcat(outputText ,"\"");
            strcat(outputText , pch);
            strcat(outputText ,"\" ");
            //}
            pch = strtok (NULL, "&");
         }
         if(j){
            if(syllTagFlag)
            {
               if(syllablesPrint==0){strcat(outputText ,"_beg");}
               else if(syllablesPrint==validSyllable-1){strcat(outputText ,"_end");}
               else {strcat(outputText ,"_mid");}
               syllablesPrint++;
            }
            strcat(outputText ,") 0) ");
         }else{
            outputText[strlen(outputText)-3]='\0';
         }
         j=0;
      }
      if(strstr(outputText, "v") != NULL)outputText =Utf8ReplaceAll("v","",outputText);
      if(strstr(outputText, " \"eu\"") != NULL)outputText =Utf8ReplaceAll(" \"eu\"","",outputText);
      if(strstr(outputText, "!") != NULL)outputText =Utf8ReplaceAll("!","",outputText);
   }
   else if(writeFormat == 1)//syllable
   {
      //printf("syl count%d\n",validSyllable );
      int syllablesPrint=0;
      for (i=0;i<syllableCount;i++)
      {
         if(strstr(syllableList[i], "euv") != NULL)syllableList[i] =Utf8ReplaceAll("euv","eu",syllableList[i]);
         syllableList[i] = SyllableReverseCorrection(syllableList[i],LangSpecificCorrectionFlag);
         if(DEBUG)printf("%s\n", syllableList[i]);
         strcat(outputText ,"(( \"");
         //currentSyllable = strdup(syllableList[i]);
         pch = strtok(syllableList[i],"&");
         while (pch != NULL && pch != "")
         {
            if(DEBUG)printf("syl %s\n",pch );
            j=1;
            if(CheckSymbol(pch))
            {
               if(strcmp(pch,"av"))
               {
                  strcat(outputText , GetUTF(pch));
               }else{
                  if(DEBUG)printf("av found");
                  strcat(outputText , GetUTF(pch));
               }
            }
            pch = strtok (NULL, "&");
         }
         if(j){
            if(syllTagFlag)
            {
               if(syllablesPrint==0){strcat(outputText ,"_beg");}
               else if(syllablesPrint==validSyllable-1){strcat(outputText ,"_end");}
               else {strcat(outputText ,"_mid");}
               syllablesPrint++;
            }
            strcat(outputText ,"\" ) 0) ");

         }else{
            outputText[strlen(outputText)-4]='\0';
         }

         j=0;
      }
   }else if(writeFormat == 2)//Aksharas  not used
   {
   	 //  int syllablesPrint=0;
     //  for (i=0;i<syllableCount;i++)
     //  {
     //     strcat(outputText ,"(( ");
     //     pch = strtok(syllableList[i],"&");
     //     while (pch != NULL && pch != "")
     //     {
     //        if(DEBUG)printf("syl  output%s  %s\n", pch,outputText);
     //        j=1;
     //        //if(strcmp(pch,"av") == NULL || CheckSymbol(pch)){
     //        strcat(outputText ,"\"");
     //        strcat(outputText , pch);
     //        strcat(outputText ,"\" ");
     //        //}
     //        pch = strtok (NULL, "&");
     //     }
     //     if(j){
     //        if(syllTagFlag)
     //        {
     //           if(syllablesPrint==0){strcat(outputText ,"_beg");}
     //           else if(syllablesPrint==validSyllable-1){strcat(outputText ,"_end");}
     //           else {strcat(outputText ,"_mid");}
     //           syllablesPrint++;
     //        }
     //        strcat(outputText ,") 0) ");
     //     }else{
     //        outputText[strlen(outputText)-3]='\0';
     //     }
     //     j=0;
     //  }
     //  if(strstr(outputText, "v") != NULL)outputText =Utf8ReplaceAll("v","",outputText);
     //  if(strstr(outputText, " \"eu\"") != NULL)outputText =Utf8ReplaceAll(" \"eu\"","",outputText);
     //  if(strstr(outputText, "!") != NULL)outputText =Utf8ReplaceAll("!","",outputText);

     //  printf("new : %s", outputText);

     // //  for(i=0;i<CONSONANTSSIZE;i++)//if(strcmp(CONSONANTS[i],input)==NULL)return 2;
   		// // {
   		// // 	h" "i   
   		// // 	if(strcmp())
   		// // }

   	  // for(i=0;i<SEMIVOWELSSIZE;i++)if(strcmp(SEMIVOWELS[i],input)==NULL)return 3;
      // for(i=0;i<VOWELSSIZE;i++)if(strcmp(VOWELS[i],input)==NULL)return 1;
      // {
      // 	if()
      // }


   }
   //final unwanted # correction. due to hidden chars
   if(strstr(outputText, "#") != NULL)outputText =Utf8ReplaceAll("#","",outputText);

   if(DEBUG)printf("Print text : %s\n",outputText);
   // if(syldictFlag && !doubleParseSyl){
   //   //for (i=0; outputText[i]; outputText[i]=='0' ? i++ : *outputText++);
   //   printf("count 0 : %d\n", CountChars(outputText,'0'));
   //   if(CountChars(outputText,'0')>1){
   //     doubleParseSyl=1;
   //
   //     main(1,"");
   //     return 1;
   //   }
   // }
   WriteFile(outputText);
   return 1;
}

//write to wordpronunciation file
void WriteFile(char * text){
   FILE *output;
   output = fopen(outputFile,"w");
   if(fliteHTSFlag){
      // text =Utf8ReplaceAll("0","#",text);
      // text =Utf8ReplaceAll("\" \"","*",text);
      // text =Utf8ReplaceAll("(","",text);
      // text =Utf8ReplaceAll(")","",text);
      // text =Utf8ReplaceAll(" ","",text);
      // text =Utf8ReplaceAll("\"","",text);
      text[strlen(text)-1]='\0';
      printf("phones : %s\n",text);
      fprintf(output,"%s",text);
      fclose(output);
      return;
   }
   fprintf(output,"%s","(set! wordstruct '( ");
   fprintf(output,"%s",text);
   fprintf(output,"%s","))\n");
   fclose(output);
   if(!writeFormat && pruiningFlag)WritePruneFile(text);
}

//output for pruning
void WritePruneFile(char * text){
   FILE *output;
   output = fopen(outputPruneFile,"w");
   text =Utf8ReplaceAll("(","",text);
   text =Utf8ReplaceAll(")","",text);
   text =Utf8ReplaceAll("0","",text);
   text =Utf8ReplaceAll("   "," ",text);
   text =Utf8ReplaceAll("  ","",text);
   text =Utf8ReplaceAll("\"\"","\" \"",text);
   fprintf(output,"%s",text);
   fclose(output);
}

//print the help of syntax
void printHelp () {
   printf ("UnifiedParser : v3.0\n");
   printf ("Usage : ./unified-parser word LangSpecificCorrection WriteFormat ForSylldict IsPruning DirectParse LangId timestamp\n");
   printf ("LangSpecificCorrection : \n\t0-No\n\t1-Yes \n");
   printf ("WriteFormat : \n\t0-Phone\n\t1-Syllable \n");
   printf ("ForSylldict : writes output to wordpronunciationsyldict\n\t0-No\n\t1-Yes \n");
   printf ("IsPruning : writes output for pruning format\n\t0-No\n\t1-Yes \n");
   printf ("DirectParse : No UTF-8 to CLS conversion\n\t0-No\n\t1-Yes \n");
   printf ("LangId : lang id for direct parsing\t0-8 \n");
   printf ("timestamp : append this to wordpronunciation\tstring \n");

   printf ("Example: ./unified-parser 1 0 0 0 - Monophone parser\n");
   printf ("Example: ./unified-parser 1 1 0 0 - Syllable parser\n");
   printf ("Example: ./unified-parser 1 2 0 0 - Aksharas parser\n");
   printf ("Example: ./unified-parser 1 3 0 0 - Direct parser for USS fallback\n");
   printf ("Example: ./unified-parser 1 4 0 0 - Syllable parser with beg mid end\n");
   
   //printf ("(test)outPutFormat :\n\t0-Festival\n\t1-Flite \n");
   return;
}

// //Lex replacer -- in dev stage.
// char * LexReplacer(char * phone){
//    //printf("1 %s\n", phone);
//    //&iv&#&aav&  DoubleModifierCorrection
//    strcat(phone,"&");
// 	register int i=0,j=0;
// 	char c1[10],c2[10];
//    //k|kh|lx|rx|g|gh|ng|c|ch|j|jh|nj|tx|txh|dx|dxh|nx|t|th|d|dh|n|p
//    //|ph|b|bh|m|y|r|r|l|w|sh|sx|zh|y|s|h|f|dxq|z|kq|khq|gq|dxhq
// 	char * consonentList[44]={"k","kh","lx","rx","g","gh","ng","c","ch","j","jh","nj","tx","txh",
//                            "dx","dxh","nx","t","th","d","dh","n","p","ph","b","bh","m","y","r",
//                         "l","w","sh","sx","zh","y","s","h","f","dxq","z","kq","khq","gq","dxhq" };
//    char * vowelList[43]={"av","nwv","nnv","rwv","lwv","lnv","aav","iiv","uuv","rqv","eev","eiv","ouv",
//                                                  "axv","oov","aiv","nnx","nxx","rrx","llx","lxx",
//                                                  "aa","iv","ov","mq","aa","ii","uu","rq","eu",
//                                                  "ee","ei","ou","oo","ax","ai","ev","uv",
//                                                  "a","e","i","o","u"};
//    for(j=0;j<44;j++){
//       strcpy(c1,"&");
//       strcat(c1,consonentList[j]);
//       strcat(c1,"&");
//       strcpy(c2,c1);
//       strcat(c2,"av&#&");
//       //if(DEBUG)            printf("%s %s\n",c1,c2 );
//    	if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
//    	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
//    }

//    for(j=0;j<43;j++){
//       strcpy(c1,"&av&#&");
//       strcat(c1,vowelList[j]);
//       strcat(c1,"&");
//       strcpy(c2,"&");
//       strcat(c2,vowelList[j]);
//       strcat(c2,"&#&");
//       //if(DEBUG)
//       printf("%s %s\n",c1,c2 );
//    	if(strstr(phone, c1) != NULL)phone =Utf8ReplaceAll(c1,"@",phone);
//    	if(strstr(phone, "@") != NULL)phone =Utf8ReplaceAll("@",c2,phone);
//    }
//    printf("1 :%s\n", phone);
//    //if(strstr(phone, "&av&#&av&") != NULL)phone =Utf8ReplaceAll("&av&#&av&","&av&#&",phone);
//    if(strstr(phone, "&&") != NULL)phone =Utf8ReplaceAll("&&","&",phone);
//    strcat(phone,"$");
//    if(strlen(phone)>9){
//       if(strstr(phone, "&av&#&$") != NULL)phone =Utf8ReplaceAll("&av&#&$","&",phone);
//       if(strstr(phone, "$") != NULL)phone =Utf8ReplaceAll("$","",phone);
//    }

//    printf("2 :%s\n", phone);
//    return phone;
// }

//The MAIN Function
void main(int argc,char** argv){

   //    int oc;
   //    while((oc = getopt (argc,argv,":DlspdP")) != -1) {
   //     switch (oc) {
   //         case 'D' :
   //          printf("Dict enabled\n" );
   //           //isDump = 1;
   //           break;
   //         case 'l' :
   //           //printHelp ();
   //           exit(0);
   //           break;
   //     }
   // }
   //printf("argv : %s\n",argv );
   if (argc<= 5) {
      printHelp ();
      exit(1);
   }
   if(strcmp(argv[2],"1")) LangSpecificCorrectionFlag = 0;
   //printf(" lang %d\n", LangSpecificCorrectionFlag);
   //printf("argg3 %s\n", argv[3] );
   if(!strcmp(argv[3],"0")) writeFormat = 0;
   if(!strcmp(argv[3],"1")) writeFormat = 1;
   if(!strcmp(argv[3],"2")) writeFormat = 2;
   if(!strcmp(argv[3],"3")) writeFormat = 3;
   if(!strcmp(argv[3],"4")) { writeFormat = 1;syllTagFlag=1;}
   //printf("argg3 %d\n", writeFormat );
   if(!strcmp(argv[4],"1")) {
      //syldictFlag = 0;
      //writeFormat = 0;
      outputFile=malloc(100);
      strcpy(outputFile,"wordpronunciationsyldict");

   }
   if(!strcmp(argv[5],"1")) {
      pruiningFlag = 1;
      //syldictFlag = 0;
      outputFile="wordpronunciation";
      writeFormat = 3;
   }
  if( argc > 6 && !strcmp(argv[6],"1")) {
      directParseFlag = 1;
      langId=atoi(argv[7]);
   }
  if(argc > 8){
   		outputFile=malloc(100);
   		strcpy(outputFile,"wordpronunciation");
   		strcat(outputFile,argv[8]);
   }else{
   		outputFile=malloc(100);
		strcpy(outputFile,"wordpronunciation");
       if(!strcmp(argv[4],"1")) {
      //syldictFlag = 0;
      //writeFormat = 0;
      outputFile=malloc(100);
      strcpy(outputFile,"wordpronunciationsyldict");
   }
   }
 //   	FILE *f;
 //   	char line1[128];
 //    f = fopen("inputunified", "r");
 //    if (!f) {
 //       printf("error");
 //    } else {
 //        fscanf(f, "%127[^\n]", line1);
 //        printf("1:%s\n", line1);
 //        fclose(f);
 //    }
 //    //char* word=strdup(line1);

	// char* word="नहिस";

      //fliteHTSFlag=1;
   char * word;
   word = (char *)calloc(1048576, sizeof(char));
   strcpy(word,argv[1]);
   if(DEBUG){
   	printf("Word : %s\n", word);
   }

   //if for syl dict
   //remove unwanted letters
   
   if( directParseFlag != 1 )word = RemoveUnwanted(word);
   if(DEBUG){printf("Cleared Word : %s\n", word);}

   if(!SetlanguageFeat(word)){return 0; }
   if( directParseFlag == 1 )langId=atoi(argv[7]);
   
   if(CheckDictionary(word)){return 0; }//!syldictFlag &&
   if(DEBUG){printf("langId : %d\n", langId);}
   word = ConvertToSymbols(word);
   //printf("%s\n", word );
	if( directParseFlag == 1 ){
		strcpy(syllabifiedWord, argv[1]);
	 	printf("%s\n", word );
	}
	//exit(0);
   // if(syldictFlag){
   //   syllableCount = 1;
   //   syllableList[0] = word;
   //   WritetoFiles();
   //   if(DEBUG)printf("syldict word : %s\n",word );
   //   return 0;
   // }
	//free(word);

   if(DEBUG){
      printf("Symbols code : %s\n", unicodeWord);
      printf("Symbols syllables : %s\n", syllabifiedWord);
   }
   syllabifiedWordOut = (char *)calloc(1048576, sizeof(char));
   yy_scan_string(syllabifiedWord);
   yyparse();

   //syllabifiedWordOut = LexReplacer(word);

   // if(!isSouth)//direct south correction - not used now
   // {
   //    //if(strstr(syllabifiedWordOut, "&av&") != NULL)syllabifiedWordOut =Utf8ReplaceAll("&av&","&eu&",syllabifiedWordOut);
   // }
   if(DEBUG){printf("Syllabified Word : %s\n", syllabifiedWordOut); }
   if(strstr(syllabifiedWordOut, "&#&") != NULL)syllabifiedWordOut = Utf8ReplaceAll("&#&","&",syllabifiedWordOut);
   strcat(syllabifiedWordOut,"&\0");
   if(DEBUG)printf("Syllabified Word out : %s\n", syllabifiedWordOut);
   syllabifiedWordOut = LangSpecificCorrection(syllabifiedWordOut,LangSpecificCorrectionFlag);
   if(DEBUG)printf("Syllabified Word langCorr : %s\n", syllabifiedWordOut);
   //syllabifiedWordOut = GeminateCorrection(syllabifiedWordOut,0);
   if(DEBUG)printf("Syllabified Word gemCorr : %s\n", syllabifiedWordOut);
   syllabifiedWordOut = CleanseWord(syllabifiedWordOut);
   if(DEBUG)printf("Syllabified Word memCorr : %s\n", syllabifiedWordOut);
   if(!isSouth)//halant split word
   {
      int i=0,count=0;
      for(i=0;i<strlen(syllabifiedWordOut);i++)
      {
         if(syllabifiedWordOut[i] =='&')count++;//just to get count of phones
      }
      int splitPosition = 2;// by default split after 2 phones
      if(GetPhoneType(syllabifiedWordOut,1) == 1) //check first is vowel
      {
         if(count>2){//more than 1 phones are there
            int type = GetPhoneType(syllabifiedWordOut,2); // get type of 2nd phone
            if(type==2)splitPosition=1;
            else if(type==3)splitPosition=3;
         }else{
            splitPosition=1;
         }
      }
      count=0;
      for(i=0;i<strlen(syllabifiedWordOut);i++)
      {
         if(syllabifiedWordOut[i] =='&')count++;
         if(count>splitPosition)
         {
           count=i;
           break;
         }
      }
      char * start,* end;
      // for first char never remove 'av'
      start = strdup(syllabifiedWordOut);
      end = strdup(syllabifiedWordOut);
      end += count;
      start[count]='\0';
      if(DEBUG)printf("posi  %d %s %s\n",count,start,end);
      end = SchwaSpecificCorrection(end);
      if(DEBUG)printf("prefinal : %s\n", syllabifiedWordOut);
      strcpy(syllabifiedWordOut,start);
      strcat(syllabifiedWordOut,end);
      if(DEBUG)printf("prefinal1 : %s\n", syllabifiedWordOut);
      syllabifiedWordOut = CleanseWord(syllabifiedWordOut);
      if(DEBUG)printf("final : %s\n", syllabifiedWordOut);
      syllabifiedWordOut = SchwaDoubleConsonent(syllabifiedWordOut);//for eu&C&C&V
      if(DEBUG)printf("final0 : %s\n", syllabifiedWordOut);
   }

   syllabifiedWordOut = GeminateCorrection(syllabifiedWordOut,0);//chk here
   if(DEBUG)printf("Syllabified Word gemCorr : %s\n", syllabifiedWordOut);

   syllabifiedWordOut = MiddleVowel(syllabifiedWordOut); // middle vowel correction
   if(DEBUG)printf("Syllabified Word  middle vowel : %s\n", syllabifiedWordOut);

   syllabifiedWordOut = Syllabilfy(syllabifiedWordOut);
   //syllabifiedWordOut = DoubleModifierCorrection(syllabifiedWordOut);
   if(DEBUG)printf("Syllabified Word final : %s\n", syllabifiedWordOut);





   SplitSyllables(syllabifiedWordOut);
   if(DEBUG)printf("Splitted to Syllables\n");
   WritetoFiles();
   if(DEBUG)printf("Files created %s\n",outputText);
   //CombinationCorrection();
}


#line 1815 "unified.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "unified.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_space = 3,                      /* space  */
  YYSYMBOL_fullvowel = 4,                  /* fullvowel  */
  YYSYMBOL_kaki = 5,                       /* kaki  */
  YYSYMBOL_conjsyll2 = 6,                  /* conjsyll2  */
  YYSYMBOL_conjsyll1 = 7,                  /* conjsyll1  */
  YYSYMBOL_nukchan = 8,                    /* nukchan  */
  YYSYMBOL_yarule = 9,                     /* yarule  */
  YYSYMBOL_consonant = 10,                 /* consonant  */
  YYSYMBOL_vowel = 11,                     /* vowel  */
  YYSYMBOL_halant = 12,                    /* halant  */
  YYSYMBOL_matra = 13,                     /* matra  */
  YYSYMBOL_YYACCEPT = 14,                  /* $accept  */
  YYSYMBOL_sentence = 15,                  /* sentence  */
  YYSYMBOL_words = 16,                     /* words  */
  YYSYMBOL_syltoken = 17                   /* syltoken  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   11

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  14
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  4
/* YYNRULES -- Number of rules.  */
#define YYNRULES  14
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  16

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   268


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  1815,  1815,  1827,  1838,  1841,  1842,  1843,  1844,  1845,
    1846,  1847,  1848,  1849,  1850
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "space", "fullvowel",
  "kaki", "conjsyll2", "conjsyll1", "nukchan", "yarule", "consonant",
  "vowel", "halant", "matra", "$accept", "sentence", "words", "syltoken", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-5)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -4,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,
      -5,    10,    -4,    -5,    -5,    -5
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,     0,     2,     3,     1,     4
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
      -5,    -5,    -5,    -1
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    11,    12,    13
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      14,    15
};

static const yytype_int8 yycheck[] =
{
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
       0,    12
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    15,    16,    17,     0,    17
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    14,    15,    16,    16,    17,    17,    17,    17,    17,
      17,    17,    17,    17,    17
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* sentence: words  */
#line 1815 "unified.y"
                  {
            if(parseLevel ==0){
            	syllabifiedWordOut = (yyvsp[0].strval);
            	if(strstr(syllabifiedWordOut, "&&") != NULL)syllabifiedWordOut =Utf8ReplaceAll("&&","&",syllabifiedWordOut);
            	parseLevel++;
            }
            else{
            	  phonifiedWord = (yyvsp[0].strval);
              }
            }
#line 2824 "unified.tab.c"
    break;

  case 3: /* words: syltoken  */
#line 1827 "unified.y"
                 {  		/*	int length = strlen($1);
   			int j = 0;
   			if ( $1[length -1] == 'h'){
   				char str[length];
   				for(j=0;j<length-1;j++)
   					str[j]=$1[j];
   				str[j]='\0';
   				strcpy($1,str);
   			}*/
   			if(DEBUG)printf("\nSyll:\t%s",(yyvsp[0].strval));
   			(yyval.strval) = (yyvsp[0].strval); }
#line 2840 "unified.tab.c"
    break;

  case 4: /* words: words syltoken  */
#line 1838 "unified.y"
                          { char* str = malloc(2*(strlen((yyvsp[-1].strval))+strlen((yyvsp[0].strval)))) ; sprintf( str,"%s%s",(yyvsp[-1].strval),(yyvsp[0].strval));(yyval.strval)=str;}
#line 2846 "unified.tab.c"
    break;

  case 5: /* syltoken: fullvowel  */
#line 1841 "unified.y"
                     { (yyval.strval) = (yyvsp[0].strval); }
#line 2852 "unified.tab.c"
    break;

  case 6: /* syltoken: kaki  */
#line 1842 "unified.y"
                { if(DEBUG)printf("kaki : %s\n",(yyvsp[0].strval) );(yyval.strval) = (yyvsp[0].strval); }
#line 2858 "unified.tab.c"
    break;

  case 7: /* syltoken: conjsyll2  */
#line 1843 "unified.y"
                     { (yyval.strval) = (yyvsp[0].strval); }
#line 2864 "unified.tab.c"
    break;

  case 8: /* syltoken: conjsyll1  */
#line 1844 "unified.y"
                     { (yyval.strval) = (yyvsp[0].strval); }
#line 2870 "unified.tab.c"
    break;

  case 9: /* syltoken: nukchan  */
#line 1845 "unified.y"
                   { (yyval.strval) = (yyvsp[0].strval); }
#line 2876 "unified.tab.c"
    break;

  case 10: /* syltoken: yarule  */
#line 1846 "unified.y"
                  { (yyval.strval) = (yyvsp[0].strval); }
#line 2882 "unified.tab.c"
    break;

  case 11: /* syltoken: consonant  */
#line 1847 "unified.y"
                     {  (yyval.strval)=(yyvsp[0].strval);/*char* str = malloc(strlen($1)+strlen("्")+4); sprintf( str,"%s&#",$1 );printf("\nconsonant:\t");$$=str*/; }
#line 2888 "unified.tab.c"
    break;

  case 12: /* syltoken: vowel  */
#line 1848 "unified.y"
                 { (yyval.strval) = (yyvsp[0].strval); }
#line 2894 "unified.tab.c"
    break;

  case 13: /* syltoken: halant  */
#line 1849 "unified.y"
                  { (yyval.strval) = (yyvsp[0].strval); }
#line 2900 "unified.tab.c"
    break;

  case 14: /* syltoken: matra  */
#line 1850 "unified.y"
                 { (yyval.strval) = (yyvsp[0].strval); }
#line 2906 "unified.tab.c"
    break;


#line 2910 "unified.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

