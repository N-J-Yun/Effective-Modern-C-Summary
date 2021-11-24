std::multiset<std::string> names;								// global 자료구조

void logAndAdd(const std::string& name)
{
	auto now = std::chrono::system_clock::now();

	log(now, "logAndAdd" );
	names.emplace(name);
}

int main()
{
	std::string petName("Darla");

	logAndAdd(petName);											// lvalue를 인자로 넘겼기 때문에 필연적으로 복사가 발생한다. 개선 여지 X
	logAndAdd(std::string("Persephone"));						// rvalue를 넘겨주기 때문에 복사대신 이동을 할 수 있으나 현재 연산에서는 복사하였다.
	logAndAdd("Patty Dog");										// rvalue를 임시로 생성하여 emplace에 전달한다.
																// 임시 객체를 생성하지 않고 다이렉트로 자료구조에 넘겨준다면 더 효율적일 것이다.
	return 0;
}

// 최적화 적용
template<typename T>
void logAndAdd(T&& name)										// universal reference로 값을 받아 다이렉트로 전달한다.
{
	auto now = std::chrono::system_clock::now();
	log(now, "logAndAdd");
	names.emplace(std::forward<T>(name));						// forward로 다이렉트 전달
}

int main()
{
	std::string petName("Darla");
	logAndAdd(petName);						
	logAndAdd(std::string("Persephone"));						// rvalue를 이동한다.
	logAndAdd("Patty Dog");										// multiset안에 다이렉트로 std::string을 생성한다.
											
	return 0;
}

// 하지만 이렇게 만든 버전에 기능을 추가하기 위해 int 버전의 오버로딩을 한다고 하자.
void logAndAdd(int idx)
{
    auto now = std::chrono::system_clock::now();
    log(now, "logAndAdd");
    names.emplace(nameFromIdx(idx));
}

// 하지만 최적화를 위해 int 대신 short를 사용하여 logAndAdd를 호출하면 에러가 발생한다
short nameIdx;
logAndAdd(nameIdx);												// error!
// 그 이유는 template버전의 logAndAdd는 short타입을 정의할 수 있고, int보다 더 적합한 버전으로 추론되기 때문이다.
// 하지만 int 버전과 달리 template 버전은 정수 타입을 처리할 수 있는 기능이 구현되어 있지 않기 때문에 구현 내부에서 에러가 발생하게 된다!

// 위 같은 이유로 universal reference와 오버로딩을 결합하는 것은 거의 항상 좋지 않은 선택이다.

// 더 끔찍한 경우도 있다.
class Person
{
public:
	tempalte<typename T>
	explicit Person(T&& n)
	: name(std::forward<T>(n))									// perfect forwarding
	{}

	explicit Person(int idx)
	: name(nameFromIdx(idx)) 
	{}

	Person(const Person& rhs);									// 컴파일러가 자동 생성한 복사 생성자
	Person(Person&& rhs);										// 컴파일러가 자동 생성한 이동 생성자

private:
	std::string name;
};

int main()
{

	Person p("Nancy");										

	auto cloneOfP(p);											// 이 코드의 의도는 복사 생성자를 통해 새로운 person 객체를 만드는 것이지만
																// 실제로는 perfect forwarding 버전의 생성자가 호출된다.

	const Person cp("Nancy");
	auto cloneOfP(cp);											// 억지로 const를 붙혀 최적의 타입을 넘겨준다면 복사생성자를 호출하긴 한다.
																// 하지만 이런 경우는 특수한 경우일 뿐이다.
																// 일반 함수버전과 템플릿 버전 함수의 인자가 모두 정확히 부합한다면
																// 일반 버전을 호출한다는 규칙 덕분에 의도에 맞게 복사생성자가 호출된 것 뿐이다.

	return 0;
}
// 애초에 이와같은 상황은 끼워맞추기를 하지 않으면 발생하지 않으며,
// 가장 좋은 방법은 universal reference와 오버로딩을 하지 않는 것이다.
