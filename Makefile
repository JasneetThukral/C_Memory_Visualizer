CC = clang  #compiler
TARGET_MAIN = main
TARGET_TEST = test

main:
	$(CC) main.c -o $(TARGET_MAIN)

test: 
	$(CC) -O0 -g test.c stack_mod.c vector_mod.c dictionary_mod.c queue_mod.c creator_methods.c -o $(TARGET_TEST)

clean:
	rm $(TARGET_MAIN) $(TARGET_TEST)
	