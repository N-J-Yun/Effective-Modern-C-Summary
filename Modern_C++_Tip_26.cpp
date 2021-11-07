-----------"Widget.h"----------------							// client가 사용할 헤더 파일
class Widget													// 컴파일 의존도가 클 수도 있는(gadget같은) 데이터 멤버의 포인터를 멤버로 가진다
{
public:
	Widget();
	...
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;								// 스마트 포인터를 활용한 pimpl idiom
																// 선언만 하고 정의는 되어있지 않다.(불완전한 타입이라고 부른다.)
};
-------------------------------------
-----------"Widget.cpp"--------------
#include "Widget.h"												
#include "gadget.h"												// client는 gadget.h의 내용이 바뀌어도 재컴파일 없이 Widget.h를 사용할 수 있다
#include <string>
#include <vector>

struct Widget::Impl
{
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3;											// 내용이 바뀔 여지가 큰 데이터 타입
};

Widget::Widget()
	: pImpl(std::make_unique<Impl>())							// 스마트포인터로 선언하여 소멸자를 따로 선언하지 않아도 자동으로 포인터 해제됨
	{}
------------------------------------
int main()
{
	Widget w;													// error

	return 0;
}
// 하지만 위 코드는 실제로는 사용할 수 없다.
// 대부분의 표준 라이브러리들의(unique_ptr도) 기본 삭제자 함수들은 delete를 적용하기 전에 포인터가 불완전한 타입을 가리키는 지를 static_assert로 확인
// 그리고 w의 소멸을 위한 코드를 산출하는 과정에서 일반적으로 static_assert는 참이 아닌 것으로 판명되어 오류 메시지가 뜬다.

// 위 같은 문제의 이유는 컴파일러가 자동으로 생성하는 특수 멤버 함수들은 암묵적으로 inline이기 때문이다.
// w가 생성되는 행에서 이후의 암묵적 파괴로 이어지는 객체를 명시적으로 생성한다.
// 따라서 컴파일러가 그 소스파일에만 있는 Widget의 소멸자의 본문을 보게 한다면 client의 코드는 문제없이 컴파일된다.
-----------"Widget.h"----------------			
class Widget									
{
public:
	Widget();
	~Widget();													// 선언만 한다

	Widget(Widget&&);											// 위와 같은 에러를 오류를 발생하므로 이동 함수들도 선언만 한다.
	Widget& operator=(const Widget&&);
	...
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;				
												
};
-------------------------------------
-----------"Widget.cpp"--------------
#include "Widget.h"						
#include "gadget.h"						
#include <string>
#include <vector>
struct Widget::Impl
{
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3;					
};
Widget::Widget()
	: pImpl(std::make_unique<Impl>())	
	{}
Widget::~Widget() = default;									// 정의, 기본 생성 소멸자의 내용이 딱히 틀린게 없으므로 default 사용
Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(const Widget&& rhs) = default;
------------------------------------
// Widget클래스는 기본적으로 사용하기 위해 만들어진 클래스이므로 복사 함수들도 지원해야 한다.
// 하지만 위 정의에서는 이동 함수들만 정의되었기 때문에 자동으로 복사 함수들이 생성되지 않는다.
// 그리고 정의해준다 해도 unique_ptr자체만 복사하는 얕은 복사를 수행하기 때문에 직접 정의해줄 필요가 있다.

-----------"Widget.h"----------------			
class Widget									
{
public:
	Widget();
	~Widget();	

	Widget(Widget&&);									
	Widget& operator=(const Widget&&);

	Widget(const Widget&);
	Widget& operator=(const Widget&);
	...
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;				
												
};
-------------------------------------
-----------"Widget.cpp"--------------
#include "Widget.h"						
#include "gadget.h"						
#include <string>
#include <vector>
struct Widget::Impl
{
	std::string name;
	std::vector<double> data;
	Gadget g1, g2, g3;					
};
Widget::Widget()
	: pImpl(std::make_unique<Impl>())	
	{}
Widget::~Widget() = default;		

Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(const Widget&& rhs) = default;

Widget::Widget(const Widget& rhs)
	: pImpl(std::make_unique<Impl>(*rhs.pImpl))					// 얕은 복사가 아닌 깊은 복사를 직접 구현하였다.			
	{}
Widget& Widget::operator=(const Widget& rhs) 
{
	*pImpl = *rhs.pImpl;										// 위와 같다.
	return *this;
}
------------------------------------
// unique_ptr대신 shared_ptr을 사용하면 위 문제들은 모두 사라진다.
// 이는 커스텀 deleter를 지원하는 방식의 차이에서 비롯된 것이다.
// unique_ptr에서 deleter의 타입은 스마트 포인터 타입의 일부이며, shared_ptr에서는 아니다.
// 이 때문에 unique_ptr은 더 작은 runtime 자료구조와 더 빠른 runtime 코드를 만들어낼 수 있다.
