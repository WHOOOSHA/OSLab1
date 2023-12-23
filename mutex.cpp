#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex mtx;
condition_variable cv;
bool flag = false;


void supplier_function()
{
	while (true)
	{
		this_thread::sleep_for(chrono::seconds(1));
		{
			unique_lock<mutex> ul(mtx);
			flag = true;
			cout << "Событие отправлено\n";
			cv.notify_one();
		}
	}
}

void consumer_function()
{
	while (true)
	{
		unique_lock<mutex> ul(mtx);
		while (!flag) {
			cv.wait(ul);
		}
		flag = false;
		cout << "Событие обработано\n";
	}
}


int main()
{
	setlocale(LC_ALL, "ru");
	thread supplier(supplier_function);
	thread consumer(consumer_function);

	supplier.join();
	consumer.join();

	return 0;
}