// shared_ptr은 unique_ptr처럼 custom deleter를 지정해줄 수 있다.
// 하지만 unique_ptr과는 다르게 deleter를 shared_ptr타입의 요소로서 받아들이지는 않는다.
auto loggingDel = [](Widget* pw)
					{
						makeLogEntry(pw);
						delete pw;
					};
std::unique_ptr<Widget, decltype(loggingDel)>
	upw(new Widget, loggingDel);

std::shared_ptr<Widget>
	spw(new Widget, loggingDel);
// 위처럼 unique_ptr은 타입 지정자 내에 함수 객체를 선언해두었지만, 
// shared_ptr은 인자로써만 받고 타입 지정자 내에는 선언하지 않아도 된다.

// 그래서 shared_ptr은 deleter지정으로 더 유연하게 설계할 수 있다.
auto customDeleter1 = [](Widget *pw) { ... };
auto customDeleter2 = [](Widget *pw) { ... };

std::shared_ptr<Widget> pw1(new Widget, customDeleter1);
std::shared_ptr<Widget> pw2(new Widget, customDeleter2);

std::vector<std::shared_ptr<Widget>> vpw{ pw1, pw2 };
// pw1과 pw2는 deleter 지정자가 다르지만 서로 같은 타입(shared_ptr<Widget>)으로 취급되므로
// 컨테이너 내에 동시에 넣어두고 사용할 수 있고
// 서로 대입이 가능하며, shared_ptr<Widget>을 인자로 호출하는 함수에 둘 다 인자로 들어갈 수 있다.

// 그리고 unique_ptr은 상태 있는(capture list가 비어있지 않은) 함수 객체를 deleter로 지정하면
// unique_ptr 사용에 추가적인 메모리 비용이 발생하는데, shared_ptr은 조금 다르다.
// 물론 shared_ptr도 메모리 비용이 발생하지만, 그 shared_ptr타입 자체의 크기가 증가하지는 않는다.
// unique_ptr은 1word -> 2word로 자체의 크기가 증가하지만, shared_ptr은 2word 크기에서 변하지 않는다.
// 그 이유는 shared_ptr은 1word를 가리키는 객체의 포인터를 저장하는데 사용하고,
// 나머지 1word를 더 큰 자료구조인 control block의 포인터를 저장하는데 사용한다.
// control block은 reference count, weak count, custom deleter, allocator 등등을 포함하고 있는 자료구조이다.
// shared_ptr은 control block의 pointer를 가짐으로써 deleter의 크기와 관계없이 고정된 크기(2word)를 가질 수 있다.

// 하지만 control block의 생성 여부때문에 지켜야할 규칙이 있다.
// shared_ptr생성자에 raw pointer를 전달하여 shared_ptr을 생성하는 일은 피해야 한다 는 것이다.
auto pw = new Widget;

std::shared_ptr<Widget> spw1(pw, loggingDel);
std::shared_ptr<Widget> spw2(pw, loggingDel);
// spw1와 spw2는 서로 다른 control block을 가지고 있지만, 둘 다 pw를 가리키는 shared_ptr이 되어버린다.
// 이렇게 되면 둘 중 하나가 소멸자를 호출하게 되면 다른 하나는 미정의 행동을 일으키게 된다.
// 일반적인 해결법은 make_shared를 사용하는 것이지만, 우리는 custom deleter를 지정했기 때문에 make_shared는 사용할 수 없다.
// 따라서 만약 위 상황처럼 반드시 raw pointer를 shared_ptr 생성자에 전달해야 한다면,
// 직접적으로 전달해 주자.
std::shared_ptr<Widget> spw1(new Widget, loggingDel);			// 이렇게!

// 그리고 spw2는 spw1를 인자로 받아 생성해주자(shared_ptr의 복사 생성자를 이용하자)
std::shared_ptr<Widget> spw2(spw1);								// 이렇게!

// 하지만 shared_ptr은 unique_ptr과 다르게 배열 타입을 관리할 수는 없다.
// 따라서 첨자 연산자(operator[])도 api로 지원하지 않는다.
// 억지로 쓰겠다고 shared_ptr<T>을 배열처럼 쓰지말자.
// 첨자 연산자가 지원되지 않기 때문에 접근도 매우 이상하게 해야될 것이다.
// 대신 std::array, std::vector, std::string같은 표준을 사용하자!

