#include "skiplist.h"
#include "stdlib.h"
#include "stdio.h"
#define RAND_HALF (RAND_MAX)/2
/*Skip List homework*/

/*Create an empty list*/
list createSkipList(int height_limit,
					int (*cmp)(const void *a, const void *b),
					void *(*alloc)(const void *),
					void (*del)(void *))
	/*three function pointers for comparing/allocing memory/deleting memory.*/
{
	int i;
	list newlist = (list) malloc(sizeof(struct skip_list));
	node header = (node) malloc(sizeof(struct skip_list_node));/*give them memory space*/
	newlist->header = header;
	newlist->height_limit = height_limit;
	newlist->level = 1;                     /*empty skiplist has level 1*/
	newlist->header->forward = (node *) malloc(sizeof(node)*height_limit);
	for (i = 0; i < height_limit; i++)
	{
		newlist->header->forward[i] = NULL;/*header points to a dumb node*/
	}
	newlist->cmp = cmp;
	newlist->alloc = alloc;/*three function pointers*/
	newlist->del = del;
	newlist->header->val = NULL;
	newlist->header->key = NULL;/*point to a dummy node*/
	return newlist;
}


void *searchNode(list s, void *key)
{
	/*initializing a pointer x*/
	node x;
	int i;
    x = s->header;
    /*go from header to the end*/
    if (key == NULL)
    {
    	return NULL;
    }
    /*this is wrong case*/
    for (i = (s->level)-1; i >= 0; i--)
    {	
    	while (1)
    	{	
    		if (x->forward[i] == NULL)/*arrive the end of this level*/
    		{
    			break;
    		}
    		else if (s->cmp(x->forward[i]->key,key)==-1)/*key is still small, can go further*/
    		{
    			x = x->forward[i];
    		}
    		else
    		{
    			break;/*found a place to go to down level*/
    		}
    	}
	}

    x = x->forward[0];/*go forward*/

	if (x == NULL)
	{
		/*arrive the end of this level*/
		return NULL;
	}
    else if (s->cmp(x->key,key) == 0)
    {
        return x->val;/*found the same key*/
    }
    else
    {
        return NULL;/*not found, failure*/
    }
}


int insertNode(list s, void *key, void *val)
{	
	node *update;/*to store pointers pointing to node*/
	node x;
	int i,lv;

	if (key == NULL)
		/*failure case*/
    {
    	return -1;
    }
    update = (node *) malloc(sizeof(node)*(s->height_limit));
    /*give update memory space*/
    x = s->header;
    
    lv = 0;
	while (lv < s->height_limit - 1 && rand() < RAND_HALF) ++lv;
	/*generate a height randomly*/
	

    for (i = (s->level)-1; i >= 0; i--)
    {	/*from high level to low level*/
    	while (1)
    	{	
    		if (x->forward[i] == NULL)
    		{
    			/*arrive the end of this level*/
    			break;
    		}
    		else if (s->cmp(x->forward[i]->key,key)==-1)
    		{
    			/*key is small, we can go further*/
    			x = x->forward[i];
    		}
    		else
    		{
    			/*found a place to go down*/
    			break;
    		}
    	}
    update[i] = x;
	}
	/*store revelant pointers on each level*/
    x = x->forward[0];
    
    if (x == NULL)
    {	
    	node newnode;
    	/*arrive the end of the skiplist*/
    	if (lv+1 > (s->level))
		{	
    		for (i = s->level; i <= lv; i++)
    		{
    			/*new pointers on higher level*/
    			update[i] = s->header;
    		}
    		s->level = lv+1;
    	}
    	newnode = (node) malloc(sizeof(struct skip_list_node));/*give space for a new node*/
       	newnode->key = s->alloc(key);
    	newnode->val = s->alloc(val);
    	newnode->forward = (node *) malloc(sizeof(node)*(s->height_limit));
    	/*give space to the pointers*/
    	for (i = 0; i <= lv; i++)
    	{
    		newnode->forward[i] = NULL;
            update[i]->forward[i] = newnode;
    	}
    	/*update the pointers and exchange some*/
    	free(update);
    	return 0;
    }
    else if (s->cmp(key,x->key)==0)
    	/*when key already existed*/
    {
    	s->del(x->val);
    	x->val = s->alloc(val);
    	free(update);
    	/*reassign the value*/
    	return 1;
    }else if (s->cmp(key,x->key)==-1)
    {/*can go further*/
    	node newnode;
    	if (lv+1 > (s->level))                 /*big lv*/
		{	
    		for (i = s->level; i <= lv; i++)
    		{
    			update[i] = s->header;/*update pointers on higher levels*/
    		}
    		s->level = lv+1;
    	}
    	newnode = (node) malloc(sizeof(struct skip_list_node));/*give space to new node*/
    	newnode->key = s->alloc(key);
    	newnode->val = s->alloc(val);
    	newnode->forward = (node *) malloc(sizeof(node)*(s->height_limit));/*give space to pointers*/
    	for (i = 0; i <= lv; i++)
    	{
    		newnode->forward[i] = update[i]->forward[i];/*update pointers*/
            update[i]->forward[i] = newnode;
    	}
    	/*give back memory*/
    	free(update);
    	return 0;
    }
    else
    {
    	/*failure cases*/
    	free(update);
    	return -1;
    }
}


