#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex mtx;
condition_variable cv;
bool ready = false;


void supplier_function()
{
	while (true)
	{
		this_thread::sleep_for(chrono::seconds(1));
		{
			unique_lock<mutex> ul(mtx);
			ready = true;
			cout << "provided\n";
			cv.notify_one();
		}
	}
}

void consumer_function()
{
	while (true)
	{
		unique_lock<mutex> ul(mtx);
		while (!ready) {
			cv.wait(ul);
		}
		ready = false;
		cout << "consumed\n";
	}
}


int main()
{
	thread supplier(supplier_function);
	thread consumer(consumer_function);

	supplier.join();
}