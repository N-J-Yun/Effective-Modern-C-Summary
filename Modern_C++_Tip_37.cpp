// f라는 함수에 인수를 전달하는 가장 기초적인 형태의 함수 템플릿
template<typename T>
void fwd(T&& param)
{
	f(std::forward<T>(param));
}

// fwd의 가변인수 버전
template<typename... Ts>
void fwd(Ts&&... params)
{
	f(std::forward<Ts>(params)...);
}

// 위 전달 함수에 의하면 대부분의 경우 아래 두 표현식은 같다.
f( expression );
fwd( expression );

// 하지만 이 둘의 결과가 달라지는 경우가 있다.
// 템플릿 타입 추론이 실패하거나 틀린 타입을 추론했을 때 perfect forwarding은 실패한다!

// 1. 중괄호 initializers
void f(const std::vector<int>& v);

f({1, 2, 3 });													// 좋음, 암시적으로 std::vector<int>로 변환된다.

fwd({1, 2, 3 });												// 에러
// 템플릿 타입 추론은 중괄호 initializer를 추론하지 못한다.
// 타입 추론이 실패하기 때문에 perfect forwarding도 실패한다.

// 하지만 auto를 사용하여 이를 해결할 수 있다.
auto il = {1, 2, 3};

fwd(il);														// 좋음, il이 auto에 의해 std::initializer_list<int>로 추론되어 전달되었다.

// 2. null 포인터를 뜻하는 0 또는 null
// null포인터의 의미로 0 또는 null을 템플릿에 넘겨주려 하면 컴파일러가 그것을 정수 타입으로 추론하기 때문에 문제가 된다.
// 틀린 타입으로 추론하기 때문에 perfect forwarding은 실패한다.

// 이는 nullptr을 사용함으로써 간단히 해결할 수 있다.

// 3. 클래스 내에 선언만 된 정수 static const 및 constexpr 데이터 멤버
class Widget
{
public:
	static constexpr std::size_t MinVals = 28;					// MinVals는 선언만 되고 정의는 없음.
	...
};

std::vector<int> widgetData;
widgetData.reserve(Widget::MinVals);							// MinVals 사용

// 위 코드는 아무 문제가 없지만 만약 어떤 코드가 MinVals의 주소를 취한다면 컴파일은 되지만 링크에 실패한다.
// 컴파일러가 산출한 코드에서 참조는 포인터처럼 취급되는 것이 보통이므로 MinVals가 universal reference에 묶이면 링크에 실패ㅏㄴ다.

// 따라서 아래 코드는 문제가 없지만
void f(std::size_t val);
f(Widget::MinVals);

// 아래 코드는 문제가 생긴다.
template<typename T>
void fwd(T&& param)												// 참조를 취했기 때문에 주소를 취함 -> 링크에 실패
{
	f(std::forward<T>(param));
}
fwd(Widget::MinVals);

// 이것을 이식성 있게 해결하려면 구현 파일에 MinVals의 정의르르 만들어 줘야 한다.
constexpr std::size_t Widget::MinVals;							// Widget의 .cpp 파일에서

// 4. 오버로딩된 함수 이름과 템플릿 이름
void f(int (*pf)(int));
void f(int pf(int));											// 위와 같은 f선언

// 오버로딩된 함수 processVal
int processVal(int value);
int processVal(int value, int priority);

f(processVal);													// int하나를 취하는 버전의 processVal을 f에 넘긴다.
// f가 요구하는 타입을 미리 알 수 있기 때문에 processVal을 넘겨주는 것이 가능하다.

// 하지만 다음은 컴파일되지 않는다.
template<typename T>
void fwd(T&& param)
{
	f(std::forward<T>(param));
}
fwd(processVal);												// 어떤 processVal인지 알 수 없다.
// fwd는 템플릿 함수이라 어떤 타입도 받을 수 있기 때문에 오버로딩된 두 함수 중에 어느 것을 전달해야 할지 모호하다.

// 같은 이유로 다음 코드도 컴파일되지 않는다.
template<typename T>
T workOnVal(T param)
{ ... }
fwd(workOnVal);													// workOnVal의 어떤 템플릿 인스턴스인지 알 수 없다.

// 이를 해결하기 위해서는 명시적으로 오버로딩된 함수나 템플릿 인스턴스를 지정해야 한다.
using ProcessFuncType = int(*)(int);
ProcessFuncType processValPtr = processVal;

fwd(processValPtr);
fwd(static_cast<ProcessFuncType>(workOnVal));
// fwd가 전달하는 함수 포인터의 타입을 미리 알고 있다면 위같은 명시적인 지정이 가능하다.

// 5. 비트필드
struct IPv4Header {
    std::uint32_t version:4,
                    IHL:4,
                    DSCP:6,
                    ECN:2,
                    totalLength:16;
    ...
};
// 비트 단위로 32bit를 각 영역으로 나누었다.

void f(std::size_t sz);											// totalLength의 크기만큼을 인자로 받는다.

IPv4Header h;
...
f(h.totalLength);												// ok

template<typename T>
void fwd(T&& param)
{
	f(std::forward<T>(param));
}
fwd(h.totalLength);												// 에러
// 메모리의 주소는 바이트 단위로 할당되기 때문에 임의의 비트를 가리키는 포인터는 만들 수 없다.
// 위 경우처럼 참조는 포인터처럼 취급되며 T&&로 param을 받았기 때문에 비트에 대한 주소를 취하려는 시도는 에러를 유발한다.

// 이를 해결하려면 비트필드 값의 복사본을 직접 생성하여 그 복사본을 전달해야 한다.
auto length = static_cast<std::uint16_t>(h.totalLength);
fwd(length);