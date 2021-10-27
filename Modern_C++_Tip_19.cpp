class Polynominal
{
public:
	using RootsType = std::vector<double>;

	RootsType roots() const
	{
		if(!rootsAreValid)								// 캐싱이 되지 않았다면
		{
			...											// 캐싱 작업을 수행하고
			rootsAreValid = true;						// 유효여부를 true로 바꾼다.
		}
		return rootVals;								// 근을 반환한다.
	}
private:
	mutable bool rootsAreValid{ false };				// 캐싱 여부를 체크
	mutable RootsType rootsVals{};						// 다항식의 근
														// 근과 캐싱 여부는 최초1회에만 값이 바뀔 수 있다.
														// 따라서 근을 반환하는 roots() 는 const로 선언하고
														// 멤버 변수들을 mutable로 선언하였다.
};
// 하지만 이러한 방식은 스레드안전성을 위반한다.
// roots()함수는 const로 선언되어있기 때문에 thread는 동시에 read 해도 된다고 판단한다.
// 하지만 그렇게 되면 최초1회의 경우 동시에 값이 바뀌어버리는 일이 발생하게 된다.

// 이를 가장 쉽게 해결하는 방법은 mutex를 사용하는 것이다.
class Polynominal
{
public:
	using RootsType = std::vector<double>;
	RootsType roots() const
	{
		std::lock_guard<std::mutex> g(m);				// mutex lock
		if(!rootsAreValid)						
		{
			...									
			rootsAreValid = true;				
		}
		return rootVals;						
														// mutex unlock
	}
private:
	mutable std::mutex m;
	mutable bool rootsAreValid{ false };		
	mutable RootsType rootsVals{};				
};
// 하지만 mutex는 복사, 이동을 지원하지 않는다.
// 따라서 mutex m 을 Polynominal 클래스의 멤버 변수로 넣는다면 Polynominal객체 또한 복사, 이동을 지원하지 않게 된다.

// std::atomic 변수를 사용해 뮤텍스에 비해 성능상의 이점을 볼 수 있지만 이는 하나의 변수 또는 메모리 장소를 다룰 때에만 적합하다.