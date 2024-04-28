#include <stdio.h>
#include <stdlib.h>

#include "g_fmt.h"

/*
 * Simple test driver to compare output from using g_fmt vs printf
 * Build with:
gcc -DIEEE_8087=1 -I../build -I.. *c -g -o g_fmt_test
 * Then run e.g.:
./g_fmt_test 1.1
 * etc...
 */
int main(int argc, char  **argv)
{
	char buf[128];
	double d;
	char *res;
	if (argc < 2)
	{
		fprintf(stderr, "Usage: g_fmt_test <number>\n");
		return 1;
	}
	d = strtod(argv[1], NULL);
	res = json_c_g_fmt(buf, d);
	printf("%s\n", res);
	printf("%0.17g\n", d);
}
