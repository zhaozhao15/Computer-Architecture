alias gcc = 'gcc -Wpedantic -Wall -Wextra -Werror --std=c89'
gcc -c skiplist.c
ar rcs liblist.a skiplist.o
gcc -o staticlist test.c -static -L. -llist