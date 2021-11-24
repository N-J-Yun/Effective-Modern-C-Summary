// perfect forwarding은 효율면에서는 최적의 선택이다.
// 다른 기법과 달리 임시 객체를 생성하지 않기 때문이다.

// 하지만 perfect forwarding에도 단점은 있다.
// 1. perfect forwarding이 불가능한 인수들이 있다.(이후 tip에서 다룬다.)
// 2. 사용자가 유효하지 않은 인수를 전달했을 때 에러 메시지가 굉장히 길고 난해하다.

// perfect forwarding의 에러 메시지는 오버로딩을 포기하거나, const T& 매개변수를 사용하거나, 값 전달 방식의 매개변수를 사용하는 기법들을 사용했을 때의
// 에러 메시지와는 차원이 다른 난해함을 보여준다.
// 위 세 기법들에서의 에러메시지는 타입 변환에 대한 간단한 에러 메시지가 출력되지만
// perfect forwarding의 두 기법의 에러메시지는 이해할 수 없는 100줄이 넘는 메시지가 출력되기도 한다.

// 이를 해결하기 위해 assert를 사용하여 미리 점검하는 방법도 있다.
// std::is_constructible이라는 타입 특성은 한 타입의 객체를 다른 한 타입의 객체로부터 생성할 수 있는지를 컴파일 시점에서 판정한다.
// 이를 사용하면 assertion을 손쉽게 작성할 수 있다.

class Person {
public:
    template <                                        // 이전과 동일
        typename T,
        typename = std::enable_if_t<
            !std::is_base_of<Person, std::decay_t<T> >::value
            &&
            !std::is_integral<std::remove_reference_t<T> >::value
        >
    >
    explicit Person(T&& n)
    : name(std::forward<T>(n))
    {
    	static_assert(
    		std::is_constructible<std::string, T>::value,
    		"Parameter n can't be used to construct a std::string"
    	);
    	...
    }
    ...
};
// 이렇게 하면 사용자가 std::string의 생성에 사용할 수 없는 타입의 인수로 Person 객체를 생성하려 할 때 지정한 에러메시지가 출력된다.
// 하지만 어떤 컴파일러의 경우, static_assert는 생성자의 본문에 있지만 매개변수를 전달하는 코드는 그보다 앞에 나오는 멤버 초기화 리스트에 있기 때문에
// 100줄이 넘는 기존의 에러메시지를 출력한 이후에 지정한 에러 메시지를 출력하기도 한다.