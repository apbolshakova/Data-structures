#ifndef TYPES
#define TYPES

typedef enum FuncResult_ { FAIL, SUCCESS } func_result_t;

typedef struct Change
{
	char type;
	size_t beginIndex;
	size_t endIndex;
	char* data;
} change_t;

typedef struct Buf
{
	char* text;
	size_t textLen;
	size_t parentVersion;
	change_t* unsavedChange;
} buf_t;

#endif