template<typename T>
typename remove_reference<T>::type&&							// 반환타입은 입력한 type의 rvalue참조이다.
move(T&& param)
{
	using ReturnType = 
		typename remove_reference<T>::type&&;

	return static_cast<ReturnType>(param);
}

template<typename T>											// C++14에서의 move 구현(개념적으로 구현한 것)
decltype(auto) move(T&& param)
{
	using ReturnType = remove_reference_t<T>&&;
	return static_cast<ReturnType>(param);
}
// std::move는 그 이름처럼 인자를 이동하는 기능을 수행하는 것이 아니라
// 인자로 받은 것을 rvalue로 변환하는 기능을 수행한다.
// 그리고 rvalue는 그저 이동을 위한 자격일 뿐이며,
// std::move는 그 자격을 부여해준다는 의미에서 move라는 이름을 붙히게 되었다.(자격을 보장해주는 것이 아니다.)

// 주석을 구현하기 위한 클래스를 구현한다고 해보자
class Annotation
{
public:
	explicit Annotation(const std::string text)					// string을 const로 입력 받아서 
		: value(std::move(text))								// value에 이동하려 한다
		{...}

private:
	std::string value;
};
// 위 코드의 의도는 끔찍하다. 하지만 위 코드는 컴파일된다.
// 하지만 text는 의도처럼 이동하지 않고 복사된다.

// string 클래스의 개념적인 구현
class string 
{
public:
	string(const string& rhs);									// copy
	string(string&& rhs);										// move
};
// Annotation의 생성자는 string을 const로 받아서 이동하려 한다.
// 하지만 string의 이동 생성자는 const인자를 받지 못한다.
// 따라서 Annotation의 생성자는 복사 생성자를 호출하게 된다.

// 위 예시를 통해 인지해야 하는 점
// 1. 이동을 지원할 객체는 const로 선언해서는 안된다. const객체에 대한 이동 요청은 복사 연산으로 대체되어 버린다.
// 2. std::move는 실제로는 아무것도 이동시키지 않는다. 이동을 위한 자격을 보장해주는 것도 아니다. 그저 rvalue로 바꿔줄 뿐이다.
