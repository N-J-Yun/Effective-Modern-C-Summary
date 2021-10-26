enum Color { black, white, red };

auto white = false;										// error, 이 범위에 이미 white가 선언되어 있다.

// C++98이 지원했던 enum은 범위없는 enum이라고 불린다.
// 범위없는 enum 내에 속하는 이름들은 enum이 정의된 scope에서 다시 정의할 수 없다.

// C++11부터는 enum class라는 범위있는 enum을 지원한다.
enum class Color { black, white, red };

auto white = false;
Color c = white;										// error, white라는 enumerator는 이 영역에 없다.
auto c = Color::white;									// 범위를 지정해주어야 보인다.

// 범위 있는 enum은 기존 C++의 원칙처럼 {} 안에서 선언된 이름들은 그 안에서만 보인다.(범위 없는 enum과 다르게)
// 범위 있는 enum은 네임스페이스를 오염시키지 않는다.

enum Color { black, white, red };
std::vector<std::size_t> primeFactors(std::size_t x);
Color c = red;
if(c<14.6)												// Color를 double과 비교하기 위해(Color 타입인 c가 암시적으로 정수 타입으로 변한다.)
{
	auto factors = primeFactors(c);						// c의 소인수들을 계산하기 위해 정수 타입으로 변환된다.
}

enum class Color { black, white, red };
std::vector<std::size_t> primeFactors(std::size_t x);	
Color c = red;
if(c<14.6)												// error, 범위있는 enum은 정수 타입으로의 변환을 지원하지 않는다.
{
	auto factors = primeFactors(c);						// error, 위와 동일
}
// 또한 범위 있는 enum은 다른 타입으로의 정수 타입으로의 암시적인 변환을 지원하지 않기 때문에
// enumerator들의 타입이 훨씬 강력하게 적용된다는 장점도 있다.

enum Color;												// error,
														// 컴파일러는 메모리를 효율적으로 사용하기 위해 enum의 크기를 최적화하여 사용하고 싶어 한다.
														// 이를 가능하게 하기 위해 C++98은 enum의 정의만 허용하고 선언은 허용하지 않았다.
														// -> enum을 수정하거나 새로운 enumerator를 추가하면 재컴파일 해야한다.
														// => 컴파일 의존성 증가
													
enum Color:std::uint8_t;								// 이를 해결하기 위해서는 바탕 타입을 지정해 주어야한다.(크기를 고정시켜주어야 한다.)

enum class Color;										// 하지만 범위 있는 enum은 default 타입을 int라고 정해 두었기 때문에 전방 선언이 가능하다.
enum class Color:std::uint32_t;							// 바탕 타입을 지정해주어도 문제 없다.
// 범위 없는 enum은 컴파일러가 메모리를 최적화하여 사용하고 싶어하기 때문에 크기가 고정되지 않아 
// 전방선언이 불가능하다. 따라서 컴파일 의존성이 증가하게 된다.
// 반면에 범위 있는 enum은 기본 타입이 int로 고정되어 있어 전방 선언을 해도 문제가 없다.

// 범위 없는 enum이 사용하기 편해 보이는 때도 있다. 
// 튜플을 사용할 때
using UserInfo = std::tuple<std::string, std::string, std::size_t>

enum UserInfoFields{ uiName, uiEmail, uiReputation };	// 위 튜플의 각 필드가 어떤 값을 의미하는 지를 담은 범위 없는 enum

UserInfo uInfo;											

// auto val = std::get<1>(uInfo);						// 만약 위 enum이 없다면 프로그래머는 1번 필드가 Email을 뜻한다는 것을 기억해야한다.
auto val = std::get<uiEmail>(uInfo);					// 하지만 이는 말도 안되는 일이므로 enum을 사용해 내가 원하는 필드가 email임을 명확히한다.

// 이를 범위있는 enum으로 구현하고자 한다 코드가 많이 장황해진다.
enum class UserInfoFields{ uiName, uiEmail, uiReputation };
UserInfo uInfo;

auto val = std::get
	<static_cast<std::size_t>(UserInfoFields::uiEmail)>(uInfo);
// 범위 없는 enum이 더 간단한 이유는 암묵적인 정수타입으로의 변환을 허용하기 때문이다.
// 하지만 위에서도 설명했듯이 범위없는 enum 사용은 네임스페이스 오염 및 enumerator들의 의도치 않은 변환이 발생할 수 있다.
// 따라서 위의 장황한 코드를 덜 장황하게 만드는 열거자의 필드에 해당하는 값을 size_t로 변환하여 반환하는 함수
// 를 만들어 사용하는 것이 미래를 위해 더 나은 선택이 될 수도 있다.
template<typename E>
constexpr std::underlying_type_t<E>						// std::get함수는 템플릿이며, 여기에 넘겨주는 것은 템플릿 타입 인수이다.
														// 따라서 열거자를 size_t로 변환하는 함수는 그 결과를 컴파일 도중에 산출해야한다.
														// 그리고 그러기 위해서는 반환값이 constexpr함수 템플릿이어야 한다.

	toUType(E enumerator) noexcept						// 이 함수가 예외를 절대 던지지 않음을 알고 있기 때문에 noexcept선언을 해준다.
{
	return static_cast
		<std::underlying_type_t<E>>(enumerator);		// 반환 타입이 함수 템플릿인 이유는 어떠한 enumerator에 대해서도 작동하기를 원해서인데,
														// 이러한 일반화가 목적이라면 반환타입도 일반화해줄 필요가 있다.
														// size_t타입을 반환하는 것이 아닌 enum의 바탕 타입을 반환하도록 한다. 
}

template<typename E>
constexpr auto											// auto 반환으로 더 깔끔하게 만들 수 있다. 
	toUType(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);	
}

// ->
auto val = std::get
	<toUType(UserInfoFields::uiEmail)>(uInfo);			// 이처럼 사용할 수 있다. 여전히 장황하지만 이전보단 덜하다.
