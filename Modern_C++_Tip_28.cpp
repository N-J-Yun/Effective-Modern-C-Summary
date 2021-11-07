void process(const Widget& lvalArg);								// lvalue를 받는 process
void process(Widget&& rvalArg);										// rvalue를 받는 process

template<typename T>
void logAndProcess(T&& param)										// param을 process로 넘겨주는 template 함수
{
	auto now =
		std::chrono::system_clock::now();

	makeLogEntry("calling 'process'", now);
	process(std::forward<T>(param));
}

int main()
{
	Widget w;

	logAndProcess(w);												// lvalue 버전 호출
	logAndProcess(std::move(w));									// rvluae 버전 호출

	return 0;
}
// forward는 param이 어떤 타입으로 넘어왔는 지를 체크하여(T타입을 검사하여) 그 타입 그대로 process를 호출한다.
// lvalue로 param을 받았다면 lvalue버전을 호출하고, rvlaue로 param을 받았다면 rvlalue버전을 호출한다.

// std::move는 무조건적인 rvlalue로의 캐스팅을 약속하고,
// std::forward는 입력받는 인자에 맞추어 rvalue로의 캐스팅을 수행한다.