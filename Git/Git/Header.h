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
func_res_t initBuf(int version); //создаЄт буфер с потенциальным родителем version, если буфер был - стираетс€
void deleteBuf();
func_res_t createVerFile(); //формирует из буфера новый файл

/*Editing.c*/
func_res_t add(int i, char* data);
bool_t indexIsCorrect(int i); //проверить индекс на корректность

/*GeneralInfo.c*/
func_res_t initGeneralInfo(char fname[FNAME_LEN]); //парсит главную информацию, дерево, создаЄт буфер
void deleteGeneralInfo();

/*Main.c*/
void handleMainCycle(); //запуск основного цикла
void printMainMenu();
void cleanup();

/*File.c*/
char* getNameOfVerFile(int version);
void strcatInt(char fileName[FNAME_LEN], int version);
bool_t exists(const char *fname);
const char* getFileExt(const char* filename);
long int getFileSize(FILE *f);

/*OperationList.c*/
void deleteOperationList(operation_t** root);
void pushIntoOpList(operation_t** root, operation_t* operation);
operation_t* getLastOperation(operation_t** root);
func_res_t printOperations(FILE* file);

/*Path.c*/
func_res_t getPath(path_t** path); //получить путь от корн€ до буфера
func_res_t pushIntoPath(path_t** root, int verNum);

/*Text.c*/
int getTextLen(); //получить длину текста на текущей итерации
int getLenDiff(operation_t* operation); //получить изменение длины текста после применени€ операций из массива operation
func_res_t print(); //выводит текст на текущей итерации
func_res_t getCurText(char* text);
func_res_t applyChanges(char* text, path_t* el);
func_res_t applyVerChanges(char* text, operation_t* opList); //пройти по пути и собрать операции, примен€€ их к text
void printText(char* text);

/*VersionsTree.c*/
func_res_t initVerTree();
func_res_t initTextAsOpearationInRootVer();
func_res_t getSourceText(char** text);
void deleteVerTree();
func_res_t buildVerTree(); //получить generalInfo->root, lastCreatedVersion
version_t* getVerPtr(version_t* p, int verNum); //обойти дерево, найти вершину и вернуть либо указатель на неЄ, либо NULL
func_res_t push();