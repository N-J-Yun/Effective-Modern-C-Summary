// 배열 -> 포인터로의 붕괴는 함수 -> 포인터로도 적용된다.
void someFunc(int, double);

template<typename T>
void f1(T param);					// param을 값으로 받는다.

template<typename T>
void f2(T& param);					// param을 참조로 받는다.

int main()
{
	f1(someFunc);					// param은 함수 포인터로 추론된다.
									// ParamType = void(*)(int, double)

	f2(someFunc);					// param은 함수 참조로 추론된다.
									// ParamType = void(&)(int, double)

	return 0;
}
// 배열에서 포인터로의 붕괴를 알고있다면
// 함수에서 포인터로의 붕괴도 알아두자.