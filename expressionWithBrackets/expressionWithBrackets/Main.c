#include "Header.h"

int main (void)
{
	opz_list_el* opzListHead = getOpz();
	if (opzListHead != NULL)
	{
		number_t* result = calculateOpz(opzListHead); //возвращает строку-результат вычислений
		if (result != NULL)
		{
			reverseStr(result->asString);
			if (result->sign == NEGATIVE) printf("-");
			puts(result->asString);
			//write result to output.txt
		}

	}
	_getch();
	return 0;
}