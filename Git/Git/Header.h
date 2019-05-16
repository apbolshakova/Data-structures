#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Constants.h"
#include "Types.h"

extern general_t* generalInfo;
extern version_t* buf;
extern version_t* verTree;

/*Buf.c*/
func_res_t initBuf(int version); //������ ����� � ������������� ��������� version, ���� ����� ��� - ���������
void deleteBuf();
func_res_t createVerFile(); //��������� �� ������ ����� ����

/*Editing.c*/
func_res_t handleAdd();
func_res_t add(int i, char* data);
func_res_t handleRemoving();
func_res_t remove(int i, int j);
func_res_t handleEditing();
char* getData(int* len);
bool_t indexIsCorrect(int i); //��������� ������ �� ������������

/*GeneralInfo.c*/
func_res_t initGeneralInfo(char* fname); //������ ������� ����������, ������, ������ �����
void deleteGeneralInfo();

/*Main.c*/
const char* getStrsFormat(int* len, int n);
void handleMainCycle(); //������ ��������� �����
void printMainMenu();
void cleanup();

/*File.c*/
char* getNameOfVerFile(int version);
void strcatInt(char* fileName, int version);
bool_t exists(const char *fname);
const char* getFileExt(const char* filename);
long int getFileSize(FILE *f);

/*OperationList.c*/
void deleteOperationList(operation_t** root);
void pushIntoOpList(operation_t** root, operation_t* operation);
operation_t* getLastOperation(operation_t** root);
func_res_t printOperations(FILE* file);

/*Path.c*/
func_res_t getPath(path_t** path); //�������� ���� �� ����� �� ������
func_res_t pushIntoPath(path_t** root, version_t* ver);

/*Text.c*/
int getTextLen(); //�������� ����� ������ �� ������� ��������
int getMaxTextLen(); //�������� ������ ��� ������
int getLenDiff(operation_t* operation); //�������� ��������� ����� ������ ����� ���������� �������� �� ������� operation
int getMaxLenDiff(operation_t* list);
func_res_t print(); //������� ����� �� ������� ��������
func_res_t getCurText(char* text, int textLen); 
func_res_t applyChanges(char* text, int textLen, path_t* el);
func_res_t applyVerChanges(char* text, int textLen, operation_t* opEl); //������ �� ���� � ������� ��������, �������� �� � text
void printText(char* text);

/*VersionsTree.c*/
func_res_t initVerTree();
func_res_t initTextAsOpearationInRootVer();
func_res_t getSourceText(char** text);
void deleteVerTree();
func_res_t buildVerTree(); //�������� generalInfo->root, lastCreatedVersion
version_t* getVerPtr(version_t* p, int verNum); //������ ������, ����� ������� � ������� ���� ��������� �� ��, ���� NULL
func_res_t push();