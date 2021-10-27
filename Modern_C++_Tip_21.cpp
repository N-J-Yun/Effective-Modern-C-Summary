// 소유권 독점 자원의 관리에서는 unique_ptr을 사용하자.
class Investment
{
public:
	...
	virtual ~Investment();												// base 클래스 포인터를 이용해 파생 클래스를 delete 한다 -> virtual
	...
};

class Stock:
    public Investment { ... };
 
class Bond:
    public Investment { ... };
 
class RealEstate:
    public Investment { ... };


template<typename... Ts>
auto makeInvestment(Ts&&... params)
{
	auto delInvmt = [](Investment* pInvestment)							// 람다 표현식을 통해 커스텀 deleter를 정의하였다.
					{
						makeLogEntry(pInvestment);						// 로그 기록(이걸 위해 커스텀 deleter 만든 것.)
						delete pInvestment;								// base 클래스 포인터를 이용해 파생 클래스 delete
					};

	std::unique_ptr<Investment, decltype(delInvmt)>						// 커스텀 deleter를 두 번째 인자로 받는 unique_ptr
		pInv(nullptr, delInvmt);

	if( ... ) // Stock 타입을 만들고자 한다면
		pInv.reset(new Stock(std::forward<Ts>(params)...));				// reset은 unique_ptr이 가리키는 것을 바꾼다.
	else if( ... ) // Bond 타입..
		pInv.reset(new Bond(std::forward<Ts>(params)...));
	else if( ... ) // RealEstate 타입..
		pInv.reset(new RealEstate(std::forward<Ts>(params)...));
	return pInv;														// caller가 선언한 포인터로 소유권을 넘긴다.(현재는 unique_ptr임시 객체)
}
// unique_ptr은 default 상태일 때 raw pointer와 크기가 동일하다.
// 하지만 더 많은 기능을 할 수 있으며, raw pointer를 사용했을 때 발생할 수 있는 미묘한 버그들을 방지해주는 상위호환 기능이다.

// 기본적으로 자원 파괴는 delete를 통해 일어나나, 커스텀 deleter를 지정할 수도 있다. 
// 상태 있는 deleter(capture list가 비어있지 않은 람다 표현식)나 함수 포인터를 사용하면 std::unique_ptr 객체의 크기가 커진다.(1word -> 2word)

// unique_ptr은 shared_ptr로 쉽게 변환될 수 있다는 매우 큰 장점이 있다.
// 이 점 때문에 unique_ptr은 팩토리 함수의 구현에 매우 적절하다.
// client는 해당 팩토리 함수를 의도에 맞게 shared_ptr로도 받을 수 있고, unique_ptr로도 받을 수 있다.