// move 연산이 저렴하고 항상 복사가능한 매개변수에 대해서는 
// by-value가 by-reference만큼이나 효율적이고 구현하기가 더 쉽고, 산출되는 목적 코드의 크기도 더 작다.

// addName함수의 3가지 구현
// 1. overloading
class Widwget
{
public:
	void addName(const std::string& newName)
	{ names.push_back(newName); }

	void addName(std::string&& newName)
	{ names.push_back(std::move(newName)); }
	...

private:
	std::vector<std::string> names;
};
// 오버로딩을 통해 rvalue, lvalue에 따른 최적화방법은 잘 작동하지만 본질적으로 같은 일을 함수를 두 개 작성해야 하는 귀찮음이 있다.
// caller가 넘겨준 인수가 lvalue던 rvalue던 그 인수는 newName이라는 참조에 묶인다. 
// cost: lvalue - 복사 1회, rvalue - 이동 1회

// 2. universal reference
class Widget
{
public:
	template<typename T>
	void addName(T&& newName)
	{
		names.push_back(std::forward<T>(newName));
	}
	...
};
// std::forward 때문에 lvalue는 복사되고 rvalue는 이동된다.
// 하지만 universal reference와 관련된 문제점들은 있을 수 있다.
// cost: lvalue - 복사 1회, rvalue - 이동 1회

// 3. by-value
class Widget
{
public:
	void addName(std::string newName)
	{ names.push_back(std::move(newName)); }
	...
};
// caller가 넘겨준 인수가 lvalue던 rvalue던 매개변수 newName은 반드시 생성된다.
// cost: lvalue - 복사 1회 + 이동 1회, rvalue - 이동 2회
// -> by-value 방식의 매개변수를 사용하면 어떤 경우이든 이동이 1회씩 더 일어난다.

// by-value 방식을 사용할 때 주의해야할 4가지 사항

// 1. by-value를 사용하라는 것이 아니라 고려하라는 것일 뿐이다.
// by-value방식은 함수를 한 번만 작성하면 된다는 장점과, 목적 코드에 함수 하나만 만들어진다는 장점, universal reference와 관련된 문제가 없다는 장점이 있다.
// 하지만 다른 대안들보다 비용이 크다는 점도 고려해야 한다.

// 2. 복사가 가능한 매개변수에 대해서만 값 전달을 고려해야 한다.
// 복사할 수 없는 매개변수는 unique_ptr처럼 move-only 타입일 텐데
// by-value방식의 장점은 함수를 하나만 작성해도 된다는 것인데, 애초에 move-only방식은 rvalue만 취급하는 함수 하나만 작성하면 된다.
// 그리고 억지로 by-value방식을 사용하면 쓸데없는 비용(move1회)이 추가된다.

// 3. by-value는 move 연산이 저렴한 매개변수에 대해서만 고려해야 한다.
// move연산 비용이 저렴하면 move가 한 번 더 일어난다고 해도 큰 문제가 되지 않는다.
// 하지만 move 연산 비용이 크다면 불필요한 move연산이 불필요한 복사를 수행하는 것과 비슷해 질 것이다.

// 4. by-value는 항상 복사되는 매개변수에 대해서만 고려해야 한다.
// 만약 조건부로 매개변수가 복사되고, 그 외의 경우에는 아무런 일도 하지 않는 함수가 있다고 한다면
// 이 함수의 매개변수를 값으로 받았을 때 복사가 필요없는 경우에도 복사가 일어날 수 있다.

// 예외적으로 lvalue 인수의 경우 by-value다음의 이동 대입은 참조 전달 다음의 복사 대입보다 훨씬 비쌀 가능성이 있다.
// 이는 함수가 매개변수를 복사하는 방식이 두 가지이기 때문이다.
// 하나는 생성을 통한 복사이고 또 하나는 대입을 통한 복사이다.
// 생성을 통한 복사는 위에서 분석한 그대로이다.

// 하지만 대입을 통한 복사에서는 추가적으로 고려해야할 사항이 있다.
// std::string이나 std::vector처럼 동적 메모리를 할당하거나 해제하는 비용이 값 전달 비용에 포함되는 경우에는
// 값 전달이 참조 전달과 거의 비슷한 빠르기로 실행될 수 있다.

// 참조 전달과는 달리 값 전달에서는 잘림 문제가 발생할 여지가 있다.
// 잘림 문제는 파생 클래스 타입의 객체가 base 클래스 타입의 객체에 대입될 때 파생 클래스 부분이 잘려나가는 문제이다.

class Widget { ... };

class SpecialWidget: public Widget{ ... };

void processWidget(Widget w);

SpecialWidget sw;

processWidget(sw);											// processWidget은 이를 SpecialWidget이 아니라 Widget으로 인식한다.