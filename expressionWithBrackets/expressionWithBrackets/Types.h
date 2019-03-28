#ifndef TYPES
#define TYPES

typedef enum Bool_ { FALSE, TRUE } bool_t;
typedef enum FuncResult_ { FAIL, SUCCESS } func_result_t;
typedef enum NumberSign_ { POSITIVE, NEGATIVE } number_sign_t;

typedef struct Number_
{
	char* asString;
	int stringLen;
	int numberSystem;
	number_sign_t sign;
} number_t;

/*Линейный список ОПЗ*/
typedef struct OpzElValue_
{
	char sign;
	number_t* number;
} opz_list_el_value;

typedef struct OpzEl_
{
	opz_list_el_value* value;
	struct OpzEl_* next;
} opz_list_el;

/*структура стека символов, используемая при получении ОПЗ*/
typedef struct OperatorStackEl_
{
	char sign;
	struct OperatorStackEl_* next;
} operator_stack_el;

/*стек чисел для подсчёта опз*/
typedef struct NumberStackEl_
{
	number_t* data;
	struct NumberStackEl_* next;
} number_stack_el;

#endif