// decltype은 들어온 값의 타입을 아무 수정 없이 내보낸다.
// 그래서 컨테이너의 operator[]같이 반환하는 값이 해당 매개변수의 값에 의존하는 경우에 많이 쓰인다.
template<typename Container, typename Index>	
auto authAndAccess(Container& c, Index i)		// auto는 후행 반환 타입임을 알려주는 용도일 뿐이지 타입 추론과는 아무런 관련이 없다.
	->decltype(c[i])							// 후행 반환 타입(trailing return type)
												// decltype은 c[i]의 타입을 추론한다.
{
	authenticateUser();
	return c[i];								
}

// 만약 decltype과 후행 반환 타입을 쓰지 않고 위 함수를 구현하면?
template<typename Container, typename Index>
auto authAndAccess(Container& c, Index i)	
{
	authenticateUser();
	return c[i];								// auto가 c[i]를 추론한다.
}
// 하지만 대개 이런 컨테이너의 [] 연산에서 반환하는 값은 참조값이다.
// 그리고 auto는 참조값을 무시하고 타입 추론을 진행하기 때문에 의도하던 타입이 나오지 않을 수 있다.
int main()
{
	std::deque<int> d;
	authAndAccess(d,5) = 10;					// &d[5]가 아닌 d[5]를 반환하여 컴파일 에러가 발생한다.

	return 0;
}

// decltype 사용 시 주의해야할 점
// decltype은 lvalue 표현식에 대해서 T가 아닌 T&를 반환한다.
decltype(auto) f1(void)
{
    int x = 0;

    return x;           						// decltype(x)는 int이므로 f1은 int를 반환
}
 
decltype(auto) f2(void)
{
    int x = 0;

    return (x);            						// decltype((x))는 int&이므로 f2는 int&를 반환
}
// 이는 decltype(auto)에도 적용되며 
// 지역변수에 대한 표현식을 반환할 때도 적용되기 때문에 매우 주의해야한다.