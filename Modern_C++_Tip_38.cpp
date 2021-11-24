// closure: 람다에 의해 만들어진 실행시점 객체
{
	int x; 
	…
	auto c1 = 
		[x](int y) { return x * y > 55; }; 
	auto c2 = c1; 
	auto c3 = c2;
}
// c1, c2 c3는 람다에서 생성된 closure의 복사본

// capture list:
// lambda의 []는 캡쳐리스트라고 불리며, 람다에서 사용되는 현재 scope의 지역변수를 넣어두어야 한다.
// [&]로 사용하면 lambda 표현식 내에서 사용된 지역변수를 추론하여 참조로 가져온다.
// [=]로 사용하면 lambda 표현식 내에서 사용된 지역변수를 추론하여 값으로 가져온다.

// init capture: C++14부터 지원하는 capture 형식
int asdf = 4;
auto c1 = 
	[x = asdf](int y) { return x * y > 55; }; 
// x는 람다 식 내에서 사용되는 데이터 멤버의 이름
// asdf는 x를 초기화할 식

// closure class: closure를 만드는 데 쓰인 클래스를 말한다.
// 기본적으로 lambda expression은 함수 객체(operator() 오버로딩을 위한 클래스 객체)이다.
// lambda 표현식도 operator()가 오버로딩된 클래스 형태를 가지고 있다. 그것이 closure class이다.

auto f = [](int x){ return func(normalize(x)); };
// 위 람다 식은

class Widget {
public:
	int operator()(int x) const 
	{ return func(normalize(x)); }
};
// 이러한 형태의 closure class를 가지고 있다.

