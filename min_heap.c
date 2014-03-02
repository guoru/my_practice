#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/time.h>

typedef struct event {
	int idx;
	struct timeval ev_time;
} EVENT;

typedef struct min_heap
{
	EVENT **p;
	unsigned n, a;
} min_heap_t;

#define evutil_timercmp(tvp, uvp, cmp)    \
	(((tvp)->tv_sec == (uvp)->tv_sec) ?   \
	((tvp)->tv_usec cmp (uvp)->tv_usec) : \
	((tvp)->tv_sec cmp (uvp)->tv_sec))

static inline void		min_heap_ctor(min_heap_t *s);
static inline void		min_heap_dtor(min_heap_t *s);
static inline void		min_heap_elem_init(EVENT *e);
static inline int		min_heap_elem_greater(EVENT *a, EVENT *b);
static inline int		min_heap_empty(min_heap_t *s);
static inline unsigned	min_heap_size(min_heap_t *s);
static inline EVENT*	min_heap_top(min_heap_t *s);
static inline int 		min_heap_reserve(min_heap_t *s , unsigned int n);
static inline int		min_heap_push(min_heap_t *s, EVENT *e);
static inline EVENT*	min_heap_pop(min_heap_t *s);
static inline int		min_heap_erase(min_heap_t *s, EVENT *e);
static inline void		min_heap_shift_up_(min_heap_t *s, unsigned hole_index, EVENT *e);
static inline void		min_heap_shift_down_(min_heap_t *s, unsigned hole_index, EVENT *e);

int min_heap_elem_greater(EVENT *a, EVENT *b)
{
	return evutil_timercmp(&a->ev_time, &b->ev_time, >);
}

void min_heap_ctor(min_heap_t *s) { s->p=0; s->n=0; s->a=0; }
void min_heap_dtor(min_heap_t *s) { if(s->p) free(s->p); }
void min_heap_elem_init(EVENT *e) { e->idx=-1; }
int min_heap_empty(min_heap_t *s) { return 0u==s->n; }
unsigned min_heap_size(min_heap_t *s) { return s->n; }
EVENT* min_heap_top(min_heap_t *s) { return s->n ? *s->p : 0; }

int min_heap_push(min_heap_t *s, EVENT *e)
{
	if (min_heap_reserve(s, s->n+1))
		return -1;
	min_heap_shift_up_(s, s->n++, e);
	return 0;
}

EVENT * min_heap_pop(min_heap_t *s)
{
	if(s->n)
	{
		EVENT *e = *s->p;
		min_heap_shift_down_(s, 0u, s->p[--s->n]);
		e->idx = -1;
		return e;
	}
	return 0;
}

int min_heap_erase(min_heap_t *s, EVENT *e)
{
	if (-1 != e->idx)
	{
		EVENT *last = s->p[--s->n];
		unsigned parent = (e->idx - 1)/2;
		
		if(e->idx>0 && min_heap_elem_greater(s->p[parent], last))
			min_heap_shift_up_(s, e->idx, last);
		else
			min_heap_shift_down_(s, e->idx, last);
		e->idx = -1;
		return 0;
	}
	return -1;
}

int min_heap_reserve(min_heap_t *s, unsigned n)
{
	if (s->a < n)
	{
		EVENT **p;
		unsigned a = s->a ? s->a*2 : 8;
		if (a<n)
			a = 0;
		if(!(p=(struct event**)realloc(s->p, a*sizeof(*p))))
			return -1;
		s->p = p;
		s->a = a;
	}
	return 0;
}

void min_heap_shift_up_(min_heap_t *s, unsigned hole_index, EVENT *e)
{
	unsigned parent = (hole_index -1)/2;
	while (hole_index && min_heap_elem_greater(s->p[parent], e))
	{
		(s->p[hole_index] = s->p[parent])->idx = hole_index;
		hole_index = parent;
		parent = (hole_index -1)/2;
	}
	(s->p[hole_index] = e)->idx = hole_index;
}

void min_heap_shift_down_(min_heap_t *s, unsigned hole_index, EVENT *e)
{
	unsigned min_child = 2*(hole_index + 1);
	while (min_child <= s->n)
	{
		min_child -= min_child == s->n || min_heap_elem_greater(s->p[min_child], s->p[min_child-1]);
		if (!(min_heap_elem_greater(e, s->p[min_child])))
			break;
		(s->p[hole_index] = s->p[min_child])->idx = hole_index;
		hole_index = min_child;
		min_child = 2*(hole_index + 1);
	}
	(s->p[hole_index] = e)->idx = hole_index;
}

void heap_print(min_heap_t *t){
	int i = 0;
	for(; i<t->n; i++)
		printf("time:%u, idx:%d\n", t->p[i]->ev_time.tv_sec, t->p[i]->idx);
}

int main(int argc, char **argv)
{
	min_heap_t *p;
	EVENT *tmp;
	int i;
	p = (min_heap_t *)calloc(1, sizeof(min_heap_t));
	min_heap_ctor(p);
	for(i=0;i<10;i++){
		struct event *e=(struct event *)calloc(1, sizeof(EVENT));
		e->ev_time.tv_sec = i+1;
		min_heap_push(p,e);
	}
	heap_print(p);
	for(i=1;i<10;i++){
		tmp = min_heap_pop(p);
		printf("pop time:%u, idx:%d\n", tmp->ev_time.tv_sec, tmp->idx);
		heap_print(p);
	}
	return 0;
}