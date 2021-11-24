class Widget
{
public:
	...
	bool isValidated(void) const;
	bool isProcessed(void) const;
	bool isArchieved(void) const;
private:
	...
};

auto pw = std::make_unique<Widget>();

auto func = [pw = std::move(pw)]
			{ return pw->isValidated()
						&& pw->isArchieved(); };

// capture list내에서 = 왼쪽은 lambda 내에서 사용되는 데이터 멤버의 이름이며, = 우측은 그것을 초기화하는 식이다.
// 만약 pw를 람다 안으로 이동하기 전에 조작할 필요가 없다면 closure 클래스의 데이터 멤버를 std::make_unique로 직접 초기화하면 된다.
auto func = [pw = std::make_unique<Widget>()]
			{ return pw->isValidated()
						&& pw->isArchieved(); };
// 이 방식은 init capture라고 불리며, C++14부터만 가능한 기법이다.
// 객체를 move하는 게 더 효율적이라면 init capture를 사용하자.