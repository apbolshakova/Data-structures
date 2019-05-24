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
status_t initBuf(int version); //������ ����� � ������������� ��������� version, ���� ����� ��� - ���������
void deleteBuf();
status_t createVerFile(); //��������� �� ������ ����� ����
status_t rewriteVerFile(version_t* ver); //������������� ���� ������

/*Editing.c*/
status_t handleAdd();
status_t add(int i, char* data);
status_t handleRemoving();
status_t remove(int i, int j);
status_t handleEditing();
char* getDataFromInput(int* len);
bool_t indexIsCorrect(int i); //��������� ������ �� ������������

/*GeneralInfo.c*/
status_t initGeneralInfo(char* fname, int verNum); //������ ������� ����������, ������, ������ �����
void deleteGeneralInfo();

/*Main.c*/
void handleMainCycle(); //������ ��������� �����
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
status_t getDataFromFile(char** storage, FILE* file);

/*OperationList.c*/
void deleteOperationList(operation_t** root);
void pushIntoOpList(operation_t** root, operation_t* operation);
operation_t* getLastOperation(operation_t** root);
status_t printOperations(FILE* file, operation_t* opListFromVer);
status_t getOperationList(operation_t** root, FILE* file);
status_t appendOpList(operation_t** opListRoot, operation_t* appendOpList);
/*Path.c*/
status_t getPath(path_t** path); //�������� ���� �� ����� �� ������
status_t pushIntoPath(path_t** root, version_t* ver);

/*Text.c*/
int getTextLen(); //�������� ����� ������ �� ������� ��������
int getMaxTextLen(); //�������� ������ ��� ������
int getLenDiff(operation_t* operation); //�������� ��������� ����� ������ ����� ���������� �������� �� ������� operation
int getMaxLenDiff(operation_t* list);
status_t print(); //������� ����� �� ������� ��������
status_t getCurText(char* text, int textLen); 
status_t applyChanges(char* text, int textLen, path_t* el);
status_t applyVerChanges(char* text, int textLen, operation_t* opEl); //������ �� ���� � ������� ��������, �������� �� � text
void printText(char* text);

/*VersionsTree.c*/
status_t initVerTree();
status_t initTextAsOpearationInRootVer();
status_t getSourceText(char** text);
void deleteVerTree(version_t* p);
status_t buildVerTree(int verNum); //�������� generalInfo->root, lastCreatedVersion ��� ������ ������
status_t loadVerTree(); //�������� generalInfo->root, lastCreatedVersion, ������ �� ���� ������ ������ � ��������� ������
status_t handleVerFile(char filePath[FNAME_LEN]);
status_t insertIntoTree(version_t* ver); //Add version into tree
version_t* getVerPtr(version_t* p, int verNum); //������ ������, ����� ������� � ������� ���� ��������� �� ��, ���� NULL
status_t push();
status_t handleVerDeleting();
status_t deleteVer(version_t* verToDelete);
void moveBackChildren(version_t* parentPtr, int verPos); //�������� ����� �������� �� �������� verPos + 1..childrenNum - 1 �� ���� �����
status_t copyVerChildren(version_t* prevParent); //copy version's children to it's parent with version's operations saving TODO
status_t relocateChild(version_t* prevParent, int i);
status_t addChild(version_t* newChild, version_t* parent); //add version to child array without any editing
void cleanupVersion(version_t** ver);