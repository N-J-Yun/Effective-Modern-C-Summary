// 배열타입에 대한 추론은 포인터 타입 추론과 적용 규칙이 다르다.

const char name[] = "j. p. briggs";			// name의 타입은 const char[13]이다.
const char* ptrToName = name;				// 컴파일러는 이같은 대입을 허용한다.

// 타입 추론에 대해선 위 같은 상황이 어떻게 적용될까?
template<typename T>
void f(T param);							// 값에 의한 전달을 한다.

int main()
{
	f(name);								// name은 배열이지만 T는 const char*로 추론된다.

	return 0;
}

void myFunc(int param[]);					// 배열 매개변수 선언과 포인터 매개변수 선언이
void myFunc(int* param);					// 위 상황처럼 같은 선언으로 취급되기 때문

// 똑같이 적용되었다. 하지만 타입 추론에서는 같은 선언으로 취급되어 오히려 문제가 되었다.
// name은 const char[13] 타입인데 타입 추론 결과 const char*가 되어 다른 타입이 추론되어 버렸기 때문

// 하지만 배열에 대한 참조를 사용하면 얘기는 달라진다.
template<typename T>
void f(T& param);							// 참조에 의한 전달을 한다.

int main()
{
	f(name);								// 배열에 대한 참조를 f로 전달한다.
											// 이렇게 되면 배열의 size까지 포함하여 참조하기 때문에
											// T는 const char[13], ParamType은 const char(&)[13]으로 추론된다.

	return 0;
}

// 이를 배열에 담긴 원소들의 갯수를 추론하는 템플릿을 만드는 데 활용할 수도 있다.
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
	return N;
}

int main()
{
	int keyVals[] = { 1, 3, 5, 7, 11, 22, 35 };				// 7개의 요소를 가지고 있다.

	std::array<int, arraySize(keyVals)> mappedVals;			// 7개의 요소를 가지는 배열 컨테이너를 생성하였다.

	return 0;
}

