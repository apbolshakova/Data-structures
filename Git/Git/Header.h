#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Constants.h"
#include "Types.h"

extern general_t* generalInfo;
extern version_t* buf;
extern version_t* verTree;

/*Init.c*/
func_result_t initGeneralInfo(char fname[FNAME_LEN]);
func_result_t initBuf(int version);

/*Misc.c*/
char* getNameOfVerFile(int version);
void strcatInt(char fileName[FNAME_LEN], int version);
bool_t exists(const char *fname);
void cleanup();
const char* getFileExt(const char* filename);

/*OperationList.c*/
void deleteOperationList(operation_t** root);

/*VersionsTree.c*/
func_result_t initVerTree();
func_result_t initTextAsOpearationInRootVer();
func_result_t getSourceText(char** text);
void deleteVerTree();
func_result_t buildVerTree();