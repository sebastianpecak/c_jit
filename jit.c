#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define CODE_BUFFER_SIZE 4096

typedef unsigned (*asmFunc)(void);

static const uint8_t AddInt[] =
{
	0x55,
	0x48, 0x89, 0xe5,
	0x89, 0x7d, 0xfc,
	0x89, 0x75, 0xf8,
	0x8b, 0x55, 0xfc,
	0x8b, 0x45, 0xf8,
	0x01, 0xd0,
	0x5d,
	0xc3
};

static const uint8_t MulInt[] =
{
        0x55,
        0x48, 0x89, 0xe5,
        0x89, 0x7d, 0xfc,
        0x89, 0x75, 0xf8,
        0x8b, 0x45, 0xfc,
        0x0f, 0xaf, 0x45, 0xf8,
        0x5d,
        0xc3
};

typedef int(*Function_t)(int, int);

#define TEST_F(testCase) \
	printf("%s: %d\n", #testCase, testCase)

int main(int argc, char *argv[])
{
	void* AddIntCode = NULL;
	void* MulIntCode = NULL;

	AddIntCode = mmap(
       		NULL,
       		CODE_BUFFER_SIZE,
       		PROT_READ | PROT_WRITE | PROT_EXEC,
		MAP_ANONYMOUS | MAP_PRIVATE,
		0,
		0
	);

        MulIntCode = mmap(
                NULL,
                CODE_BUFFER_SIZE,
                PROT_READ | PROT_WRITE | PROT_EXEC,
                MAP_ANONYMOUS | MAP_PRIVATE,
                0,
                0
        );

	// Validate pointers.
	if (AddIntCode && MulIntCode)
	{
		// Copy code to execuatble buffer.
		memcpy(AddIntCode, AddInt, sizeof(AddInt));
		memcpy(MulIntCode, MulInt, sizeof(MulInt));

		Function_t addInts = (Function_t)AddIntCode;
		Function_t mulInts = (Function_t)MulIntCode;

		// Run functions.
		TEST_F(addInts(1, 2));
		TEST_F(mulInts(2, 3));
		TEST_F(addInts(mulInts(2, 2), addInts(3, 7)));
	}
	else
	{
		printf("Could not map execuatble memory.");
	}

	return 0;
}

