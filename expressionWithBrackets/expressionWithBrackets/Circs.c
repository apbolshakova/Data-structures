#include "Header.h"

bool_t isDigit(char symbol)
{
	if ('0' <= symbol && symbol <= '9') return TRUE;
	else return FALSE;
}

bool_t isHexDigit(char symbol)
{
	if (isDigit(symbol) || ('A' <= symbol && symbol <= 'F'))  return TRUE;
	else return FALSE;
}

bool_t isValidNumberSystem(int num)
{
	if (MIN_NUMBER_SYSTEM <= num && num <= MAX_NUMBER_SYSTEM) return TRUE;
	else return FALSE;
}

bool_t isValidDigit(int numberSystem, char symbol)
{
	int digit = symbolToInt(symbol);
	if (digit == INVALID_DIGIT || digit >= numberSystem) return FALSE;
	return TRUE;
}

bool_t isOperator(char symbol)
{
	if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/') return TRUE;
	else return FALSE;
}

int symbolToInt(char symbol)
{
	if ('0' <= symbol && symbol <= '9') return symbol - '0';
	if ('A' <= symbol && symbol <= 'F') return symbol - 'A' + 10;
	return INVALID_DIGIT;
}