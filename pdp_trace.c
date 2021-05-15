#include "pdp_trace.h"

void trace(const char * format, ...) {
	if(trace_is_on == 0)
		return;
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

void trace_reg() {
	for(int i = 0; i < 8; i ++)
        trace("R%d:%06o ", i, reg[i]);
    trace("\n");
}