#include "Header.h"

number_t* handleBigAdd(number_t* num1, number_t* num2)
{
	if (num1->sign == POSITIVE && num2->sign == POSITIVE)
		return bigAdd(num1, num2);
	else if (num1->sign == NEGATIVE)
		return bigSub(num2, num1);
	else if (num2->sign == NEGATIVE)
		return bigSub(num1, num2);
	else
	{
		number_t* result = bigAdd(num1, num2);
		result->sign = NEGATIVE;
		return result;
	}
}

number_t* handleBigSub(number_t* num1, number_t* num2)
{
	if (num1->sign == POSITIVE && num2->sign == POSITIVE)
		return bigSub(num1, num2);
	else if (num1->sign == NEGATIVE)
	{
		number_t* result = bigAdd(num1, num2);
		result->sign = NEGATIVE;
		return result;
	}
	else if (num2->sign == NEGATIVE)
		return bigAdd(num1, num2);
	else
		return bigSub(num2, num1);
}

number_t* handleBigMul(number_t* num1, number_t* num2)
{
	number_t* result = bigMul(num1, num2);
	if (num1->sign != num2->sign)
	{
		result->sign = NEGATIVE;
	}
	return result;
}

number_t* handleBigDiv(number_t* num1, number_t* num2)
{
	if (num2->asString == '0')
	{
		printf("ERROR: attempt to dizide by zero.\n");
		return NULL;
	}
	number_t* result = bigDiv(num1, num2);
	if (num1->sign != num2->sign)
	{
		result->sign = NEGATIVE;
	}
	return result;
}

number_t* bigAdd(number_t* num1, number_t* num2)
{
	int digit1 = 0, digit2 = 0, carry = 0, temp = 0;
	number_t* result = (number_t*)malloc(sizeof(number_t));
	result->numberSystem = CALC_NUMBER_SYSTEM;
	result->sign = POSITIVE;
	result->stringLen = max(num1->stringLen, num2->stringLen) + 1; //+1 дл€ учЄта переноса
	result->asString = (char*)calloc(result->stringLen + 1, sizeof(char)); //+1 дл€ нулевого символа
	char* sav = result->asString;

	while (*(num1->asString) && *(num2->asString)) //основной цикл
	{
		digit1 = *(num1->asString) - '0';
		digit2 = *(num2->asString) - '0';
		temp = carry + digit1 + digit2;

		*(result->asString) = temp % CALC_NUMBER_SYSTEM + '0';
		carry = temp / CALC_NUMBER_SYSTEM;
		
		result->asString++;
		num1->asString++;
		num2->asString++;
	}

	while (*(num1->asString)) //первое число длиннее
	{
		digit1 = *(num1->asString) - '0';
		temp = carry + digit1;

		*(result->asString) = temp % CALC_NUMBER_SYSTEM + '0';
		carry = temp / CALC_NUMBER_SYSTEM;
		result->asString++;
		num1->asString++;
	}

	while (*(num2->asString)) //второе число длиннее
	{
		digit2 = *(num2->asString) - '0';
		temp = carry + digit2;

		*(result->asString) = temp % CALC_NUMBER_SYSTEM + '0';
		carry = temp / CALC_NUMBER_SYSTEM;
		result->asString++;
		num2->asString++;
	}

	if (carry) //остаток
	{
		*(result->asString) = carry + '0';
		result->asString++;
	}
	*(result->asString) = '\0';
	result->asString = sav;
	return result;
}

number_t* bigSub(number_t* minuend, number_t* subtrahend)
{
	int borrow;
	int temp;
	int i;

	number_t* result = (number_t*)malloc(sizeof(number_t));
	result->numberSystem = CALC_NUMBER_SYSTEM;
	result->sign = POSITIVE;
	result->stringLen = max(minuend->stringLen, subtrahend->stringLen);
	result->asString = (char*)calloc(result->stringLen + 1, sizeof(char)); //+1 дл€ нулевого символа
	char* sav = result->asString;
	borrow = 0;

	while (*(minuend->asString) || *(subtrahend->asString))
	{
		char mDigit = *(minuend->asString);
		char sDigit = *(subtrahend->asString);
		temp = mDigit - borrow - sDigit;
		if (temp > 0) borrow = 0;
		else 
		{
			temp += 10;
			borrow = 1;
		}
		*(result->asString) = (char)temp % 10;

		//≈сли какой-то NULL - не увеличивать адрес, иначе - увеличивать
	}
}

int max(int a, int b)
{
	if (a >= b) return a;
	return b;
}