int deleteNode(list s, void *key)/*delete a node*/
{
	int i;
	node *update = (node *) malloc(sizeof(node)*(s->height_limit));
	node x = s->header;
	
	for (i = (s->level)-1; i >= 0; i--)/*from high level to low level*/
    {	
    	while (1)
    	{	
    		if (x->forward[i] == NULL)/*arrive the end of the level*/
    		{
    			break;
    		}
    		else if (s->cmp(x->forward[i]->key,key)==-1)
    		{
    			x = x->forward[i];/*key is small, we can go further*/
    		}
    		else
    		{
    			break;/*found a place to go down*/
    		}
    	}
    update[i] = x;
	}
	/*record the recelant pointers*/
    x = x->forward[0];


    if (x == NULL)/*reach the end of the skiplist*/
    {
    	
    	free(update);/*free pointers*/
    	return -1;
    }
    else if (s->cmp(x->key,key) == 0)
    {
    	
        for (i = 0; i < s->level; i++)/*found the key*/
        {
            update[i]->forward[i] = x->forward[i];/*update the pointers*/
        }
        if (x)
        {
        	free(x->forward);/*free pointers*/
        	s->del(x->key);
        	s->del(x->val);
        	free(x);/*free space*/
    	}
    	
        while (s->level > 1 && s->header->forward[(s->level)-1] == NULL)/*test a level is empty*/
        {
        	s->level--;
        }
        free(update);/*free memory*/
        
        return 0;
    }
    else
    {	/*failure cases*/
    	return -1;
    }
}


void freeSkipList(list s)/*free whole list*/
{
	node current_node = s->header;/*from the head*/
	node next_node;
    while(current_node->forward[0] != NULL)/*when list is not empty*/
    {
		next_node = current_node->forward[0];
		deleteNode(s,current_node);
		current_node = next_node;/*go next and free again*/
    }
	free(s);
}


int compareInt(const void *a, const void *b)/*compare two int*/
	{
		if (*(int *)a < *(int *)b)/*small*/
		{
			return -1;
		}else if (*(int *)a > *(int *)b)/*big*/
		{
			return 1;
		}
		else
		{
			return 0;/*equal*/
		}
	}


void deleteInt(void *a)
	{
		free(a);/*free a node*/
	}


void *allocInt(const void *a)/*give space to an int*/
	{	
		int *ans;
		ans = (int *)malloc(sizeof(int));
		*ans = *(int *)a;
		return ans;/*give space done*/
	}
