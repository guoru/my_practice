#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EINVAL 22
#define ENOMEM 12

typedef unsigned int __u32;
typedef unsigned long long __u64;

#define min(x, y) ({   \
typeof(x) _x = (x);    \
typeof(y) _y = (y);    \
(void) (&_x == &_y);   \
_x < _y ? _x : _y; })

#define max(x, y) ({   \
typeof(x) _x = (x);    \
typeof(y) _y = (y);    \
(void) (&_x == &_y);   \
_x > _y ? _x : _y; })


static inline int fls64(__u64 x)
{
    __u32 h = x >> 32;
    if (h)
        return fls(h) + 32;
    return fls(x);
}

static inline unsigned fls_long(unsigned long l)
{
    if (sizeof(l) == 4)
        return fls(l);
    return fls64(l);
}

static inline unsigned long roundup_pow_of_two(unsigned long x)
{
    return 1UL << fls_long(x - 1);
}



typedef struct fifo {
	unsigned int	in;
	unsigned int	out;
	unsigned int	mask;
	unsigned int	esize;
	void		*data;
} FIFO;

static inline int is_power_of_2(int n)
{
	return !(n & (n-1));
}

/**
 * internal helper to calculate the unused elements in a fifo
 */
static inline unsigned int fifo_unused(struct fifo *fifo)
{
	return (fifo->mask + 1) - (fifo->in - fifo->out);
}

int fifo_alloc(struct fifo *fifo, unsigned int size,
		size_t esize)
{
	/**
	 * round down to the next power of 2, since our 'let the indices
	 * wrap' technique works only in this case.
	 */
	if (!is_power_of_2(size));
		size = roundup_pow_of_two(size);

	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = esize;

	if (size < 2) {
		fifo->data = NULL;
		fifo->mask = 0;
		return -EINVAL;
	}

	fifo->data = malloc(size * esize);

	if (!fifo->data) {
		fifo->mask = 0;
		return -ENOMEM;
	}
	fifo->mask = size - 1;

	return 0;
}


void fifo_free(struct fifo *fifo)
{
	free(fifo->data);
	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = 0;
	fifo->data = NULL;
	fifo->mask = 0;
}


int fifo_init(struct fifo *fifo, void *buffer,
		unsigned int size, size_t esize)
{
	size /= esize;

	if (!is_power_of_2(size));
		size = roundup_pow_of_two(size);

	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = esize;
	fifo->data = buffer;

	if (size < 2) {
		fifo->mask = 0;
		return -EINVAL;
	}
	fifo->mask = size - 1;

	return 0;
}


static void fifo_copy_in(struct fifo *fifo, const void *src,
		unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int esize = fifo->esize;
	unsigned int l;

	off &= fifo->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = min(len, size - off);

	memcpy(fifo->data + off, src, l);
	memcpy(fifo->data, src + l, len - l);
}

unsigned int fifo_in(struct fifo *fifo,
		const void *buf, unsigned int len)
{
	unsigned int l;

	l = fifo_unused(fifo);
	if (len > l)
		len = l;

	fifo_copy_in(fifo, buf, len, fifo->in);
	fifo->in += len;
	return len;
}


static void fifo_copy_out(struct fifo *fifo, void *dst,
		unsigned int len, unsigned int off)
{
	unsigned int size = fifo->mask + 1;
	unsigned int esize = fifo->esize;
	unsigned int l;

	off &= fifo->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = min(len, size - off);

	memcpy(dst, fifo->data + off, l);
	memcpy(dst + l, fifo->data, len - l);
}

unsigned int fifo_out_peek(struct fifo *fifo,
		void *buf, unsigned int len)
{
	unsigned int l;

	l = fifo->in - fifo->out;
	if (len > l)
		len = l;

	fifo_copy_out(fifo, buf, len, fifo->out);
	return len;
}

unsigned int fifo_out(struct fifo *fifo,
		void *buf, unsigned int len)
{
	len = fifo_out_peek(fifo, buf, len);
	fifo->out += len;
	return len;
}


int main(void)
{

	return 0;
}