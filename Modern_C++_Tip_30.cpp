class Widget
{
public:
	Widget(Widget&& rhs)										// 이동 생성자, rvalue reference
	: name(std::move(rhs.name))									// rvalue 객체를 마지막으로 사용할 때는 std::move를 적용한다.
	, p(std::move(rhs.p))
	{...}

	template<typename T>
	void setName(T&& newName)									// setter 템플릿 함수, universal reference
	{ name = std::forward<T>(newName); }						// universal reference를 사용할 때는 std::forward를 적용한다.

private:
	std::string name;
	std::shared_ptr<SomeDataStructure> p;

};
// rvalue - std::move, universal - std::forward
// 그 반대로 사용하는 것은 좋지 않다.

// 주의해야할 것은 rvalue나 universal reference에 묶인 객체를 여러번 사용할 때는 사용하기 전에 다른 객체로 이동하는 일은 피해야 한다.
// std::move나 std::forward를 적용하는 것은 해당 참조의 마지막 사용이어야 한다.
template<typename T>
void setSignText(T&& text)
{
	sign.setText(text);											// 이동 전에 사용(내용 수정을 절대 하면 안됨)

	auto now = std::chrono::system_clock::now();

	signHistory.add(now, std::forward<T>(text));				// 조건부 rvalue 캐스팅
}

// 하지만 RVO(return value optimization)가 적용될 수 있는 지역 변수에 대해서는 절대 std::move나 std::forward를 적용해서는 안된다.
// RVO가 적용되기 위한 조건
// 1. 지역 객체의 타입이 함수의 반환 타입과 같아야 한다.
// 2. 그 지역 객체가 함수의 반환 값이어야 한다.(함수 매개변수는 X, return문 내부의 임시객체는 O)
Widget makeWidget(void)
{
	Widget w;
	...
	return w;
}
// 이 경우 컴파일러는 w의 복사 제거를 수행하거나
// 아니면 함수를 다음과 같이 작성된 것으로 취급한다.
Widget makeWidget(void)
{
	Widget w;
	...
	return std::move(w);										// 복사 제거를 수행하지 않을 때는 w를 rvalue로 취급
}

// 이는 컴파일러가 자동으로 적용해주는 것으로 만약 최적화의 대상이 될 수 있는 지역 객체에 std::move나 std::forward를 적용하여 반환하면
// 2번의 조건을 불만족하여 최적화가 적용되지 않는다(참조의 반환이기 때문)
// 굳이 직접 rvalue로 명시적으로 만들지 않아도 컴파일러가 자동으로 해주며
// 오히려 2번의 조건마저 불만족시켜 반환값 최적화를 무효화해 버릴 수도 있다.