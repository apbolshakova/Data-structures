#include "Header.h"

int main (void)
{
	opz_list_el* opzListHead = getOpz();
	if (opzListHead != NULL)
	{
		number_t* result = calculateOpz(&opzListHead);
		if (result != NULL)
		{
			reverseStr(result->asString);
			if (result->sign == NEGATIVE) printf("-");
			puts(result->asString);
		}
	}
	deleteOpzList(&opzListHead);
	_getch();
	return 0;
}