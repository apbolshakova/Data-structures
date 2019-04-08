#include "Header.h"

number_t* handleBigAdd(number_t* num1, number_t* num2)
{
	if (num1->sign == POSITIVE && num2->sign == POSITIVE)
		return bigAdd(num1, num2);
	else if ((num1->sign == NEGATIVE) && (num2->sign == NEGATIVE))
	{
		number_t* result = bigAdd(num1, num2);
		result->sign = NEGATIVE;
		return result;
	}
	else if (num1->sign == NEGATIVE)
	{
		if (maxNumber(num2, num1) >= 0) return bigSub(num2, num1);
		else
		{
			number_t* result = bigSub(num1, num2);
			result->sign = NEGATIVE;
			return result;
		}
	}
	else if (num2->sign == NEGATIVE)
	{
		if (maxNumber(num1, num2) >= 0) return bigSub(num1, num2);
		else
		{
			number_t* result = bigSub(num2, num1);
			result->sign = NEGATIVE;
			return result;
		}
	}
}

number_t* handleBigSub(number_t* num1, number_t* num2)
{
	if (num1->sign == POSITIVE && num2->sign == POSITIVE)
	{
		if (maxNumber(num1, num2) >= 0) return bigSub(num1, num2);
		else
		{
			number_t* result = bigSub(num2, num1);
			result->sign = NEGATIVE;
			return result;
		}
	}
	else if (num1->sign == NEGATIVE && num2->sign == POSITIVE)
	{
		number_t* result = bigAdd(num1, num2);
		result->sign = NEGATIVE;
		return result;
	}
	else if (num1->sign == POSITIVE && num2->sign == NEGATIVE)
		return bigAdd(num1, num2);
	else
	{
		if (maxNumber(num2, num1) >= 0) return bigSub(num2, num1);
		else
		{
			number_t* result = bigSub(num1, num2);
			result->sign = NEGATIVE;
			return result;
		}
	}
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
	if (num2->stringLen == 1 && num2->asString[0] == '0')
	{
		printf("ERROR: attempt to divide by zero.\n");
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
	result->stringLen = max(num1->stringLen, num2->stringLen) + 1; //+1 для учёта переноса
	result->asString = (char*)calloc(result->stringLen + 1, sizeof(char)); //+1 для нулевого символа
	char* sav = result->asString;
	char* numStr1 = num1->asString;
	char* numStr2 = num2->asString;

	while (*numStr1 || *numStr2) //основной цикл
	{
		if (*numStr1) digit1 = *numStr1 - '0';
		else digit1 = 0;
		if (*numStr2) digit2 = *numStr2 - '0';
		else digit2 = 0;
		temp = carry + digit1 + digit2;

		*(result->asString) = temp % CALC_NUMBER_SYSTEM + '0';
		carry = temp / CALC_NUMBER_SYSTEM;
		
		result->asString++;
		if (*numStr1) numStr1++;
		if (*numStr2) numStr2++;
	}
	if (carry) //остаток
	{
		*(result->asString) = carry + '0';
		result->asString++;
	}
	*(result->asString) = '\0';
	result->asString = sav;
	trimZeros(result);
	return result;
}

number_t* bigSub(number_t* minuend, number_t* subtrahend)
{
	int borrow = 0, temp = 0;
	char mDigit = 0, sDigit = 0;
	number_t* result = (number_t*)malloc(sizeof(number_t));
	result->numberSystem = CALC_NUMBER_SYSTEM;
	result->sign = POSITIVE;
	result->stringLen = max(minuend->stringLen, subtrahend->stringLen);
	result->asString = (char*)calloc(result->stringLen + 1, sizeof(char)); //+1 для нулевого символа
	char* sav = result->asString;
	char* mString = minuend->asString;
	char* sString = subtrahend->asString;

	while (*mString || *sString)
	{
		if (*mString) mDigit = *mString - '0';
		else mDigit = 0;
		if (*sString) sDigit = *sString - '0';
		else sDigit = 0;
		temp = mDigit - borrow - sDigit;
		if (temp >= 0) borrow = 0;
		else 
		{
			temp += 10;
			borrow = 1;
		}
		*(result->asString) = (char)temp % 10 + '0';

		++result->asString;
		if (*mString) mString++;
		if (*sString) sString++;
	}
	*(result->asString) = '\0';
	result->asString = sav;
	trimZeros(result);
	return result;
}

number_t* bigMul(number_t* num1, number_t* num2)
{
	number_t* result = (number_t*)malloc(sizeof(number_t));
	result->numberSystem = CALC_NUMBER_SYSTEM;
	result->sign = POSITIVE;
	result->stringLen = num1->stringLen + num2->stringLen;
	result->asString = (char*)calloc(result->stringLen + 1, sizeof(char)); //+1 для нулевого символа
	number_t* row = num1;	
	for (int i = 0; i < num2->stringLen; i++) 
	{
		for (int j = 1; j <= num2->asString[i] - '0'; j++) 
		{
			result = bigAdd(result, row);
		}
		digitShift(row, 1);
	}
	trimZeros(result);
	return result;
}

number_t* bigDiv(number_t* dividend, number_t* divisor)
{
	number_t* result = (number_t*)malloc(sizeof(number_t));
	result->numberSystem = CALC_NUMBER_SYSTEM;
	result->sign = POSITIVE;
	result->stringLen = dividend->stringLen;
	result->asString = (char*)calloc(result->stringLen + 1, sizeof(char)); //+1 для нулевого символа
	
	number_t* row = (number_t*)malloc(sizeof(number_t));
	row->numberSystem = CALC_NUMBER_SYSTEM;
	row->sign = POSITIVE;
	row->stringLen = 0;
	row->asString = (char*)calloc(result->stringLen + 1, sizeof(char));

	for (int i = dividend->stringLen - 1; i >= 0; i--) 
	{
		digitShift(row, 1);
		row->asString[0] = dividend->asString[i];
		result->asString[i] = '0';
		while (maxNumber(row, divisor) >= 0)
		{
			result->asString[i]++;
			row = bigSub(row, divisor);
		}
		if (!(isDigit(result->asString[i]))) result->asString[i] = '0';
	}
	trimZeros(result);

	free(row);
	row = NULL;
	return result;
}

void digitShift(number_t* n, int d) //умножить n на 10^d
{
	for (int i = n->stringLen; i >= 0; i--)
	{
		n->asString[i + d] = n->asString[i];
	}
	for (int i = 0; i < d; i++) n->asString[i] = '0';
	n->stringLen = n->stringLen + d;
}

void trimZeros(number_t* num)
{
	while ((num->stringLen > 1) && ((num->asString[num->stringLen - 1] == 0) || 
		                             num->asString[num->stringLen - 1] == '0'))
		num->stringLen--;

	if ((num->stringLen == 1) && (num->asString[0] == '0'))
		num->sign = POSITIVE;
	char* temp = (char*)realloc(num->asString, num->stringLen + 1);
	num->asString = temp;
	num->asString[num->stringLen] = '\0';
}

number_t* bigPow(int base, long exponent)
{
	number_t* baseNum = (number_t*)malloc(sizeof(number_t));
	baseNum->numberSystem = CALC_NUMBER_SYSTEM;
	baseNum->sign = POSITIVE;
	baseNum->asString = (char*)calloc(2, sizeof(char));
	sprintf(baseNum->asString, "%lu", base);
	reverseStr(baseNum->asString);
	baseNum->stringLen = 1;
	if (baseNum->asString[1]) baseNum->stringLen++; //число двузначно

	number_t* result = (number_t*)malloc(sizeof(number_t));
	result->numberSystem = CALC_NUMBER_SYSTEM;
	result->sign = POSITIVE;
	result->stringLen = 1 + base * exponent;
	result->asString = (char*)calloc(result->stringLen + 1, sizeof(char)); //+1 для нулевого символа
	
	result->asString[0] = '1';
	result->asString[1] = '\0';
	for (int i = 0; i < exponent; i++) result = bigMul(result, baseNum);
	trimZeros(result);
	return result;
}