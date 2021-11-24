auto f = [](auto x){ return normalize(x); };
// 람다의 매개변수란에 auto를 사용하였다.

class Widget
{
public:
	template<typename T>
	auto operator()(T x) const
	{ return normalize(x); }
	...
};
// 위 람다의 closure class는 위와 같은 형태가 된다.

// 만약 normalize가 lvalue와 rvalue를 다르게 처리한다면?
// 람다는 x를 normalize에 perfect forwarding할 필요가 있다.

auto f = [](auto&& x)
			{ return normalize(std::forward<???>(x)); };
// ???에 어떤 타입 인수를 넘겨야할까?
// 이 같은 경우에는 decltype을 사용하여 의도대로 구현할 수 있다.
// 만약 lvalue가 전달되었다면 decltype(x)는 lvalue참조에 해당하는 타입을 산출하고,
// rvalue가 전달되었다면 rvalue참조 타입을 산출한다.

auto f = [](auto&& x)
			{ return normalize(std::forward<decltype(x)>(x)); };

// C++14에서는 가변 인자를 지원하여 더 완벽한 람다를 구현할 수 있다.
auto f = [](auto&&... xs)
			{ return normalize(std::forward<decltype(xs)>(xs))... };