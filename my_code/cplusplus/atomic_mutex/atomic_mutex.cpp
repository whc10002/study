#include <iostream>
#include <chrono>
#include <atomic>
#include <mutex>
#include <thread>

std::mutex _mux;
int i = 0;
int j = 0;

void a()
{
	std::lock_guard<std::mutex> lock(_mux);
	i++;
	j++;
}

struct S
{
	int k = 0;
	int l = 0;
	
	void doSomething()
	{
		k++;
		l++;
	}
};

std::atomic<S> s;
void b()
{
	S tmp = s.load();
	S new_s;
	do {
		new_s = tmp;
		//new_s.doSomething(); // whatever modifications you want
		new_s.k++;
		new_s.l++;
	} while (!s.compare_exchange_strong(tmp, new_s));
}

S no_sync;
void c()
{
	int test = 0;
	test = test * 23;
	no_sync.doSomething();
}

int main(void)
{
	std::thread thr1([]() {
		std::chrono::high_resolution_clock clock;
		auto t1 = clock.now();
		for (int i = 0;i < 1000000;i++) {
			a();
		}
		auto diff1 = clock.now() - t1;
		std::cout << "mutex:" << diff1.count() << std::endl;
	});

	std::thread thr2([]() {
		std::chrono::high_resolution_clock clock;
		auto t1 = clock.now();
		for (int i = 0;i < 1000000;i++) {
			a();
		}
		auto diff1 = clock.now() - t1;
		std::cout << "mutex:" << diff1.count() << std::endl;
	});

	std::thread thr3([]() {
		std::chrono::high_resolution_clock clock;
		auto t2 = clock.now();
		for (int i = 0;i < 1000000;i++) {
			b();
		}
		auto diff2 = clock.now() - t2;
		std::cout << "atomic:" << diff2.count() << std::endl;
	});

	std::thread thr4([]() {
		std::chrono::high_resolution_clock clock;
		auto t2 = clock.now();
		for (int i = 0;i < 1000000;i++) {
			b();
		}
		auto diff2 = clock.now() - t2;
		std::cout << "atomic:" << diff2.count() << std::endl;
	});

	std::thread thr5([]() {
		std::chrono::high_resolution_clock clock;
		auto t3 = clock.now();
		for (int i = 0;i < 1000000;i++) {
			c();
		}
		auto diff3 = clock.now() - t3;
		std::cout << "no_sync:" << diff3.count() << std::endl;
	});

	std::thread thr6([]() {
		std::chrono::high_resolution_clock clock;
		auto t3 = clock.now();
		for (int i = 0;i < 1000000;i++) {
			c();
		}
		auto diff3 = clock.now() - t3;
		std::cout << "no_sync:" << diff3.count() << std::endl;
	});

	thr1.join();
	thr2.join();
	thr3.join();
	thr4.join();
	thr5.join();
	thr6.join();
	
	std::cout << "mutex:\t" << "i:" << i << "\tj:" << j << std::endl;
	
	S tmp = s.load();
	std::cout << "atomic:\t" << "tmp.k:" << tmp.k << "\ttmp.l:" << tmp.l << std::endl;
	
	std::cout << "no_syn:\t" << "no_sync.k:" << no_sync.k << "\tno_sync.l:" << no_sync.l << std::endl;
	return 0;
}
