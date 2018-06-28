#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char c = 'a';
	char* p = (char*)malloc(sizeof(char));
	memcpy(p, &c, sizeof(char));
	printf("%p\n", p);

	return 0;
}
