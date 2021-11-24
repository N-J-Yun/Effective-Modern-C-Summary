// 참조에 대한 참조는 기본적으로 위법이다.
// 하지만 특별한 문맥 4가지에서는 참조 축약을 활용하여 허용된다.

// 참조는 lvalue, rvalue 2개 밖에 없으므로 참주 축약은 4가지 상황에서 일어날 수 있다.
// 두 참조 중 하나라도 lvalue라면 참조 축약은 lvalue로 결정된다.
// 하지만 rvalue & rvalue라면 참조 축약은 rvalue로 결정된다.

// 1. 템플릿 인스턴스화
template <typename T>
void f(T&& fParam)
{
	...
	 someFunc(std::forward<T>(fParam));
}

template<typename T>
T&& forward(remove_reference_t<T>& param)						// C++14에서의 대략적인 forward 구현
{
	return static_cast<T&&>(param);
}

// fParam에 lvalue의 Widget이 들어올 때 forward는 어떻게 인스턴스화 될까? 
// lvalue의 Widget이므로 T는 Widget&로 추론된다.
Widget& && forward(remove_reference_t<Widget&>& param)
{
	return static_cast<Widget& &&>(param);
}

// 그리고 위 상황에서는 참조 축약이 발생하며 remove_reference_t<Widget&>은 Widget을 산출한다.
Widget& forward(Widget& param)
{
	return static_cast<Widget&>(param);
}
// 템플릿의 인스턴스화 과정에서 참조 축약이 발생하는 것을 확인할 수 있다.

// 2. auto 타입 추론
Widget w;
auto&& w1 = w;
// auto의 타입추론에 의해 w의 타입은 lvalue의 Widget으로 추론된다.

Widget& && w1 = w;
// 그리고 참조 축약에 의해 Widget& && 은 Widget&으로 축약된다.

Widget& w1 = w;
// auto 타입추론 과정에서 참조 축약이 발생하는 것을 확인할 수 있다.

// 3. typedef와 별칭 선언의 지정 및 사용
template<typename T>
class Widget
{
public:
	typedef T&& RvalueRefToT;
// rvalue 참조 타입에 대한 typdef가 선언되었다.
	...
};

Widget<int&> w;
// Widget의 lvalue타입으로 인스턴스화 한다면?

typedef int& && RvalueRefToT;
// RvalueRefToT는 위처럼 인스턴스화 된다.

typedef int& RvalueRefToT;
// typdef와 별칭 사용 과정에서 참조 축약이 발생하였다.

// 4. decltype의 지정 및 사용
// auto와 유사하다.