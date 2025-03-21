#include <stdlib.h>
#include "onomondo/softsim/mem.h"

void *port_malloc(size_t size) { return malloc(size); }
void port_free(void *p) { free(p); }
