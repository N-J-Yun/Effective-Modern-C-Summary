int main()
{
	int x(0);
	int y = 0;
	int z{0};
	int z = {0};			// 이 코드는 대부분의 c++ 컴파일러에서 z{0} 와 동일하게 취급된다.

	return 0;
}
// 위의 4가지 초기화 방법은 결과는 같다.
// 하지만 {}를 사용하는 초기화방식이 가장 범용적이다.
class Widget
{
private:
	int x { 0 };			// x의 default값은 0이다.
	int y = 0;				// y의 default값은 0이다.
	int z(0);				// error, default값 지정에는 괄호를 사용할 수 없다.
};

std::atomic<int> ai1{ 0 };		// ai1의 초기값은 0이다.
std::atomic<int> ai2(0);		// ai2의 초기값은 0이다.
std::atomic<int> ai3 = 0;		// error, 초기값 지정에 = 을 사용할 수 없다.
// 이렇듯 {}초기화 방식만이 대부분의 경우에서 적용가능하다.

// {}초기화 방식은 narrowing conversion을 방지하는 역할도 할 수 있다.
int main()
{
	double x, y, z;

	int sum1{x + y + z};		// error!

	int sum2(x + y + z);		// ok
	int sum3 = x + y + z;		// ok, 하지만 이런 예전부터 지원되오던 코드는 narrowing conversion을 막지 않는다.
								// legacy 코드를 너무 많이 건드릴 수 있기 때문이다.
	return 0;
}

// 또한 {}초기화 방식은 성가신 코드를 막아주는 효과도 있다.
Widget w1(10);					// Widget의 생성자에 10을 전달한다.

Widget w2();					// *성가신 코드*, Widget을 return하는 함수를 선언한다!
	
Widget w3{};					// Widget의 생성자를 아무 인수도 없이 호출한다.

// 하지만 일반적으로 ()초기화보다 {}초기화를 선호하라고 하지는 않는다.
class Widget
{
public:
	Widget(int i, bool b);
	Widget(int i, double d);
};

int main()
{
	Widget w1(10, true);		// 첫 번째 생성자를 호출한다.
	Widget w2{10, true};		// 첫 번째 생성자를 호출한다.

	Widget w3(10, 5.0);			// 두 번째 생성자를 호출한다.
	Widget w4{10, 5.0};			// 두 번째 생성자를 호출한다.

	return 0;
}
// 기초적인 경우 ()와 {}를 통한 생성자 호출은 같은 역할을 수행한다.

class Widget
{
public:
	Widget(int i, bool b);
	Widget(int i, double d);
	Widget(std::initializer_list<long double> il);

	operator float() const;		// float로 변환시킨다.(암시적 변환 허용)
};

int main()
{
	Widget w1(10, true);		// 첫 번째 생성자를 호출한다.
	Widget w2{10, true};		// 세 번째 생성자를 호출한다!(10과 true를 long double로 변환해서)

	Widget w3(10, 5.0);			// 두 번째 생성자를 호출한다.
	Widget w4{10, 5.0};			// 세 번째 생성자를 호출한다!(10과 5.0을 long double로 변환해서)

	Widget w5(w4);				// 복사 생성자를 호출한다.
	Widget w6{w4};				// 세 번째 생성자를 호출한다!(w4를 float로 변환 시키고, 다시 long double로 변환시킨다.)

	Widget w7(std::move(w4));	// 이동 생성자를 호출한다.
	Widget w8{std::move(w4)};	// w6때와 같은 행동을 취한다.

	return 0;
}
// 하지만 위 예시처럼 std::initializer_list를 인자로 받는 생성자가 있다면 
// 부합하는 타입과 관계없이 인수가 변환될 수만 있다면 {}를 모두 initializer_list를 인자로 받는 생성자를 호출한다.
// 변환될 수 없다면 기존처럼 정상적으로 호출된다.(ex. std::string -> int)

class Widget
{
public:
	Widget();
	Widget(std::initializer_list<int> il);
};

// 하지만 인자 없이 전달할 때 default 생성자를 호출하고 싶다면 {}를 사용하거나 아무것도 적지 않아야 한다.
Widget w1;						// default 생성자 호출
Widget w2{};					// default 생성자 호출
Widget w3();					// 함수 선언

// 만약 인자 없이 initializer_list를 인자로 받는 생성자를 호출하고 싶다면 아래처럼 호출한다.
Widget w4({});
Widget w5{{}};

// 위의 예시 같은 경우는 생각한 것보다 자주 발생하는 문제이다.
// 따라서 일반적으로 사용하는 초기화 방법을 정해두고, 그 예외에 대해서만 나머지 하나를 사용하도록 하자.
// 그리고 정한 방법을 일관성 있게 사용하도록 하자.