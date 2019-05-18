#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <fileapi.h>

#include "Constants.h"
#include "Types.h"

extern general_t* generalInfo;
extern version_t* buf;
extern version_t* verTree;

/*Buf.c*/
status_t initBuf(int version); //создаЄт буфер с потенциальным родителем version, если буфер был - стираетс€
void deleteBuf();
status_t createVerFile(); //формирует из буфера новый файл

/*Editing.c*/
status_t handleAdd();
status_t add(int i, char* data);
status_t handleRemoving();
status_t remove(int i, int j);
status_t handleEditing();
char* getData(int* len);
bool_t indexIsCorrect(int i); //проверить индекс на корректность

/*GeneralInfo.c*/
status_t initGeneralInfo(char* fname, int verNum); //парсит главную информацию, дерево, создаЄт буфер
void deleteGeneralInfo();

/*Main.c*/
void handleMainCycle(); //запуск основного цикла
void printMainMenu();
int getID(char action[ACTION_LEN]);
status_t handleAction(int ID);
void cleanup();

/*File.c*/
char* getNameOfVerFile(int version);
char* getDirName();
void strcatInt(char* fileName, int version);
bool_t exists(const char *fname);
const char* getFileExt(const char* filename);
long int getFileSize(FILE *f);
int getVerNum(char filePath[FNAME_LEN]);

/*OperationList.c*/
void deleteOperationList(operation_t** root);
void pushIntoOpList(operation_t** root, operation_t* operation);
operation_t* getLastOperation(operation_t** root);
status_t printOperations(FILE* file);

/*Path.c*/
status_t getPath(path_t** path); //получить путь от корн€ до буфера
status_t pushIntoPath(path_t** root, version_t* ver);

/*Text.c*/
int getTextLen(); //получить длину текста на текущей итерации
int getMaxTextLen(); //получить размер дл€ буфера
int getLenDiff(operation_t* operation); //получить изменение длины текста после применени€ операций из массива operation
int getMaxLenDiff(operation_t* list);
status_t print(); //выводит текст на текущей итерации
status_t getCurText(char* text, int textLen); 
status_t applyChanges(char* text, int textLen, path_t* el);
status_t applyVerChanges(char* text, int textLen, operation_t* opEl); //пройти по пути и собрать операции, примен€€ их к text
void printText(char* text);

/*VersionsTree.c*/
status_t initVerTree();
status_t initTextAsOpearationInRootVer();
status_t getSourceText(char** text);
void deleteVerTree(version_t* p);
status_t buildVerTree(int verNum); //получить generalInfo->root, lastCreatedVersion дл€ нового дерева
status_t loadVerTree(); //получить generalInfo->root, lastCreatedVersion, пройти по всех файлам версий и построить дерево
status_t handleVerFile(char filePath[FNAME_LEN]);
version_t* getVerPtr(version_t* p, int verNum); //обойти дерево, найти вершину и вернуть либо указатель на неЄ, либо NULL
status_t push();