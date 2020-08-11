all:
	rm -rf test_sample_kv
	gcc -g -O0 -std=gnu99  utils.h utils.c hashfn.h hashfn.c \
	queue_item.h queue_item.c queue.h queue.c connection.h connection.c \
	thread.h thread.c sample_kv.h main.c -o test_sample_kv -levent -lpthread