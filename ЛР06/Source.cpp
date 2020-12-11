#include <vector>
#include <list>
#include <shared_mutex>
#include <thread>
#include <iostream>
#include <locale>
#define FULL 10
using namespace std;

void Consumer(int time);
void Producer(int time);
void PrintVector();
bool IsEmpty(std::unique_lock<std::mutex>& lock);
bool IsFull(std::unique_lock<std::mutex>& lock);

std::mutex acs;
std::vector<int> st;

int main()
{
	setlocale(LC_ALL, "Russian");

	thread t1([]() {Producer(500);  });
	thread t2([]() {Consumer(300);  });
	thread t3([]() {Producer(700);  });


	t1.join();
	t2.join();
	t3.join();


	return 1;
}

void Consumer(int time)
{
	std::unique_lock<std::mutex> lock(acs, defer_lock);

	while (true)
	{
		lock.lock();
		if (!IsEmpty(lock))
		{
			st.pop_back();
			cout << "Потребитель " << this_thread::get_id() << " :: ";
			PrintVector();
		}
		lock.unlock();
		this_thread::sleep_for(chrono::milliseconds(time));
	};

}

void Producer(int time)
{
	std::unique_lock<std::mutex> lock(acs, defer_lock);
	lock.lock();
	cout << this_thread::get_id() << " производитель начал" << endl;
	lock.unlock();


	while (true)
	{
		lock.lock();
		if (!IsFull(lock))
		{
			st.push_back(5);
			cout << "Производитель " << this_thread::get_id() << " :: ";
			PrintVector();
		}
		lock.unlock();
		this_thread::sleep_for(chrono::milliseconds(time));
	}

	lock.lock();
	cout << this_thread::get_id() << " Потребитель закончил" << endl;
	lock.unlock();
};


void PrintVector()
{
	for (int i = 0; i < st.size(); i++)
	{
		cout << st[i] << "\t";
	}cout << endl;
}


bool IsEmpty(std::unique_lock<std::mutex>& lock)
{
	bool temp = st.empty();
	return temp;
};


bool IsFull(std::unique_lock<std::mutex>& lock)
{
	bool temp;
	if (st.size() == FULL)
	{
		temp = true;
	}
	else
	{
		temp = false;
	};
	return temp;
}

