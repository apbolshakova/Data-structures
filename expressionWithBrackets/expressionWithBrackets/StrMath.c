#include "Header.h"

void add(number_t** numPtr, number_t* numToAdd)
{
	int num1 = 0, num2 = 0, carry = 0, temp = 0;
	number_t* num = *numPtr;
	number_t* result = (number_t*)malloc(sizeof(number_t));

	result->numberSystem = CALC_NUMBER_SYSTEM;
	result->stringLen = max(num->stringLen, numToAdd->stringLen) + 1; //+1 для учёта переноса
	result->asString = (char*)calloc(result->stringLen + 1, sizeof(char)); //+1 для нулевого символа
	char* sav = result->asString;

	while (*(num->asString) && *(numToAdd->asString))
	{
		num1 = *(num->asString) - '0';
		num2 = *(numToAdd->asString) - '0';
		temp = carry + num1 + num2;

		*(result->asString) = temp % CALC_NUMBER_SYSTEM + '0';
		carry = temp / CALC_NUMBER_SYSTEM;
		
		result->asString++;
		num->asString++;
		numToAdd->asString++;
	}

	while (*(num->asString))
	{
		num1 = *(num->asString) - '0';
		temp = carry + num1;

		*(result->asString) = temp % CALC_NUMBER_SYSTEM + '0';
		carry = temp / CALC_NUMBER_SYSTEM;
		result->asString++;
		num->asString++;
	}

	while (*(numToAdd->asString))
	{
		num1 = *(numToAdd->asString) - '0';
		temp = carry + num1;

		*(result->asString) = temp % CALC_NUMBER_SYSTEM + '0';
		carry = temp / CALC_NUMBER_SYSTEM;
		result->asString++;
		numToAdd->asString++;
	}

	if (carry)
	{
		*(result->asString) = carry + '0';
		result->asString++;
	}
	*(result->asString) = '\0';
	result->asString = sav;

	(*numPtr)->asString = NULL;
	free((*numPtr)->asString);
	*numPtr = NULL;
	free(*numPtr);

	*numPtr = result;
}

int max(int a, int b)
{
	if (a >= b) return a;
	return b;
}