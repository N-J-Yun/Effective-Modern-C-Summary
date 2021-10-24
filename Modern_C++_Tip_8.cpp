template<typename T>
void f(const T& param)
{
	using std::cout;
	cout << "T = " << typeid(T).name() << '\n';						// T 		= class Widget const *
	cout << "param = " << typeid(param).name() << '\n';				// param 	= class Widget const *
}

std::vector<Widget> createVec();

int main()
{
	const auto vw = createVec();

	if(!vw.empty())
	{
		f(&vw[0]);
	}

	return 0;
}
// 
// 위 같은 결과가 나오는 이유는 std::type_info::name은 템플릿 함수에 값 매개변수가 전달된 것처럼 주어진 타입을 추론하기 때문이다.
// 그래서 실제로는 class Widget const* const & 타입인 param이 &가 무시되고 const 포인터가 무시되어 class Widget const *로 나오게 된다.

// 위 같은 복잡한 타입에 대해서 IDE는 타입 추론이 알아보기 힘들게 나온다.

// boost라이브러리의 type_index를 사용하면 깔끔하게 인간 친화적으로 나온다.
#include <boost/type_index.hpp>
template<typename T>
void f(const T& param)
{
	using boost::typeindex::type_id_with_cvr;
	std::cout << "T = " << 
		type_id_with_cvr<T>().pretty_name() << '\n';				// T 		= class Widget const *
	std::cout << "param = " << 
		type_id_with_cvr<decltype(param)>().pretty_name() << '\n';	// param 	= class Widget const * const &
}
// boost::typeindex::type_id_with_cvr는 const, &, volatile을 무시하지 않고 타입추론을 진행하기 때문에
// 우리가 원하던 타입을 얻을 수 있다.
// 하지만 일부 타입에 대해서는 부정확할 수 있으므로 결국 타입 추론에 대해서 완벽히 알아두는 것이 가장 좋은 방법이다.