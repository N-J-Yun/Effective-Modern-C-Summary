// 람다의 기본 capture모드에는 2가지 종류가 있다.
// 1. 참조 capture mode
// 참조 capture mode는 참조가 대상을 잃을 위험이 있다.
using FilterContainer = 
	std::vector<std::function<bool(int)>>;
FilterContainer filters;

filters.emplace_back(
	[](int value) { return value % 5 == 0; }
);
// filters 컨테이너에 5의 배수를 선별하는 필터 함수를 추가하였다.

// 하지만 나누는 수를 실행시점에 계산해야 할 수도 있다.
void addDivisorFilter(void)
{
	auto calc1 = computeSomeValue1();
	auto calc2 = computeSomeValue2();

	auto divisor = computeDivisor(calc1, calc2);

	filetrs.emplace_back(
		[&](int value) { return value % divisor == 0; }				// lambda의 []는 캡쳐리스트라고 불리며, 현재 scope의 지역변수를 넣어두어야 한다.
																	// [&]로 사용하면 lambda 표현식 내에서 사용된 지역변수를 추론하여 참조로 가져온다.
	);
}
// 위 람다에서는 divisor를 참조하는데 divisor는 addDivisorFilter가 return 하면 더 이상 존재하지 않게 된다.
// 따라서 filters는 미정의된 행동이 유발된다.
// 명시적으로 지정해도 똑같다.
filetrs.emplace_back(
	[&divisor](int value) { return value % divisor == 0; }	
// 하지만 명시적으로 지정해주는 것을 습관으로 한다면 직관적으로 오류를 미리 인지할 가능성이 높아진다.

// 2. 값 capture mode
// 값 capture는 포인터가 대상을 잃을 수 있으며, 람다가 자기 완결적이라는 오해를 부를 수 있다.
using FilterContainer = 
	std::vector<std::function<bool(int)>>;
FilterContainer filters;

filters.emplace_back(
	[=](int value) { return value % divisor == 0; }
);
// divisor값을 복사하여 capture함으로써 위의 dangling 문제를 해결하였다.
// 하지만 값 capture mode는 dangling 문제의 근본적인 해결책이 되지 못한다.
class Widget
{
public:
	...
	void addFilter(void) const;

private:
	int divisor;
};

void Widget::addFilter(void) const
{
	filters.emplace_back(
		[=](int value) { return value % divisor == 0; }
	);
}
// 언뜻 보기에는 문제가 없어보이지만 실은 그렇지 않다.
// capture는 람다가 생성된 범위 안에서만 보이는 non-static 지역변수에 대해서만 적용된다.
// 하지만 divisor는 Widget클래스의 데이터 멤버일 뿐이기 때문에 divisor는 capture될 수 없다.

// 위와 같은 현상이 일어나는 이유는 this 때문이다.
// 컴파일러는 위 코드를 다음처럼 취급한다.
void Widget::addFilter(void) const
{
	auto currentObjectPtr = this;

	filters.emplace_back(
		[currentObjectPtr](int value)
		{ return value % currentObjectPtr->divisor == 0; }
	);
}
// 람다가 closure안에 capture하는 것은 divisor가 아니라 Widget의 this 포인터이다.
// 따라서 this 포인터가 소멸되어 버린다면 미정의 동작을 유발한다.

void doSomeWork(void)
{
	auto pw =
		std::make_unique<Widget>();
	pw->addFilter();
}
// doSomeWork가 종료되면 Widget이 파괴되어 filters에는 대상을 잃은 포인터(this)가 존재한다.

// C++14에는 데이터 멤버를 capture하는 좋은 방법이 있다.
void Widget::addFilter(void) const
{
	filters.emplace_back(
		[divisor = divisor](int value)
		{ return value % divisor == 0; }
	);
}
// 값 capture mode의 또 다른 단점은 closure가 외부 데이터의 변화로부터 격리되어 있다는 오해를 부를 수 있다는 점이다.
void addDivisorFilter(void)
{
	static auto calc1 = computeSomeValue1();
	static auto calc2 = computeSomeValue2();

	static auto divisor = computeDivisor(calc1, calc2);

	filters.emplace_back(
		[=](int value)
		{ return value % divisor == 0; }
	);

	++divisor;
}
// 이 람다는 아무것도 capture하지 않는다.
// capture list는 non-static 지역변수만 담을 수 있기 때문이다.
// 위 람다에서 사용되는 divisor는 값 capture로 받은 지역변수가 아니고,
// static 변수 divisor를 참조로 capture한 것과 똑같은 결과를 유발한다.