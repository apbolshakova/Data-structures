#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Constants.h"
#include "Types.h"

extern general_t* generalInfo;
extern version_t* buf;
extern version_t* verTree;

const char* getFileExt(const char* filename);

void deleteOperationList(operation_t* root);

func_result_t initVerTree(); //создать новую системы версий
void deleteVerTree();
func_result_t buildVerTree(); //построить дерево с данными обо всех существующих версиях

func_result_t initBuf(int version); //создать буфер
func_result_t initGeneralInfo(char fname[FNAME_LEN]);

//func_result_t loadVersion(version_t* verInfo, FILE* verFile); //положить в verInfo все доступные данные
//func_result_t handleFile(char fname[FNAME_LEN], int version, version_t* verInfo);