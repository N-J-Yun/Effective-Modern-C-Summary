// C++14부터는 make_unique함수를 추가로 지원한다. 
// 따라서 make 함수 종류는 3개가 되었다.(make_shared, make_unique, allocate_shared)

// 그리고 make함수는 new를 사용하여 스마트 포인터를 생성하는 것보다 좋다.

// 1. 스마트 포인터를 생성할 때 new를 사용하면 중복 코드가 생기지만, make 함수를 사용하면 그런 일은 없어진다.
auto upw1(std::make_unique<Widget>());							// make 함수를 사용한 경우
std::unique_ptr<Widget> upw2(new Widget);						// new를 사용한 경우

// 2. make 함수를 사용하면 예외 안전성이 더 향상된다.
void processWidget(std::shared_ptr<Widget> spw, int priority);	// 우선순위에 따라 Widget을 처리하는 함수
int computePriority();											// 우선 순위를 계산 하는 함수

processWidget(std::shared_ptr<Widget>(new Widget)				// 사용 예시
				, computePriority());
// 하지만 이렇게 사용하게 되면 메모리 누수가 발생할 수 있다.
// 스마트 포인터를 쓰는데 왜 누수가 발생할 수 있을까?
// 그 이유는 C++의 인자 평가를 보면 알 수 있다.

// C++은 processWidget의 함수 인자들을 3파트로 나눈다.
// 1. new Widget 2. std::shared_ptr<Widget>생성자 호출 3. computePriority()함수 호출
// 하지만 C++은 3파트를 위 순서대로 평가하지 않는다.(어떤 순서로 평가할 지는 아무도 모른다.)
// 만약 1,3,2 순으로 평가를 하는데 3에서 예외가 발생한다면?
// 이는 곧바로 메모리 누수로 이어질 수 있다.

// 하지만 make 함수를 사용하면 이런 일을 방지할 수 있다.
processWidget(std::make_shared<Widget>(), computePriority());
// make_shared함수 호출 내에서 스마트 포인터를 생성해내기 때문에 computePriority()함수의 예외 발생여부와 관계없이 예외 안전성을 지켜낸다.

// 3. make 함수를 사용하면 효율성 측면에서도 더 좋다.
std::shared_ptr<Widget> spw(new Widget);
// new를 사용하여 shared_ptr을 생성할 때를 보면,
// 메모리 할당이 두 번 일어남을 알 수 있다.(new로 Widget을 한 번 생성, shared_ptr의 생성자에서 control block을 한 번 생성)

auto spw = std::make_shared<Widget>();
// 하지만 make 함수는 Widget과 control block의 메모리를 동시에 할당한다.
// 따라서 프로그램의 정적인 크기를 줄일 수 있고(메모리 할당 코드가 한 번만 있으므로),
// control block에 내부 관리용 정보를 포함할 필요가 없어져서 프로그램의 전체적인 메모리 사용량이 줄어들 여지가 생긴다.

// 위 세 장점들은 allocate_shared에서도 비슷하게 적용된다!

// 하지만 make 함수를 사용할 수 없는 상황도 존재한다.
// custom deleter를 지정할 때
auto widgetDeleter = [](Widget* pw) { ... };

std::unique_ptr<Widget, decltype(widgetDeleter)>
	upw(new Widget, widgetDeleter);
std::shared_ptr<Widget> spw(new Widget, widgetDeleter);
// make 함수로는 이렇게 사용할 수 없다.

// make 함수들은 내부적으로 매개변수들을 완벽 전달할 때 중괄호가 아니라 괄호를 사용한다. 
// 이 때문에, 피지칭 객체를 중괄호 초기치로 생성하려면 반드시 new를 직접 사용해야 한다.
// 하지만 auto 타입 추론을 이용해 우회할 수도 있다.
auto initList = { 10, 20 };
auto spv = std::make_shared<std::vector<int>>(initList);

// 그리고 만약 make 함수를 사용할 수 없지만 최적화는 하고 싶을 경우에는 이러한 해결법이 있다.
// 메모리 누수 가능성이 있는 기존 코드 + custom deleter를 지정하여 make 함수를 사용할 수 없는 경우
void processWidget(std::shared_ptr<Widget> spw, int priority);
processWidget(std::shared_ptr<Widget>(new Widget, cusDel)	
				, computePriority());

// 1차 해결법 - shared_ptr로 new 부분을 묶어준다.
std::shared_ptr<Widget> spw(new Widget, cusDel);
processWidget(spw, computePriority());
// 하지만 이러한 전달은 lvalue로 전달하므로 값을 복사하게 된다.
// make_shared함수를 사용할 때는 rvalue로 전달하기 때문에 비용이 덜 든다.

// 2차 해결법 - std::move 함수 사용
processWidget(std::move(spw), computePriority());
// 효율적이며 예외에 안전하다!

// 하지만 이러한 경우는 특수한 상황이므로 make 함수를 쓸 수 있다면 대부분 쓰는 것이 좋다.
