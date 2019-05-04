#ifndef TYPES
#define TYPES

typedef enum FuncResult_ { FAIL, SUCCESS } func_result_t;

typedef struct Change //операция над текстом
{
	char type;
	int beginIndex;
	int endIndex;
	char* data;
} change_t;

typedef struct Buf //несохранённая версия со всеми изменениями в неё
{
	char* text;
	int textLen;
	int verNum;
	int parentVer;
	change_t* unsavedChange;
} buf_t;

#endif