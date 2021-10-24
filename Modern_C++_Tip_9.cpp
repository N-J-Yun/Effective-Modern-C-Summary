int main()
{
	int x;									// 문맥에 따라 초기화되지 않을 수도 있다.
	auto x;									// error, auto는 초기화를 반드시 해줘야 한다.
	auto x = 0;								// ok

	return 0;
}

template<typename It>
void dwim(It b, It e)
{
	while(b != e)
	{
		// typename std::iterator_traits<It>::value_type
		// 	currValue = *b;
		auto currValue = *b;				// 같은 내용을 auto를 쓰면 타자량을 줄일 수 있다.
	}
}

auto derefUPLess =
	[](const std::unique_ptr<Widget>& p1,
		const std::unique_ptr<Widget>& p2)
		{ return *p1 < *p2; };				// C++11부터 사용할 수 있는 람다표현식에서의 auto 사용

auto derefUPLess =
	[](const auto& p1,
		const auto& p2)
		{ return *p1 < *p2; };				// C++14부터는 람다 매개변수에도 auto를 사용할 수 있다.


// std::function 객체를 사용하면 어떨까?
std::function<bool(std::unique_ptr<Widget>&,
					std::unique_ptr<Widget>&)>
	derefUPLess = [](const std::unique_ptr<Widget>& p1,
					 const std::unique_ptr<Widget>& p2)
		{ return *p1 < *p2; };				// 보기에도, 타이핑 하기에도 훨씬 장황하다.
											// 하지만 그것 이외에도 더 큰 문제가 있다.
											// std::function으로 선언된 변수(derefUPLess)의 타입은 std::function 템플릿의 한 인스턴스이며,
											// 그 크기는 임의의 주어진 시그니처에 대해 고정되어 있다.
											// 그런데 그 크기가 요구된 클로저를 저장하기에 부족하다면 힙 메모리를 할당해서 클로저를 저장한다.

											// 반면 auto로 선언된 클로저를 담는 변수는 그 클로저와 같은 타입이며, 그 클로저에 요구된 메모리만 사용한다.


std::unordered_map<std::string, int> m;

for(const std::pair<std::string, int>& p : m)
{
	...
}
// 문제가 없어보이는 코드이지만 큰 문제가 있다.
// unordered_map의 key요소는 const 이기 때문에 for문에서 사용된 pair의 std::string은 const std::string이 되었어야 했다.
// 이런 타입 mismatch 때문에 compiler는 훨씬 더 많은 일을 해야한다.
// 타입을 변환시키기 위해 p를 묶고자 하는 타입의 임시 객체를 생성하고, m의 각 객체를 복사하고, 참조 p를 그 임시 객체에 묶는다.
// 그리고 loop 턴이 끝나면 그 임시객체들을 삭제한다.
// 임시객체가 생성되고 복사가 일어나므로 효율이 떨어지는 것은 물론이고, p의 주소를 구하면 임시 객체의 주소를 얻게 된다.

for(const auto& p : m)
{
	...
}
// 하지만 auto를 사용하면 이런 실수를 없앨 수 있다.

// auto의 장점
// 1. 초기화되지 않은 변수를 피할 수 있다.
// 2. 장황한 타입을 타이핑하는 것을 피할 수 있다.
// 3. closure를 직접적으로 묶어 효율이 좋다.
// 4. 타입 mismatch를 피할 수 있다.

// auto는 의무가 아닌 선택이지만, auto를 사용함으로써 발생할 수 있는 인지적 문제들은 IDE나 변수 이름 등으로 완화할 수 있다.
// 또한 코드 수정에 대한 번거로움도 auto를 통해 해결하여 리팩토링에도 효율적으로 작동한다.

