std::multiset<std::string> names;									// 글로벌 데이터 구조체

template<typename T>
void logAndAdd(T&& name)											// 이 함수는 모든 타입을 다 받는 universal reference이며 오버로딩 하지 않는다.
{
	logAndAddImpl(													// int타입의 인자를 받는 경우와, 그 외의 경우를 나눠서 오버로딩 버전을 구현한다.
		std::forward<T>(name),
		std::remove_reference_t<T>									// C++14, 이 두번째 인자로 어떤 logAndAddImpl 오버로딩 버전을 사용할지 결정
	);
}

templaet<typename T>												// 비 정수 타입 오버로딩 버전 
void logAndAddImpl(T&& name, std::false_type)						// false_type, true_type은 컴파일타임 타입이다.
																	// 이 함수는 컴파일타임에 선택되어야 하는데
																	// false, true는 런타임 타입이므로 사용할 수 없다.
{
	auto now = std::chrono::system_clock::now();
	log(now, "logAndAdd");
	names.emplace(std::forward<T>(name));
}

std::string nameFromIdx(int idx);									
	
void logAndAddImpl(int idx, std::true_type)							// 정수 타입 오버로딩 버전 					
{
	logAndAdd(nameFromIdx(idx));									// 입력받은 인덱스의 name을 받아와 다시 logAndAdd로 넘긴다.
}
// 이 방식을 Tag Dispatch라고 한다.
// Tag Dispatch는 universal reference의 오버로딩을 대체할 수 있는 기법이다.

// 하지만 생성자같은 경우에는 자동생성된 복사생성자가 문제가 될 수 있다.
// 결국 한계가 있는 기법이다.
