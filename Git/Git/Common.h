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
status_t initBuf(int version); //create buffer with "version" attached as buffer's parent (existing buffer will be destroyed)
void deleteBuf();
status_t createVerFile(); //create new version file based on buffer
status_t rewriteVerFile(version_t* ver); //recreate version file

/*Editing.c*/
status_t handleAdd();
status_t add(int i, char* data, version_t* ver);
status_t handleRemoving();
status_t remove(int i, int j);
status_t handleEditing();
char* getDataFromInput(int* len);
bool_t indexIsCorrect(int i); //check if index exists in text

/*GeneralInfo.c*/
status_t initGeneralInfo(char* fname, int verNum); //parse general info and tree, creates buffer
void deleteGeneralInfo();

/*Main.c*/
void handleMainCycle();
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
void shiftIntoOpList(operation_t** root, operation_t* operation);
operation_t* getLastOperation(operation_t** root);
status_t printOperations(FILE* file, operation_t* opListFromVer);
status_t getOperationList(operation_t** root, FILE* file);
status_t appendOpList(operation_t** opListRoot, operation_t* appendOpList);
status_t reverseVerOperations(version_t* newRoot); //handle new root operations, reverse operations in other involved versions and reverse order of versions
status_t reverseOpList(operation_t** opList);
status_t getReversedOperation(operation_t** result, operation_t* src);
void swapOpLists(operation_t** opList1, operation_t** opList2);

/*Path.c*/
status_t getPath(verList_t** path, version_t* ver); //get path from root to buffer
status_t shiftIntoPath(verList_t** root, version_t* ver);
status_t insertLostVersIntoTree(verList_t** root);
void deletePath(verList_t** root);

/*Text.c*/
int getTextLen(version_t* ver); //get text length at current iteration
int getMaxTextLen(version_t* ver); //get size for text buffer (enough to safe applying all of the changes)
int getLenDiff(operation_t* operation); //get how much text length was changed after applying operations from "operation"
int getMaxLenDiff(operation_t* list);
status_t print(); //print text as it looks like at current iteration
status_t getCurText(char* text, int textLen, version_t* ver, operation_t* applyChangesUpTo);
status_t applyChanges(char* text, int textLen, verList_t* el, operation_t* applyChangesUpTo);
status_t applyVerChanges(char* text, int textLen, operation_t* opEl); //apply all of the changes to text
void printText(char* text);

/*VersionsTree.c*/
status_t initVerTree();
status_t initTextAsOpearationInRootVer();
status_t getSourceText(char** text); //open file generalInfo->fileName and get text from it
void deleteVerTree(version_t* p);
status_t buildVerTree(int verNum); //get generalInfo->root and lastCreatedVersion for new tree
status_t loadVerTree(); //parse information from version files and build tree
status_t handleVerFile(char filePath[FNAME_LEN], verList_t** lostVersList);
status_t insertIntoTree(version_t* ver, int parentVerNum, verList_t** lostVersList); //add version into tree
version_t* getVerPtr(version_t* p, int verNum); //find required node in tree and return pointer to it or NULL
status_t push();
status_t handleVerDeleting();
status_t deleteVer(version_t* verToDelete);
status_t deleteFromChildren(version_t* verToDelete, version_t* parent);
void moveBackChildren(version_t* parentPtr, int verPos); //move children with indexes verPos + 1..childrenNum - 1 to previous
status_t copyVerChildren(version_t* prevParent); //copy version's children to it's parent with version's operations saving
status_t relocateChild(version_t* prevParent, int i);
status_t addChild(version_t* newChild, version_t* parent); //add version to child array without any editing
void cleanupVersion(version_t** ver);

/*Rebase.c*/
status_t handleRebasing();
status_t rebase(int verNum);
status_t setVerAsRoot(version_t* newRoot);

/*Merge.c*/
status_t handleMerging();
status_t merge(int verNum);
int* getOffsets(verList_t* pathToBuf, int size); //get an array which shows how many chars before each one from original text
void updateOffsetsForVer(operation_t* opEl, int* offset, int size);
int findClosestIndex(int valueToFind, int* array, int size);
status_t saveMergeInBuffer(int* bufOffset, int* verOffset, int size, version_t* ver);