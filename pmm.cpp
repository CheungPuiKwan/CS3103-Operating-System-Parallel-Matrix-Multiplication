#include <pthread.h>
#include <stdio.h>

#define MATRIX_SIZE 2000
#define THREADS_NUM 4

int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int matrixC[MATRIX_SIZE][MATRIX_SIZE];
int n, p, m;

struct thread_args
{
	int from;
	int to;
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void init()
{
	scanf("%d%d%d", &n, &p, &m);

	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < p; ++j)
		{
			scanf("%i", &matrixA[i][j]);
		}
	}

	for (size_t i = 0; i < p; ++i)
	{
		for (size_t j = 0; j < m; j++)
		{
			scanf("%i", &matrixB[i][j]);
		}
	}
}

void *multiply(void *arg)
{
	struct thread_args *parsed_args = (struct thread_args *)arg;
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < p; ++j)
		{
			for (size_t k = parsed_args->from; k < parsed_args->to; ++k)
			{
				matrixC[i][k] += matrixA[i][j] * matrixB[j][k];
			}
		}
	}
}

void printResult()
{
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < m; ++j)
		{
			printf("%i ", matrixC[i][j]);
		}
	}
}

int main(int argc, char *argv[])
{
	init();

	pthread_t threads[THREADS_NUM];
	struct thread_args args[THREADS_NUM];
	int current_from = 0, chunk_size = m / THREADS_NUM;

	for (size_t i = 0; i < THREADS_NUM; ++i)
	{
		args[i].from = current_from;
		args[i].to = current_from + chunk_size;
		current_from += chunk_size;
	}
	args[THREADS_NUM - 1].to = m;

	for (size_t i = 0; i < THREADS_NUM; ++i)
	{
		pthread_create(&threads[i], NULL, multiply, &args[i]);
	}
	for (size_t i = 0; i < THREADS_NUM; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	printResult();

	return 0;
}