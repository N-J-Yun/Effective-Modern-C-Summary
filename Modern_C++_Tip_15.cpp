class Widget
{
public:
	...
private:
	Widget(const Widget&);
	Widget& operator=(const Widget&);							// C++98에서의 복사방지법
};
// C++98에서는 클래스 사용자의 함수 사용을 막기 위해 private 영역에 함수를 선언하고 정의하지 않는 방법을 사용했다.
// C++11부터는 더 좋은 방법을 사용할 수 있다.
class Widget
{
public:
	Widget(const Widget&) = delete;
	Widget& operator=(const Widget&) = delete;					// C++11부터의 복사방지법
};
// delete 키워드를 함수 뒤에 붙힘으로써 클래스 사용자의 함수 사용을 막을 수 있다.
// 그리고 delete 키워드를 붙히는 함수들은 관례적으로 public 접근자 영역에 선언한다.
// 만약 private 영역에 delete 함수를 선언했을 때 
// 해당 함수를 호출한 사용자에게 컴파일러는 "private 영역의 함수에 접근했다" 라고만 알려주기 때문이다.

bool isLucky(int number);										// 이 사용법은 허용한다.
bool isLucky(char) = delete;									// 하지만 이러한 사용법은 방지한다.
bool isLucky(bool) = delete;									// 위와 동일
bool isLucky(double) = delete;									// 위와 동일하고 float의 사용도 막는다.
// 위처럼 오버로딩을 delete함으로써 특정 타입의 입력을 막을 수도 있다.

// 직관성뿐만 아니라 성능상으로도 delete 키워드 사용이 더 좋다.
// C++98에서의 함수 사용 방지법은(private영역에 선언) linking 타임에 에러가 발생했음을 알려주지만
// C++11에서의 함수 사용 방지법은(delete 키워드 사용) compile 타임에 에러가 발생했음을 알려준다.

// 템플릿에서의 경우는 특히 더하다.

// C++98 방식은 클래스 밖의 함수에는 절대 사용할 수 없고, 클래스 안의 함수도 일부만 사용가능하며
// 그게 된다고 하더라도 linking 타임에서 의도를 달성할 수 있다.

// 사실 C++98에서의 함수 사용 방지법(private 영역에 선언하고 정의하지 않는 것)이 달성하고자 했던 바는
// delete 함수를 사용함으로써 모두 이루어졌다. 그냥 delete 함수를 사용하자.
