#include <stdio.h>

struct test_struct
{
	const int hey;
	const int doei;
};

int main(void)
{
	struct test_struct pog = {10, 5};
	printf("yoo %i and \n %i", pog.hey, pog.doei);
	return(0);
}
