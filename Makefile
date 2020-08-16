all:
	rm -rf test_sample_kv
	gcc -g -O0 -std=gnu99  -DEV_STANDALONE -DEV_USE_EPOLL=1  utils.h utils.c hash_list.h hash_list.c hashfn.h hashfn.c \
	log.h log.c thread.h thread.c kv.h main.c -o test_sample_kv -lev -lpthread
clean:
	rm -rf test_sample_kv