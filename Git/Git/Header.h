#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Constants.h"
#include "Types.h"

extern general_t* generalInfo;
extern version_t* buf;
extern version_t* verTree;

/*Editing.c*/
func_result_t add(int i, char* data);
bool_t indexIsCorrect(int i); //проверить индекс на корректность
int getTextLen(); //получить длину текста на текущей итерации

/*Init.c*/
func_result_t initGeneralInfo(char fname[FNAME_LEN]);
func_result_t initBuf(int version);

/*Misc.c*/
char* getNameOfVerFile(int version);
void strcatInt(char fileName[FNAME_LEN], int version);
bool_t exists(const char *fname);
void cleanup();
const char* getFileExt(const char* filename);
long int getFileSize(FILE *f);

/*OperationList.c*/
void deleteOperationList(operation_t** root);
func_result_t pushIntoOpList(operation_t* opBuf);

/*VersionsTree.c*/
func_result_t initVerTree();
func_result_t initTextAsOpearationInRootVer();
func_result_t getSourceText(char** text);
void deleteVerTree();
func_result_t buildVerTree(); //получить generalInfo->root, lastCreatedVersion и curText