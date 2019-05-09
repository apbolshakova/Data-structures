#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Constants.h"
#include "Types.h"

extern general_t* generalInfo;
extern version_t* buf;
extern version_t* verTree;

void strcatInt(char fileName[FNAME_LEN], int version);
char* getNameOfVerFile(int version);

bool_t exists(const char *fname);
const char* getFileExt(const char* filename);
void cleanup(); //�������� ���������� ����������

void deleteOperationList(operation_t** root);

func_result_t initVerTree(); //������� ����� ������� ������
void deleteVerTree();
func_result_t buildVerTree(); //��������� ������ � ������� ��� ���� ������������ �������

func_result_t initBuf(int version); //������� �����
func_result_t initGeneralInfo(char fname[FNAME_LEN]);

//func_result_t loadVersion(version_t* verInfo, FILE* verFile); //�������� � verInfo ��� ��������� ������
//func_result_t handleFile(char fname[FNAME_LEN], int version, version_t* verInfo);