template<typename T>
void f(ParamType param);

f(expr);							// T와 ParamType을 expr로 부터 추론한다.

// ParamType의 타입에 따라 T를 추론하는 방법이 달라진다.
// ParamType의 타입의 경우를 3가지로 나눌 수 있다.

// case 1. 포인터 또는 참조자 타입인데 보편 참조(universal reference)가 아닐 경우
// 1) expr의 타입이 참조자라면 그 참조자를 무시한다.
// 2) (expr -> paramtype)패턴매칭 방식으로 T타입 결정
template<typename T>
void f(T& param);					// ParamType이 참조자일 경우

int main()
{
	int x = 27;
	const int cx = x;
	const int& rx = x;

	f(x);							// T = int, ParamType = int&

	f(cx);							// T = const int, ParamType = const int&

	f(rx);							// T = const int, ParamType = const int&

	return 0;
}
// ParamType이 이미 T& 이기 때문에 expr이 참조로 들어와도 T에 적용되지 않는다.
// ParamType에 const가 없기 때문에 expr에 const가 붙어서 오면 T와 ParamType에도 적용된다.
template<typename T>
void f(const T& param);					// ParamType이 const 참조자일 경우

int main()
{
	int x = 27;
	const int cx = x;
	const int& rx = x;

	f(x);							// T = int, ParamType = const int&
	f(cx);							// T = int, ParamType = const int&
	f(rx);							// T = int, ParamType = const int&

	return 0;
}
// ParamType에 이미 const와 참조자가 다 들어와있기 때문에 expr의 const와 &는 T에 적용되지 않는다.

template<typename T>
void f(T* param);					// ParamType이 포인터일 경우(const 포인터 포함)에도 위와 동일하게 적용된다.

int main()
{
	int x = 27;
	const int *px = &x;

	f(&x);							// T = int, ParamType = int*
	f(px);							// T = const int, ParamType = const int*

	return 0;
}
// ParamType이 이미 T* 이기 때문에 expr이 포인터로 들어와도 T에 적용되지 않는다.
// ParamType에 const가 없기 때문에 expr에 const가 붙어서 오면 T와 ParamType에도 적용된다.

// case 2. 보편 참조(universal reference)일 경우
// 1) expr이 lvalue면 T와 ParamType는 둘다 lvalue 참조로 추론된다.
// 2) expr이 rvalue라면 정상적으로 case 1이 적용된다.
template<typename T>
void f(T&& param);					// ParamType이 universal reference일 경우

int main()
{
	int x = 27;
	const int cx = x;
	const int& rx = x;

	f(x);							// x는 lvalue이다 -> T = int&, ParamType = int&
	f(cx);							// x는 lvalue이다 -> T = const int&, ParamType = const int&
	f(rx);							// x는 lvalue이다 -> T = const int&, ParamType = const int&

	f(27);							// x는 rvalue이다 -> T = int, ParamType = int&&

	return 0;
}
// expr이 lvalue인지, rvalue인지에 따라 참조가 무시되거나 T, ParamType모두에게 적용된다.

// case 3. 포인터도 참조도 아닐 경우
// 값에 의한 전달이기 때문에 새로운 객체가 생기는 것이다.
// 1) expr의 타입이 참조면 참조자를 무시한다.
// 2) expr의 참조자를 무시한 후, expr이 const이거나 volatile이면 그것들 또한 무시한다.

template<typename T>
void f(T param);

int main()
{
	int x = 27;
	const int cx = x;
	const int& rx = x;

	const char* const ptr = "Fun with pointers";

	f(x);							// T, ParamType 모두 int
	f(cx);							// T, ParamType 모두 int
	f(rx);							// T, ParamType 모두 int
				
	f(ptr);							// const char* const 타입의 인수를 전달한다.
									// 이 경우 포인터 자체는 값으로 전달 되기 때문에 ptr의 const는 무시된다.(다른 객체 가리킬 수 있음)
									// 하지만 가리키던 객체의 const는 유지된다.(가리킨 객체의 값은 수정할 수 없음.)
									// 따라서 T, ParamType 모두 const char*로 추론된다.

	return 0;
}
// expr이 const 값이었음에도 ParamType이 const가 적용되지 않았음을 인지하자.

