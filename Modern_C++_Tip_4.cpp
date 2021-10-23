template<typename T>
void f(ParamType param);

f(expr);
// template 에서의 타입 추론
// ParamType과 T를 추론한다.



template<typename T>
void func_for_x(T param);

template<typename T>
void func_for_cx(const T param);

template<typename T>
void func_for_rx(const T& param);

int main()
{	

	// case 3(포인터도 참조도 아닌 경우)
	auto x = 27;						// x는 int
	func_for_x(27);						// 위와 같은 추론결과

	const auto cx = x;					// cx는 const int
	func_for_cx(x);						// 위와 같은 추론결과

	// case 1(포인터or참조이지만 universal reference가 아닌 경우)
	const auto& rx = x;					// rx는 const int&
	func_for_rx(x);						// 위와 같은 추론결과

	// case 2(universal reference인 경우)
	auto&& uref1 = x;					// x는 lvalue이므로 uref1은 int&로 추론

	auto&& uref2 = 27;					// 27은 rvalue이므로 uref2는 int&&로 추론

	// 붕괴(decay)
	const char name[] = "asdf";

	auto arr1 = name;					// arr1의 타입은 const char*
	auto& arr2 = name;					// arr2의 타입은 const char(&)[5]

	void someFunc(int, double);

	auto func1 = someFunc;				// func1의 타입은 void(*)(int, double)
	auto& func2 = someFunc;				// func2의 타입은 void(&)(int, double)

	return 0;
}
// auto의 타입 추론은 template 타입 추론과 거의 일치한다.

// 하지만 예외도 있다.
int main()
{
	// C++98 부터
	int x1 = 27;
	int x2(27);

	// C++11 부터
	int x3 = { 27 };
	int x4{ 27 };

	// 대입결과는 모두 같다 변수 xn에 int 27을 대입한 것
	// 하지만 auto를 사용하면 얘기가 달라진다.

	auto x1 = 27;						// 타입은 int로 추론, 값은 27
	auto x2(27);						// 타입은 int로 추론, 값은 27

	auto x3 = { 27 };					// 타입은 std::initializer_list<int>로 추론, 값은 { 27 }
	auto x4{ 27 };						// 타입은 std::initializer_list<int>로 추론, 값은 { 27 }
	// auto는 중괄호 리스트로 된 값으로 초기화를 할 때 타입을 std::initializer_list로 추론한다. 
	auto x5 = { 1, 2, 3.0 };
	// 만약 중괄호 리스트 안에 값들의 타입이 일치하지 않으면 error가 발생한다.

	return 0;
}

// 사실 x5 예제에서는 두 가지의 타입 추론이 발생한다.
// 한 가지는 auto의 x5에 대한 타입 추론이고
// 다른 한 가지는 std::initializer_list<T>의 T에 대한 타입 추론이다.

template<typename T>
void f(T param);

template<typename T>
void f2(std::initializer_list<T> initList);

int main()
{
	auto x = {3,4,5};					// x의 타입은 std::initializer_list<int>로 추론된다.

	f({3,4,5});							// error, T의 타입을 추론할 수 없다.
	f2({3,4,5});						// T는 int, initList의 타입은 std::initializer_list<int>로 추론된다.

	return 0;
}
// auto와 template 타입 추론의 차이는 {} 초기화 리스트를 만났을 때
// auto는 std::initializer_list타입으로 바로 추론을 한다는 것이고,
// template은 그렇지 않다는 것이다.

// C++11까지는 위 내용이 끝이지만 C++14는 예외가 좀 더 있다.

// C++14에서는 함수 반환값이 auto인 것을 허용해주는데 여기서 return 값이 {}리스트라면?
auto createInitList()
{
	return { 1, 2, 3 }; 				// error, { 1, 2, 3 }의 타입을 추론할 수 없다.  
}
// 여기서는 auto 타입 추론이 일어나지 않고 template 타입 추론이 일어난다.
// 따라서 타입을 추론할 수 없게 된다.

// C++14의 lambda에서도 똑같은 일이 발생한다.
std::vector<int> v;

auto resetV =
[&v](const auto& newValue) { v = newValue; }; 

resetV({ 1, 2, 3 }); 					// error, { 1, 2, 3 }의 타입을 추론할 수 없다.  