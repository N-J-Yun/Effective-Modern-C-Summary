int main()
{
	int sz;
	constexpr auto arraySize1 = sz;								// error, sz는 컴파일 시간에 값을 알 수 없다.
	std::array<int, sz> data1;									// error, sz는 컴파일 시간에 값을 알 수 없다.

	constexpr auto arraySize2 = 10;								// ok
	std::array<int, arraySize2> data2;							// ok

	const auto arraySize = sz;									// ok, arraySize는 sz를 복사한 값의 상수이다.
	std::array<int, arraySize> data;							// error, sz는 컴파일 시간에 값을 알 수 없다.

	return 0;
}
// 모든 const 객체가 컴파일 시점에서 알려지는 값으로 초기화되지는 않는다.
// 모든 constexpr은 const이지만, 모든 const가 constexpr인 것은 아니다.
// 컴파일 시간에 결정되는 상수를 사용하고 싶다면 const가 아닌 constexpr을 사용해야 한다.

// constexpr을 반환하는 멤버함수를 사용함으로써 더 효율좋은 코드를 만들어낼 수도 있다.
class Point
{
public:
	constexpr Point(double xVal = 0, double yVal = 0) noexcept
		: x(xVal)
		, y(yVal)
	{}

	constexpr double xValue() const noexcept { return x; }
	constexpr double yValue() const noexcept { return y; }

	constexpr void setX(double newX) noexcept { x = newX; }		// C++11에서는 안되지만 C++14부터는 제약이 해제되어 setter에도 가능.
	constexpr void setY(double newY) noexcept { x = newY; }

private:
	double x, y;
};

constexpr Point reflection(const Point& p) noexcept				// Point의 setter와 생성자가 모두 constexpr이기 때문에
{																// constexpr 형식으로 반환할 수 있게 되었다.
	Point result;

	result.setX(-p.xValue());
	result.setY(-p.yValue());

	return result;
}

constexpr Point midpoint(const Point& lhs, const Point& rhs) noexcept
{
	return { (lhs.xValue() + rhs.xValue()) / 2,
			 (lhs.yValue() + rhs.yValue()) / 2 };				// setter를 사용하지 않고 getter만으로 임시 객체를 생성해 복사본을 반환
}

int main()
{
	constexpr Point p1(9.4, 27.7);
	constexpr Point p2(28.8, 5.3);
	constexpr auto mid = midpoint(p1, p2);						
	constexpr auto reflectMid = reflection(mid);				// 호출한 함수들이 모두 constexpr 반환값이기 때문에 컴파일 타임에 정해진 값이
																// 들어온다면 mid, reflectMid도 컴파일 타임 상수로 초기화될 것이다.
	return 0;
}
// constexpr을 사용할 수 있다면 반드시 사용하자.
// 컴파일타임 상수는 소프트웨어가 더 빨라지게 한다.(컴파일 시간은 길어지지만)