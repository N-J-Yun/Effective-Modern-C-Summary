// 이론적으로 emplace 함수들은 insertion 함수보다 항상 더 효율적이다.
std::vector<std::string> vs;
vs.push_back("xyzzy");
// 문자열 리터럴은 std::string과 타입이 다르므로 위 문장은 다음과 같이 해석된다.
vs.push_back(std::string("xyzzy"));
// 그 결과 임시 std::string 객체가 생성되고 std::vector의 push_back함수가 호출되며 std::string의 소멸자가 호출된다.
// 비용이 하고자하는 기능에 비해 너무 많이 든다.

// 이는 emplace_back 멤버 함수를 사용하여 더 효율적으로 만들 수 있다.
vs.emplace_back("xyzzy");

// emplace 함수들은 perfect forwarding을 이용하므로 임시 객체의 생성과 소멸 비용이 들지 않는다는 장점이 있다.
vs.emplace_back(50, 'x');
// perfect forwarding 때문에 std::string의 생성자에(50, 'x')를 전달하게 된다.

// 다음 세 조건이 모두 성립한다면 거의 항상 emplace의 성능이 insertion의 성능을 능가한다.
// 1. 추가할 값이 컨테이너에 대입되는 것이 아니라 컨테이너 안에서 생성된다.
// 2. 추가할 인수 타입이 컨테이너가 담는 타입과 다르다.
// 3. 컨테이너가 기존 값과의 중복 때문에 새 값을 거부할 우려가 별로 없다.

vs.emplace_back("xyzzy");									// 이 문장은 위 3조건을 모두 만족한다.
															// 비어있는 vector에 새로운 값이 생성되며(1)
															// 추가할 인수 타입이(char[6]) 컨테이너가 담는 타입(std::string)과 다르며(2)
															// 컨테이너가 기존 값과의 중복 때문에 새 값을 거부할 우려가 없다.(비어있기 때문에)(3)

// 하지만 emplace함수를 사용할지에 대해 고민할 때 고려해야할 2가지 상황이 있다.
// 1. 자원 관리 객체를 컨테이너에 추가한다면, 그리고 자원 획득과 그 자원을 자원 관리 객체로 변환하는 시점 사이에 아무것도 끼어들지 못하게 하라는 조언을
// 제대로 따른다면, emplace가 insertion보다 더 나은 성능을 보일 가능성은 별로 없다.

std::list<std::shared_ptr<Widget>> ptrs;
void killWidget(Widget& pWidget);							// custom deleter

ptrs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));
ptrs.push_back({ new Widget, killWidget });					// 위와 동일한 문장
// push_back함수는 insertion이기 때문에 임시 std::shared_ptr 객체가 생성되었다가 파괴되는 일이 생긴다.
// 하지만 스마트포인터를 사용함으로써 예외가 발생해도 어떠한 누수도 일어나지 않는다는 안정성에 대한 장점이 있다.

// 하지만 emplace_back을 사용한다면?
ptrs.emplace_back(new Widget, killWidget);
// perfect forwarding을 사용하기 때문에 생성자로 전달하기 위해 shared_ptr이 아닌 생 포인터를 써야한다.
// 따라서 예외안전성이 보장되지 않고 자원이 새는 경우가 발생할 수 있다.

// 자원 관리 객체를 생성하는 작업에는 예외 안전성이 보장되어야 하므로 어떤 방식이든 임시 객체의 생성과 파괴 비용이 발생하게 된다.
// 따라서 emplace를 사용해도 insertion함수보다 더 나은 성능을 보여주지 못한다.

// 2. emplace 함수는 insertion함수라면 거부당했을 타입 변환들을 수행할 수 있다.
// 초기화 방식에는 복사 초기화와 직접 초기화 방식 2가지가 있다.
// explicit 생성자에서는 복사 초기화를 사용할 수 없지만 직접 초기화는 사용할 수 있다.
// std::regex는 const char*타입의 인수를 받고 explicit으로 선언된 생성자를 제공한다.(정규 표현식)

std::regex r1 = nullptr;									// error!, 복사 초기화

std::regex r2(nullptr);										// good, 직접 초기화

// emplace함수는 직접 초기화를 사용하기 때문에 explicit 생성자를 지원한다.
// 반면 insertion함수는 복사 생성자를 사용하므로 explicit 생성자를 지원하지 않는다.

std::vector<std::regex> regexes;

regexes.emplace_back(nullptr);								// good, perfect forwarding으로 regex의 생성자에 nullptr을 직접 넘겨준다.

regexes.push_back(nullptr);									// error, 복사 초기화에서는 explicit 생성자를 사용할 수 없다.

