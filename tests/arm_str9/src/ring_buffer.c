#include "ring_buffer.h"

void rb_init(ring_buffer_t *rb, rb_data_t *buffer, unsigned int size)
{
    rb->start	= buffer;
    rb->in	= buffer;
    rb->out	= buffer;
    rb->end	= buffer + size;
    rb->qty	= 0;
}

int rb_put(ring_buffer_t *rb, rb_data_t data)
{
    if (rb_full(rb)) return 0;
    *rb->in = data;
    ++rb->qty;
    if (++rb->in == rb->end)
	    rb->in = rb->start;
    return 1;
}

int rb_get(ring_buffer_t *rb, rb_data_t *data)
{
    if (!rb->qty) return 0;
    *data = *rb->out;
    --rb->qty;
    if (++rb->out == rb->end)
	    rb->out = rb->start;
    return 1;
}

unsigned int rb_put_ptr(ring_buffer_t *rb, unsigned char *buffer, unsigned int size)
{
    /*
     * STR9 USB FIFO only allows 32 bits data exchange, so we take care of
     * it here
     */

    register unsigned int temp;
    register unsigned int i = 0;
    unsigned int *buf32 = (unsigned int *) buffer;
    
    for (;;) {
    	/* We must read in units of 32 bits */
    	temp = *buf32; /* reading beyond the limit is ok because we don't have MMU */
    	if (!rb_put(rb, (rb_data_t) (temp & 0xFF)) || ++i == size) break;
    	if (!rb_put(rb, (rb_data_t) (temp >> 8)) || ++i == size) break;
    	if (!rb_put(rb, (rb_data_t) (temp >> 16)) || ++i == size) break;
    	if (!rb_put(rb, (rb_data_t) (temp >> 24)) || ++i == size) break;
    	++buf32;
    }

    return i;
}

unsigned int rb_get_ptr(ring_buffer_t *rb, unsigned char *buffer, unsigned int size)
{
    unsigned int temp;
    register unsigned int *buf32 = (unsigned int *) buffer;
    register unsigned int i;
    const unsigned int qty = size < rb->qty ? size : rb->qty;

    for (i = 0; i < qty; i += sizeof(unsigned int)) {
    	temp = 0;
    	rb_get(rb, (rb_data_t *) &temp);
    	rb_get(rb, ((rb_data_t *) &temp) + 1);
    	rb_get(rb, ((rb_data_t *) &temp) + 2);
    	rb_get(rb, ((rb_data_t *) &temp) + 3);
    	*buf32++ = temp;
    }

    return qty;
}

int rb_empty(ring_buffer_t *rb)
{
    return 0 == rb->qty;
}

int rb_full(ring_buffer_t *rb)
{
    return rb->in == rb->out && rb->qty != 0;
}
