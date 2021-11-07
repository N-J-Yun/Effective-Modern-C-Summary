void f(Widget&& param);									// rvalue 참조

Widget&& var1 = Widget();								// rvalue 참조

template<typename T>									// rvalue 참조
void f(std::vector<T>&& param);

auto&& var2 = va1;										// rvalue 참조 X	

template<typename T>									// rvalue 참조 X	
void f(T&& param);

// 일반적으로 어떤 타입 T에 대한 rvalue 참조를 선언할 때에는 T&&라는 표기를 사용한다.
// 하지만 모든 코드에서 T&&는 rvalue 참조가 아니다.

// 형태가 T&&이고, T타입이 추론되는 경우와, auto&&로 객체를 선언하는 경우에는 universal reference이다.
// universal reference는 두 가지 상황에서 발생한다.
// 두 경우 모두 타입 추론이 발생한다.

// 1. 템플릿 매개변수
template<typename T>
void f(T&& param);										// 참조 선언의 형태는 반드시 T&&이어야 한다.
														// const가 붙어도 안된다.
// 형태가 T&&라고 해서 모두 universal reference인 것은 아니다.
// 타입추론이 발생해야지만 universal refernence가 된다.

// 2. auto 선언
auto&& var2 = var1;
// 이러한 경우는 생각보다 잘 없지만 C++14이후로 lambda 표현식을 사용하면서 그 빈도가 늘었다.

auto timeFuncInvocation = 
	[](auto&& func, auto&&... params)
	{
		...
		std::forward<decltype(func)>(func)(
			std::forward<decltype(params)>(params)...
		);
		...
	}
// 위 코드의 디테일은 무시하고, auto&&가 사용되었으며 이 형태가 생각보다 자주 쓰인다는 사실을 기억해두자. 

// universal reference는 caller로부터의 전달값과 대응된다.
template<typename T>
void f(T&& param);

int main()
{
	Widget w;
	f(w);												// lvalue가 전달되어 param은 Widget&가 된다(lvalue refernce)

	f(std::move(w));									// rvalue가 전달되어 param은 Widget&&가 된다(rvalue refernce)

	return 0;
}
