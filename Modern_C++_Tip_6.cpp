template<typename Container, typename Index>
decltype(auto)										// C++14부터 사용할 수 있다.
													// 타입추론은 auto처럼 초기값에 대해 진행하지만 decltype의 규칙을 적용한다.
authAndAccess(Container& c, Index i)
{
	authenticateUser();
	return c[i];										// return 타입은 c[i]와 일치한다.
}
// 하지만 아직 이 함수는 다듬을 필요가 있다.
// 위 함수에서 Container c는 lvalue에 대한 참조로써 함수에 전달된다.
// 이는 반환값을 통해 값을 수정하기 위함인데, 이렇게 되면 rvalue에 대한 참조 컨테이너는 전달 받을 수 없다는 문제가 생긴다.

std::deque<std::string> makeStringDeque();				// factory 함수

auto s = authAndAccess(makeStringDeque(), 5);			// 임시객체를 생성하여 authAndAccess에 전달하기 때문에
														// rvalue에 대한 참조가 전달 되었다.
														// 위 함수로는 이러한 사용을 지원할 수 없다.

// 따라서 함수를 수정할 필요가 있다.
// 항목 25에 맞춰 구현도 바꿔줄 필요가 있다.
template<typename Container, typename Index>
decltype(auto) authAndAccess(Container&& c, Index i)	// c는 이제 보편참조이다.(universal reference)
{
	authenticateUser();
	return std::forward<Container>(c)[i];				
}

// 이는 C++14부터 사용가능한 방식이다.