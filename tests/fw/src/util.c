#include "util.h"

void vic_connect(u16 source, void (*isr) (void))
{
	if (source > 15) {
		const u16 src = source - 16;
		VIC1->VAiR[src] = (unsigned int) isr;
		VIC1->VCiR[src] |= src | 0x20;
		VIC1->INTER |= 1 << src;
	} else {
		VIC0->VAiR[source] = (unsigned int) isr;
		VIC0->VCiR[source] = source | 0x20;
		VIC0->INTER |= 1 << source;
	}
}
