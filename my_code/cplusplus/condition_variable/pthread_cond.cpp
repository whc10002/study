#include <pthread.h>
#include <assert.h>
#include <iostream>

#define BSIZE (1024)

typedef struct {
	char buf[BSIZE];
	int occupied;
	int nextin;
	int nextout;
	pthread_mutex_t mutex;
	pthread_cond_t more;
	pthread_cond_t less;
} buffer_t;

buffer_t buffer;

void producer(buffer_t *b, char item)
{
	pthread_mutex_lock(&b->mutex);

	while (b->occupied >= BSIZE)
		pthread_cond_wait(&b->less, &b->mutex);

	assert(b->occupied < BSIZE);

	b->buf[b->nextin++] = item;

	b->nextin %= BSIZE;
	b->occupied++;

	/* now: either b->occupied < BSIZE and b->nextin is the index
	   of the next empty slot in the buffer, or
	   b->occupied == BSIZE and b->nextin is the index of the
	   next (occupied) slot that will be emptied by a consumer
	   (such as b->nextin == b->nextout) */

	pthread_cond_signal(&b->more);

	pthread_mutex_unlock(&b->mutex);
}

char consumer(buffer_t *b)
{
	char item;
	pthread_mutex_lock(&b->mutex);
	while(b->occupied <= 0)
		pthread_cond_wait(&b->more, &b->mutex);

	assert(b->occupied > 0);

	item = b->buf[b->nextout++];
	b->nextout %= BSIZE;
	b->occupied--;

	/* now: either b->occupied > 0 and b->nextout is the index
	   of the next occupied slot in the buffer, or
	   b->occupied == 0 and b->nextout is the index of the next
	   (empty) slot that will be filled by a producer (such as
	   b->nextout == b->nextin) */

	pthread_cond_signal(&b->less);
	pthread_mutex_unlock(&b->mutex);

	return(item);
}

int main(int argc, char* argv[])
{
	return 0;
}
