// std::shared_ptr처럼 동작하되, 대상을 잃을 수도 있는 포인터가 필요하면 weak_ptr을 사용하자.
// 일반적으로 weak_ptr은 shared_ptr에서 생성한다.
// weak_ptr은 자신을 생성하는데 쓰인 shared_ptr과 같은 객체를 가리키지만, 해당 객체의 참조 카운트를 건드리지 않는다.
auto spw = std::make_shared<Widget>();					// shared_ptr 생성
std::weak_ptr<Widget> wpw(spw);							// shared_ptr에서 weak_ptr 생성
spw = nullptr;											// shared_ptr의 참조 카운트가 0이 되었다. 가리키던 객체 소멸

if(wpw.expired())...									// wpw이 가리키는 객체가 만료되었는지 체크할 수 있다.

// weak_ptr이 가리키던 객체를 역참조하고 싶다면 2가지 방법을 쓸 수 있다.
std::shared_ptr<Widget> spw1 = wpw.lock();				// 1. weak_ptr의 lock 멤버 함수를 사용(shared_ptr을 반환), 만료되었으면 null
auto spw2 = wpw.lock();									// 위와 같은 방법이지만 auto를 썼다.

std::shared_ptr<Widget> spw3(wpw);						// 2. shared_ptr의 생성자에 weak_ptr을 넘겨준다, 만료되었으면 예외 발생

// weak_ptr의 주 사용처 2가지
// 1. factory function의 캐시
std::unique_ptr<const Widget> loadWidget(WidgetId id);	// 일반적인 factory 함수
// 하지만 만약 loadWidget함수가 비용이 매우 크고 ID들이 반복해서 사용되는 경우가 많다고 하자.
// 그렇다면 호출결과를 캐싱하는 함수를 만들어 최적화를 할 수 있을 것이다.
// 그러나 요청한 Widget들을 캐시에 모두 저장해두면 성능상의 문제가 발생하므로 이를 캐시에서 지우는게 자연스럽다.
// 하지만 caller가 캐싱되는 객체를 가리키는 스마트 포인터를 받는 것은 확실하며, 그 객체들의 수명을 caller가 결정해야 한다는 것도 명확하다.
// 그렇기 때문에 캐시에 있는 포인터들은 자신들이 가리키는 객체가 사라졌는지(만료) 검사할 수 있어야 한다.
// 따라서 캐시의 포인터는 weak_ptr이어야 한다.

std::shared_ptr<const Widget> fastLoadWidget(WidgetId id)
{
	static std::unordered_map<WidgetId, std::weak_ptr<const Widget>> cache;

	auto objPtr = cache[id].lock();						// objPtr은 캐시의 해당 weak_ptr가 가리키는 shared_ptr, 대상이 없다면 null

	if(!objPtr)											// 캐시에 저장되어 있는 경우
	{
		objPtr = loadWidget(id);
		cache[id] = objPtr;
	}
	return objPtr;
}

// 2. observer 패턴
// observer 패턴은 subject(상태가 변하는 객체)와, observer(subject의 상태 변화를 관찰하는 객체)로 구성된다.
// subject들은 observer의 포인터를 멤버로 들고 있어 각 observer들에게 자신의 상태를 손쉽게 알려줄 수 있다.
// 하지만 subject들은 이미 소멸된 observer에게 자신의 상태를 알리는 것을 피할 필요가 있다.
// 이럴 때 subject들은 observer들을 가리키는 weak_ptr을 요소로 하는 컨테이너를 멤버 변수로 하여 observer들의 만료 검사를 손쉽게 할 수 있다.

// 성능 측면에서 weak_ptr은 shared_ptr와 동일하다.
// 같은 control block을 가리키며 atomic reference를 필요로 하는 것도 같다.
// 대신 weak_ptr은 비지칭 대상의 reference count를 건드리는 것이 아니라 control block의 weak count를 건드린다.