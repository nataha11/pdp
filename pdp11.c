#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdarg.h>

#define MEMSIZE (64 * 1024)

#include "pdp_mem.h"
#include "pdp_run.h"
#include "pdp_trace.h"
#include "pdp_mem.c"
#include "pdp_run.c"
#include "pdp_trace.c"

int main(int argc, char const * argv[]) {
	load_file(argc, argv);
	test_mem();
	return 0;
}





