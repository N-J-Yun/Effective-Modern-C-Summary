class Widget
{
public:
	using DataType = std::vector<double>;
	DataType& data() { return values; }
private:
	DataType values;
};

Widget makeWidget();

int main()
{
	Widget w;

	auto vals1 = w.data();							// data()의 반환 타입은 lvalue 참조이기 때문에 w.values를 vals1에 복사한다.

	auto vals2 = makeWidget().data();				// makeWidget()은 팩토리 함수로써 임시 객체를 반환한다.
													// 하지만 data()의 반환 타입은 lvalue 참조이기 때문에 rvalue인 임시 객체와 매치되지 않는다.
													// 따라서 임시 객체의 std::vector를 복사하는 것은 시간낭비이다.
	return 0;
}

// 위 문제를 해결하기 위해서는 rvalue 참조 오버로딩이 필요하다.
class Widget
{
public:
	using DataType = std::vector<double>;

	DataType& data() &								// lvalue참조 반환 함수임을 명시했다.
		{ return values; }							// 따라서 lvalue 참조를 return 했다.

	DataType&& data() &&							// rvalue 참조 반환 함수임을 명시했다.
		{ return std::move(values); }				// 따라서 rvalue 참조를 return 했다.
};

int main()
{
	auto vals1 = w.data();							// lvalue 오버로딩된 함수를 호출, vals1은 복사 생성된다.

	auto vals2 = makeWidget().data();				// rvalue 오버로딩된 함수를 호출, vals2는 이동 생성된다.

	return 0;
}
// 멤버 함수를 정의할 때 참조 한정자를 붙힘으로써 다르게 오버로딩된 함수를 정의할 수 있다.