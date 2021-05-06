#include "pdp_trace.h"

void trace(const char * format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}