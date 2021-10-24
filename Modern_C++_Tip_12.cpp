// 포인터와 정수 타입이 같이 오버로딩 되어있을 때 포인터를 받는 오버로딩 버전을 호출하고 싶다면?
// ***이런 짓은 하지 않는게 좋다.***
void f(int);
void f(bool);

void f(void*);

int main()
{
	f(0);											// f(int)를 호출한다.
	f(NULL);										// 컴파일 되지 않을 수도 있고, 된다면 f(int)를 호출한다.

	f(nullptr);										// f(void*)를 호출한다.

	return 0;
}
// 0은 int이지 포인터가 아니다.
// Null 또한 구현에 따라 int 이외의 정수 타입을 부여할 수도 있다.
// 하지만 nullptr을 쓰면 위 상황처럼 오버로딩이 예상과 다르게 호출되는 일이 없어진다.

// nullptr을 쓰면 코드의 명확성도 좋아진다.(특히 auto와 함께 썼을 때)
int main()
{
	auto result = findRecord(...arguments...)

	if(result == 0) {...}							// 이렇게 선언하면 findRecord가 어떤 타입을 return 하는 지 알 수 없기 때문에
													// 포인터를 return 하는지 정수형 타입을 return 하는지 구분할 수 없다.
													// 코드가 애매모호 해졌다.

	if(result == nullptr) {...}						// 하지만 nullptr을 사용함으로써 findRecord의 return 타입이 포인터 타입임을 예측할 수 있다.

	return 0;
}

// nullptr은 std::nullptr_t타입이며 std::nullptr_t는 모든 포인터로 변환될 수 있다.
// 이러한 특성은 템플릿이 관여하면 더 빛이 난다.

int 	f1(std::shared_ptr<Widget> spw);
double 	f2(std::unique_ptr<Widget> upw);
bool 	f3(Widget* pw);
// mutex로 락을 걸어주어야만 하는 함수 3개가 위처럼 선언되어있다고 할 때 이를 사용한다면
int main()
{
	std::mutex f1m, f2m, f3m;
	using MuxGuard = std::lock_guard<std::mutex>;
	{
		MuxGuard g(f1m);
		auto result = f1(0);
	}

	{
		MuxGuard g(f2m);
		auto result = f2(NULL);
	}

	{
		MuxGuard g(f3m);
		auto result = f3(nullptr);
	}

	return 0;
}
// 이렇게 사용할 수 있다. 앞의 두 코드는 동작하지 않지만, 일단 코드가 반복되니 이를 템플릿으로 바꿔보자
template<typename FuncType,
		typename MuxType,
		typename PtrType>
decltype(auto) lockAndCall(FuncType func,				// decltype(auto)은 C++14에서 사용가능
							MuxType& mutex,
							PtrType ptr)			
{
	MuxGuard g(mutex);
	return func(ptr);
}

int 	f1(std::shared_ptr<Widget> spw);
double 	f2(std::unique_ptr<Widget> upw);
bool 	f3(Widget* pw);

int main()
{
	std::mutex f1m, f2m, f3m;
	using MuxGuard = std::lock_guard<std::mutex>;		

	auto result1 = lockAndCall(f1, f1m, 0);				// 이 호출은 FuncType이 f1인 템플릿을 인스턴스화한다.
														// 0은 템플릿 타입 추론에 의해서 int로 추론된다.
														// 따라서 0은 f1의 인자로써 들어가게 되는데 f1은 shared_ptr<Widget>타입을 
														// 입력받고 int 는 shared_ptr<Widget>타입으로 변환할 수 없기 때문에 에러가 발생한다.

	auto result2 = lockAndCall(f2, f2m, NULL);			// 위 상황과 정확히 같은 이유로 에러가 발생한다.

	auto result3 = lockAndCall(f3, f3m, nullptr);		// 하지만 nullptr은 템플릿 타입 추론에 의해 std::nullptr_t타입으로 추론되고
														// std::nullptr_t타입은 어떤 포인터로든 변환될 수 있다.
														// 따라서 f3은 nullptr을 Widget* 타입으로 변환하여 입력받을 수 있기 때문에
														// 의도한대로 정상적으로 호출이 완료되었다.
	return 0;
}

// 이처럼 포인터 타입에 대해서는 0이나 null 대신 nullptr을 쓰는 것이 무조건 좋다. 아니 옳다.