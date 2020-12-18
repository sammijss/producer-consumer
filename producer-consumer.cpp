#include<mutex>
#include<queue>
#include<thread>
#include<unistd.h>
#include<condition_variable>
#include<iostream>
using namespace std;

class MsgQueue {
	private:
		std::mutex _mutex;
		std::condition_variable _condvar; 
		std::queue<std::string> _msgqueue;

	public:
		static MsgQueue& Getinstance() {
			static MsgQueue obj;
			return obj;
		}

		void Addmsg(std::string msg) {
			std::unique_lock<std::mutex> lock(_mutex);
			cout <<"Adding msg: " <<msg <<endl;
			_msgqueue.push(std::move(msg));
			lock.unlock();
			_condvar.notify_all();
		}

		const std::string Getmsg() {
			std::unique_lock<std::mutex> lock(_mutex);
			_condvar.wait(lock, [&] (){ return (!_msgqueue.empty()); });
			std::string msg = _msgqueue.front();
			_msgqueue.pop();
			lock.unlock();
			return (msg);
		}
};

class Producer {
	public:
		static void Produce() {
			MsgQueue &msgobj = MsgQueue::Getinstance();
			long long counter = 0;
			while (true) {
				msgobj.Addmsg(std::string("Count: " + std::to_string(counter++)));
				usleep(100);	/* delay in time to produce msg */
			}
		}
};

class Consumer {
	public:
		static void Consume() {
			MsgQueue &msgobj = MsgQueue::Getinstance();
			while (true) {
				const std::string msg = msgobj.Getmsg();
				cout <<"msg: " <<msg <<endl;
			}
		}
};

int main() {
	std::thread producer1(Producer::Produce);
	std::thread producer2(Producer::Produce);
	std::thread consumer1(Consumer::Consume);

	producer1.join();
	producer2.join();
	consumer1.join();
}
