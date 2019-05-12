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
func_res_t initBuf(int version); //создаёт буфер с потенциальным родителем version, если буфер был - стирается
void deleteBuf();
func_res_t createVerFile(); //формирует из буфера новый файл

/*Editing.c*/
func_res_t add(int i, char* data);
bool_t indexIsCorrect(int i); //проверить индекс на корректность

/*GeneralInfo.c*/
func_res_t initGeneralInfo(char fname[FNAME_LEN]); //парсит главную информацию, дерево, создаёт буфер
void deleteGeneralInfo();

/*Main.c*/
void handleMainCycle(); //запуск основного цикла
void printMainMenu();

/*Misc.c*/
char* getNameOfVerFile(int version);
void strcatInt(char fileName[FNAME_LEN], int version);
bool_t exists(const char *fname);
void cleanup();
const char* getFileExt(const char* filename);
long int getFileSize(FILE *f);
int getTextLen(); //получить длину текста на текущей итерации
int getLenDiff(operation_t* operation); //получить изменение длины текста после применения операций из массива operation
func_res_t print(); //выводит текст на текущей итерации

/*OperationList.c*/
void deleteOperationList(operation_t** root);
void pushIntoOpList(operation_t** root, operation_t* operation);
operation_t* getLastOperation(operation_t** root);
func_res_t printOperations(FILE* file);

/*VersionsTree.c*/
func_res_t initVerTree();
func_res_t initTextAsOpearationInRootVer();
func_res_t getSourceText(char** text);
void deleteVerTree();
func_res_t buildVerTree(); //получить generalInfo->root, lastCreatedVersion
version_t* getVerPtr(version_t* p, int verNum); //обойти дерево, найти вершину и вернуть либо указатель на неё, либо NULL
func_res_t push();