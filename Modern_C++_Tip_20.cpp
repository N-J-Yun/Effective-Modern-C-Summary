// 컴파일러가 자동으로 필요에 의해 생성하는 함수를 특수 멤버 함수라고 한다.
// 그 종류로는 default 생성자, default 소멸자, default 복사 함수들, default 이동 함수들이 있다.

// default 이동 함수들은 이동 함수들이나 복사 함수들, 소멸자가 명시적으로 선언되어 있지 않은 클래스에 대해서만 자동으로 작성된다.
// default 복사 함수들은 각각의 함수가 명시적으로 선언되어 있지 않은 클래스에 대해서만 자동으로 작성되며,
// 만일 이동 함수들이 하나라도 선언되어 있으면 삭제된다.

// Rule of Five
// 만약 위 6개의 함수 중 하나라도 사용자 정의하였거나, delete하였다면 나머지 5개의 함수들도 사용자 정의하거나, delete해라.
