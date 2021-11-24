class Person
{
public:
	template<typename T,
			typename = std::enable_if_t<condition>::type>			// C++14, condition을 만족하는 템플릿을 활성화한다.(default가 비활성화)
			explicit Person(T&& n);
	...
};

// 위의 perfect forwarding 생성자와 복사 생성자가 충돌하지 않으려면
// perfect forwarding 생성자에 넘겨진 타입이 Person인 경우에는 템플릿을 비활성화 시켜야 한다.
// 두 타입이 같은지를 비교하는 std::is_same을 사용한다.

Person p("Nancy");
auto cloneOfP(p);

// 하지만 위 경우 auto는 Person&타입으로 추론하며 is_same은 이를 같다고 받아들이지 않는다.
// std::decay를 사용하여 const, reference, volatile을 무시하고 타입을 비교할 수 있다.

// 따라서 우리가 필요한 조건 표현식은 !std::is_same<Person, std::decay_t<T>>::value이다.

class Person
{
public:
	template<
		typename T,
		typename = std::enable_if_t<
					!std::is_same<Person,
							std::decay_t<T>>::value
					>
	>
	explicit Person(T&& n);

	...
};

// 하지만 아직 상속이 관여하는 경우는 해결하지 못하였다.

class SpecialPerson: public Person
{
public:
	SpecialPerson(const SpecialPerson& rhs)
	: Person(rhs)
	{ ... }

	SpecialPerson(SpecialPerson&& rhs)
	: Person(std::move(rhs))
	{ ... }

	...
};

// SpecialPerson과 Person은 다른 타입으로 추론되지만, 우리는 같은 타입으로 추론되기를 원한다.
// std::is_base_of가 이를 해결한다.

class Person
{
public:
	template<
		typename T,
		typename = std::enable_if_t<
					!std::is_base_of<Person,
							std::decay_t<T>
							>::value
					>
	>
	explicit Person(T&& n);

	...
};

// 기존에 구현하고자 하였던 정수타입과 비정수타입의 구분도 위 방식으로 구현할 수 있다.
class Person
{
public:
	template<
		typename T,
		typename = std::enable_if_t<
				!std::is_base_of<Person, std::decay_t<T>>::value			// 복사 생성자(base, derived 모두)와 겹치는 경우 비활성화
				&&
				!std::is_integral<std::remove_reference_t<T>>::value		// 정수 타입 비활성화
		>
	>
	explicit Person(T&& n)
	: name(std::forward<T>(n))
	{ ... }

	explicit Person(int idx)
	: name(nameFromIdx(idx))
	{ ... }

	...

private:
	std::string name;
}
// std::enable_if_t를 통해 템플릿의 인스턴스화를 제한함으로써 universal reference와 오버로딩을 함께 사용할 수 있다.