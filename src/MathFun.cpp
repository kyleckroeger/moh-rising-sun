// Partial reconstruction; boundaries and inferred types are recorded in
// docs/MathFun.md. The original file contains additional unrecovered functions.
extern "C" int rand(void);
extern "C" void srand(unsigned int seed);
extern int g_bMathFunRandomSeeded;

int MathFunCloseToZero(float value, float epsilon)
{
    if (value > epsilon)
        return 0;
    return value >= -epsilon;
}

void MathFunSRandom(unsigned int seed)
{
    srand(seed);
    g_bMathFunRandomSeeded = 1;
}

unsigned int MathFunRandomUI32Raw()
{
    return rand();
}

int MathFunRandomSign()
{
    return (rand() & 2) - 1;
}

int MathFunRandomSignOrZero()
{
    return int(((long long)rand() * 3) >> 31) - 1;
}

long long MathFunRandomI64(long long minimum, long long maximum)
{
    long long range = maximum - minimum + 1;
    return minimum + ((range * rand()) >> 31);
}
