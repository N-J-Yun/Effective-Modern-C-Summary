std::vector<bool> features(const Widget& w);				// 5번째 요소가 우선순위를 표시한다.

Widget w;
bool highPriority = features(w)[5];							// 우선순위 표시요소를 가져온다.
processWidget(w, highPriority);								// 우선순위에 맞춰 처리한다.


auto highPriority = features(w)[5];							
processWidget(w, highPriority);								// 미정의 행동!

// 위 같은 현상이 발생하는 이유는 vector::operator[]은 해당 타입의 참조자를 반환하는데
// vector<bool>은 각 bool 요소를 1bit 로 저장하도록 템플릿 특수화가 되어있다.
// 그리고 c++는 비트단위의 접근을 허용하지 않는다!
// 따라서 vector<bool>은 bool&가 아닌 std::vector<bool>::reference를 반환한다.

// std::vector<bool>::reference는 proxy class의 종류로써 vector<bool>::operator[]가 비트에 대한 참조를 반환하는 것처럼 보여준다.

// 위 경우처럼 "보이지 않는" 프록시 타입 때문에 auto가 초기화 표현식의 타입을 잘못 추론할 수 있다.(보이는 프록시 타입도 존재한다, 스마트포인터 등)


