#include <stdio.h>/*include the head files needed*/
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "tsp.h"/*include the head files needed*/

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int weight = 2147483647;
int route[100];          /*save the route*/
int t;        /*for test now now now now now*/
int n;
int s;
int map[100][100];/*save the adjacency matrix*/

int get_next_route()/*get the next route*/
{
    int tmp,i,j,k;
    int save[100];/*save the tmp route var*/

    pthread_mutex_lock(&lock);
    for (i = n-3; i >= 0; i--)/*get the first i that a[i]<a[i+1]*/
    {
        if (route[i] < route[i+1])
        {
            break;
        }
        if (i == 0)
        {
            return -1;       /*it already is the last route, we can end*/
        }                 
    }

    for (j = n-2; j > i; j--)           /*get the last aj > ai*/
    {
        if (route[j] > route[i])
        {
            break;
        }
    }

    tmp = route[i];/*swap a[i] and a[j]*/
    route[i] = route[j];
    route[j] = tmp;
    
    k = 0;
    for (j = n-2; j > i; j--)          /*save inverse */
    {
        save[k] = route[j];
        k=k+1;
    }
    k = 0;
    for (j = i+1; j <= n-2; j++)        /*replace the origin with inverse*/
    {
        route[j] = save[k];   
        k++;
    }

    pthread_mutex_unlock(&lock);          /*unlock*/
    return 0;
}

int get_total_weight()            /*get the current weight*/
{
	int i;
	int total_weight;
	int myroute[100];            /*save the copy of the route*/

	pthread_mutex_lock(&lock);
	for (i = 0; i <= n-2; i++)
	{
		myroute[i] = route[i];       /*copy the route*/
	}
	pthread_mutex_unlock(&lock);
	
	if (map[0][myroute[0]] == 0 || map[myroute[n-2]][0] == 0)
	{
		return 2147483647;           /*this path is not available*/
	}

	total_weight = map[0][myroute[0]];/*initialize the weight*/

	for (i = 0; i < n-2; i++)
	{
		if (map[myroute[i]][myroute[i+1]] == 0)   
		{
			return 2147483647;       /*this path is not available*/
		}
		total_weight += map[myroute[i]][myroute[i+1]];/*do the add up*/
	}

	total_weight += map[myroute[n-2]][0];      
	
	return total_weight;/*get weight*/
}

void* exec(){
	for (;;)
	{
		int wei;
		if (get_next_route() == -1)/*we can end the route*/
		{
			pthread_mutex_unlock(&lock);
			return NULL;
		}/*exit the thread*/

		wei = get_total_weight(route);
		pthread_mutex_lock(&lock);
		if ( wei < weight)/*we get a better solution*/
		{
			weight = wei;
		}
		pthread_mutex_unlock(&lock);
	}
}


int main()
{
	int data[1000];       /*save the input*/
	int i,j,k;

	pthread_t checkThread[20];    /*save the threads*/

	i = 0;
	while (scanf("%d%*c", data + i++) == 1)
		;                   /*read the input*/

	t = data[0];
	n = data[1];
	s = data[2];/*get threads, cities, start*/

	if (s > n-1 || s < 0)
	{
		printf("%d\n", -1);/*invalid input*/
		return 0;
	}

	k = 3;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)/*initialize the adjacency matrix*/
		{
			map[i][j] = data[k];
			k++;
		}
	}

	for (i = 0; i <= n-2; i++)/*initialize the route*/
	{
		route[i] = i+1;
	}

	weight = get_total_weight(); /*get weight*/

	for (i = 0; i < t; i++) {     /*create threads*/
        pthread_create(&checkThread[i], NULL, &exec, NULL);
    }

    for (i = 0; i < t; i++) {     /*end threads*/
        pthread_join(checkThread[i], NULL);
    }

    if (weight == 2147483647)     /*no path is solution*/
    {
    	printf("%d\n", -1);
    }
    else
    {
    	printf("%d\n", weight);     /*out put the solution*/
    }

	return 0;
}