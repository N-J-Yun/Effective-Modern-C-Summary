typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS;

using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;
// 위의 두 선언은 같은 의미이다. 

// 하지만 using을 쓰는 것이 다른 상황에서는 더 간단하며 좋다.
typedef void (*FP)(int, const std::string&);

using FP = void (*)(int, const std::string&);				
// 함수 포인터를 선언할 때 using을 사용하는 것이 훨씬 보기도 좋고 직관적이다.

template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;				

MyAllocList<Widget> lw;										// using으로 선언하면 사용할 때 깔끔하게 사용할 수 있다.

template<typename T>
class Widget
{
private:
	MyAllocList<T> list;									// typedef와 달리 typename 없이 ::type 없이 사용할 수 있다.
};

template<typename T>
struct MyAllocList
{
	typedef std::list<T, MyAlloc<T>> type;					// typedef를 쓰기 위해서는 템플릿 구조체 내에서 선언해주어야 한다.
};

MyAllocList<Widget>::type lw;								// ::type을 붙혀 줘야 한다.

template<typename T>
class Widget
{
private:
	typename MyAllocList<T>::type list;						// 템플릿 내에서 사용한다면 typename을 통해서 타입임을 명시해주어야 한다.
															// ::type은 T에 의존적인 타입이기 때문이다.
};
// 템플릿을 선언할 때도 템플릿에서 사용할 때도 using을 쓰는 것이 훨씬 좋다.
