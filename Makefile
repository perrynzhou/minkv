all:
	rm -rf test_sample_kv
	gcc -g -O0 -std=gnu99  -DEV_STANDALONE -DEV_USE_EPOLL=1  utils.h utils.c queue.h queue.c hash_list.h hash_list.c hashfn.h hashfn.c \
	log.h log.c thread.h thread.c thread_ev_io.h thread_ev_io.c  kv.h main.c -o test_sample_kv -lev -lpthread
clean:
	rm -rf test_sample_kv