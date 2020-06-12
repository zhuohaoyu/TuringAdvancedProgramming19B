extern void* malloc(unsigned int);
extern void free(void*);

void Test6_func1(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 1;
	free(mem);
}

void Test6_func2(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 2;
	free(mem);
}

void Test6_func3(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 3;
	free(mem);
}

void Test6_func4(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 4;
	free(mem);
}

void Test6_func5(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 5;
	free(mem);
}

void Test6_func6(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 6;
	free(mem);
}

void Test6_func7(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 7;
	free(mem);
}

void Test6_func8(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 8;
	free(mem);
}

void Test6_func9(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 9;
	// free(mem);
}

void Test6_func10(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 10;
	free(mem);
}

void Test6_func11(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 11;
	free(mem);
}

void Test6_func12(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 12;
	free(mem);
}

void Test6_func13(unsigned int size)
{
	int *mem = malloc(size);
	*mem = 13;
	free(mem);
	free(mem + 1);
}
