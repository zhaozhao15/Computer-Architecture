#include <stdio.h>
#include <stdlib.h>
#include "skiplist.h"
#define getName(var) #var


int main(int argc, char * argv []) {
	long int i; int toprint; long int circles; int ind; int newval;
	int j; int res; int *val;

	/* initialize some previous value */
	int keys[20] = {20,14,16,17,13,15,12,11,18,3,4,6,7,8,9,10,2,5,1,19};
	int vals[20] = {7,9,5,3,2,4,6,8,0,7,5,3,3,5,7,98,0,5,2,34};

	if (argv) {}
	toprint = 0;
	if (argc == 1) toprint = 1;

	/* start the process repeatedly to magnify the memory leak */
	circles = 1;
	for (i = 0; i < circles; i++) {
		list skpli = createSkipList(3, &compareInt, &allocInt, &deleteInt);

		printf("Check attributes in empty list:\n");
		if (skpli->level == 1) printf("     Check level: Correct!\n");
		else printf("     Check level: Wrong!\n");
		if (skpli->header->key == NULL) printf("     Check head key: Correct!\n");
		else printf("     Check head key: Wrong!\n");
		if (skpli->header->val == NULL) printf("     Check head value: Correct!\n");
		else printf("     Check head value: Wrong!\n");


		printf("Starting testing with empty skiplist\n");
		printf("   Searching:\n");
		ind = 0;
		val = searchNode(skpli, keys + ind);
		if (val == NULL) {printf("     testing searching in empty list --Success!\n");}
		else {printf("     testing searching in empty list, reach error output.\n");}
		printf("   End Searching\n");
		printf("   Deleting:\n");
		ind = 3;
		res = deleteNode(skpli, keys + ind);
		if (res == -1) {printf("     testing deleting in empty list --Success!\n");}
		else {printf("     testing deleting in empty list, reach error in output.\n");}
		printf("   End Deleting\n");
		printf("   Inserting:\n");
		ind = 0;
		res = insertNode(skpli, keys + ind, vals + ind);
		if (res == 0) {printf("     testing inserting into empty list --Success!\n");}
		else {printf("     testing inserting into empty list, reach error in output.\n");}
		printf("     Current skiplist level: %d\n", skpli->level);
		printf("   End Inserting\n");
		printf("          s->header->forward[0]: %p\n", (void *)skpli->header->forward[0]);
		printf("   Deleting:\n");
		ind = 0;
		res = deleteNode(skpli, keys + ind);
		if (res == 0) {printf("     testing deleting list with one node --Success!\n");}
		else {printf("     testing deleting in empty list, reach error in output.\n");}
		printf("     Current level: %d\n", skpli->level);
		printf("   End Deleting\n");

		printf("Starting inserting the rest of the keys and values\n");
		for (j = 0; j < 5; j++) {
			res = insertNode(skpli, keys + j, vals + j);
			if (res == -1) {if (toprint == 1) printf("     reported insertion failed \n");}
			else if (res == 0) {if (toprint == 1) printf("     insert done for %d-th key-value\n", j+1);}
		}
		printf("     Current skiplist level: %d\n", skpli->level);


		printf("Starting searching node\n");
		ind = 18;
		val = searchNode(skpli, &(keys[ind]));
		if (val == NULL) printf("     reported search failed!\n");
		else {
			if (toprint == 1) printf("     first searching result: %d \n", *val);
			if (*val == vals[ind]) {if (toprint == 1) printf("     search result: Correct!\n");}
			else {if (toprint == 1) printf("     search result: WRONG!\n");}
		}


		printf("Starting inserting existing node!\n");
		ind = 1; newval = 123;
		vals[ind] = newval;
		res = insertNode(skpli, keys + ind, vals + ind);
		if (res == -1) {if (toprint == 1) printf("     reported insertion failed\n");}
		else if (res == 1){
			val = searchNode(skpli, keys + ind);
			if (toprint == 1) printf("     reported insertion sucessed\n     the anwser is %d \n", *val);
		} else {
			printf("     the insertion return value wrong\n");
		}
		printf("Starting searching the changed value\n");
		val = searchNode(skpli, &(keys[ind]));
		if (toprint == 1) printf("     searching result: %d \n", *val);
		if (*val == vals[ind]) {printf("     search result: Correct!\n");}
		else {printf("     search result: WRONG!\n");}


		printf("Starting delete node\n");
		ind = 18;
		res = deleteNode(skpli, keys + ind);
		if (res == 0) printf("     reported delete node succeed\n");
		if (res == -1) {if (toprint == 1) printf("     reported delete node failed\n");}
		printf("End delete node\n");

		printf("Starting search unexisting node with way larger key\n");
		ind = 23;
		val = searchNode(skpli, &ind);
		if (val == NULL) printf("     The answer is true NULL\n");
		else printf("     The answer has something wrong");

		printf("Starting search unexisting node with deleted key\n");
		ind = 18;
		val = searchNode(skpli, keys + ind);
		if (val == NULL) printf("     The answer is true NULL\n");
		else printf("     The answer has something wrong\n");

		printf("Starting delete unexisting node\n");
		ind = 18;
		res = deleteNode(skpli, keys + ind);
		if (res == -1) {if (toprint == 1) printf("     reported delete node failed-And that's right\n");}
		printf("End delete node\n");

		printf("Starting inserting existing node!\n");
		ind = 3; newval = 56;
		vals[ind] = newval;
		res = insertNode(skpli, keys + ind, vals + ind);
		if (res == -1) {if (toprint == 1) printf("     reported insertion failed");}
		else {
			val = searchNode(skpli, keys + ind);
			if (toprint == 1) printf("     reported insertion sucessed\n     the anwser is %d \n", *val);
		}
		printf("Starting searching the changed value\n");
		val = searchNode(skpli, &(keys[ind]));
		if (toprint == 1) printf("     searching result: %d \n", *val);
		if (*val == vals[ind]) {printf("     search result: Correct!\n");}
		else {printf("     search result: WRONG!\n");}

		printf("Starting inserting back the deleted node\n");
		ind = 18;
		res = insertNode(skpli, keys + ind, vals + ind);
		if (res == 0) printf("     The reinserting is Succeed!\n");
		else printf("     The reinserting is Failed!\n");

		printf("Starting deleting all the nodes\n");
		for (j = 0; j < 5; j++) {
			res = deleteNode(skpli, keys + j);
			if (res == -1) {if (toprint == 1) printf("     reported deleting failed \n");}
			else if (res == 0) {if (toprint == 1) printf("     delete done for %d-th key-value\n", j+1);}
		}
		printf("     Current skiplist level: %d\n", skpli->level);		

		printf("Free the skip list at last\n");
		freeSkipList(skpli);
	}
	printf("Hi there! The test for the skip list is finished!\n");
	return 0;
}

/*

#define DEBUG
#include <stdio.h>
#define PTPTR(ptr) printf("%p \n", ptr)

*/