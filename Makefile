all:
	rm -rf test_sample_kv
	gcc -g -O0 -std=gnu99  utils.h utils.c hash_list.h hash_list.c hashfn.h hashfn.c \
	log.h log.c queue_item.h queue_item.c queue.h queue.c connection.h connection.c \
	thread.h thread.c sample_kv.h main.c -o test_sample_kv -levent -lpthread
clean:
	rm -rf test_sample_kv