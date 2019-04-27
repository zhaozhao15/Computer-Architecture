alias gcc = 'gcc -Wpedantic -Wall -Wextra -Werror --std=c89'
gcc -shared -fPIC -c skiplist.c -o liblist.so
gcc -o dynamiclist test.c -L. -llist