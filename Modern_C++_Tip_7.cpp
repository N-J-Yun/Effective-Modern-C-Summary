template<typename T>
class TD;										// 정의되지 않은 템플릿 클래스를 선언한다.

int main()
{
	const int theAnswer = 42;

	auto x = theAnswer;

	auto y = &theAnswer;

	TD<decltype(x)> xType;						// 정의되지 않은 템플릿을 사용하려 했기 때문에 에러가 발생한다.
	TD<decltype(y)> yType;						// 해당 컴파일러 에러를 통해 x, y의 타입 추론 결과를 알 수 있다.

	return 0;
}