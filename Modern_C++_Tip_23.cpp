// shared_ptr로 Widget 자원을 관리한다고 하자.
std::vector<std::shared_ptr<Widget>> processedWidgets;				// 이런 자료구조를 선언할 수 있을 것이다.

// 그리고 Widget 클래스 내에 Widget자원을 처리하는 함수가 있다고 하자.
class Widget
{
public:
	...
	void process();
	...
} ;

void Widget::process()
{
	...																// 처리하는 과정..

	processedWidgets.emplace_back(this);							// 처리가 끝난 this를 자료구조에 추가한다, 하지만 잘못 됐다.
}
// 위의 구문은 this라는 포인터를 가리키는 shared_ptr의 control block을 새로 선언한다.
// 하지만 만약 해당 Widget을 가리키는 shared_ptr이 멤버 함수 밖에 이미 선언되어 있었다면?
// 하나의 자원을 가리키는 포인터가 2개이며, control block도 2개인 상황이 되어버린다!

// 이런 상황을 해결할 수 있는 방법이 std::shared_ptr API에 정의되어 있다.
class Widget: public std::enable_shared_from_this<Widget>			// 이름이 매우 더럽지만 내용물은 확실하다(CRTP 패턴을 사용)
{
public:
	...
	void process();
	...
};

void Widget::process()
{
	...
	processedWidgets.emplace_back(shared_from_this());				// 현재 객체(this)의 control block을 살펴보고 그 control block을 참조하는
																	// 새로운 shared_ptr을 생성한다!
}
// std::enable_shared_from_this<T>는 클래스 템플릿이며, Curiously Recurring Template Pattern(CRTP)을 사용하고 있다.
// enable_shared_from_this를 상속받고 있는 클래스의 멤버 함수는 현재 객체(this)의 control block을 참조하는 새로운 shared_ptr을 만들 수 있다.
// 하지만 만약 멤버 함수 외부에 this를 참조하는 shared_ptr이 존재하지 않고 있다면 이는 미정의 행동을 일으킨다.

// 이를 방지하기 위해 생성자를 private으로 선언하고 팩토리 함수로만 객체를 생성할 수 있도록 client의 사용을 유도할 수 있다.
class Widget: public std::enable_shared_from_this<Widget>
{
public:
	template<typename... Ts>
	static std::shared_ptr<Widget> create(Ts&&... params);			// 팩토리 함수, 이 함수로만 Widget 객체 생성 가능.

	...
	void process();
	...
private:
	...																// 생성자를 private으로 선언
};
// shared_ptr의 control block이 생각보다 크기가 크고 하드웨어 단계에서 느리게 만들 수 있다는 생각이 들 수 있지만,
// 이 shared_ptr이 주는 기능을 생각하면 이는 충분히 가치있는 소비이다.
// 자동적인 메모리 관리(garbage collection), 자동 소멸(destructor)같은 핵심적인 기능들이 있기 때문이다.
// 그리고 일반적인 상황에서는 shared_ptr은 비용이 그렇게 들지 않는다.
// 비용이 생겨봤자 한두개의 machine instructions 밖에 비용이 들지 않는다.
// 따라서 자원관리에서는 raw pointer보다는 shared_ptr같은 스마트 포인터를 적극적으로 사용하자!