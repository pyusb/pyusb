#ifndef RING_BUFFER_H_INCLUDED
#define RING_BUFFER_H_INCLUDED

typedef unsigned char rb_data_t;

#define rb_size(rb) ((rb)->qty)

typedef struct ring_buffer
{
    rb_data_t *start;	    /* start of the buffer */
    rb_data_t *in;	    /* position to put the next byte */
    rb_data_t *out;	    /* position to get the next byte */
    rb_data_t *end;	    /* one element past of the last element of the buffer */
    unsigned int qty;	    /* number of bytes in the buffer */
} ring_buffer_t;

/*
 * Initialize the ring buffer structure
 *
 * Arguments:
 *  rb - ring buffer
 *  buffer - buffer used by the data structure
 *  size - size of the buffer
 */
void rb_init(ring_buffer_t *rb, rb_data_t *buffer, unsigned int size);

/*
 * Put one byte in the buffer
 *
 * Arguments:
 *  rb - ring buffer
 *  data - data to put
 *
 *  returns: 1 if ok, 0 if buffer is full
 */
int rb_put(ring_buffer_t *rb, rb_data_t data);

/*
 * Get one byte from the ring buffer
 *
 * Arguments:
 *
 *  rb - ring buffer
 *  data - returns with the data read
 *
 *  returns: 1 if ok, 0 if buffer is full
 */
int rb_get(ring_buffer_t *rb, rb_data_t *data);

/*
 * Put a buffer of the data into the ring buffer
 *
 * Returns the number of data written
 */
unsigned int rb_put_ptr(ring_buffer_t *rb, unsigned char *buffer, unsigned int size);

/*
 * Read size bytes to the buffer
 *
 * Returns the number of bytes read
 */
unsigned int rb_get_ptr(ring_buffer_t *rb, unsigned char *buffer, unsigned int size);

/*
 * Returns != 0 if it is empty
 */
int rb_empty(ring_buffer_t *rb);

/*
 * Returns != 0 if it is full
 */
int rb_full(ring_buffer_t *rb);

#endif /* RING_BUFFER_H_INCLUDED */
