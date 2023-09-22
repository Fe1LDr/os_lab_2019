#include "swap.h"

void Swap(char *left, char *right)
{
	// ваш код здесь
	char temp = *left;
    *left = *right;
    *right = temp;
}
