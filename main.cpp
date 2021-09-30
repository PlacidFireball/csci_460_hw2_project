#include <iostream>
#include <mutex>
#include <thread>
#include <random>
#include <pthread.h>
#include <chrono>
#include "LinkedList.h"
#include "LinkedList.cpp"

#define LOG(s) std::cout << s << std::endl

/* Random Number generation */
std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist25(0,24);
LinkedList<unsigned long> list = LinkedList<unsigned long>();
std::mutex mutex = std::mutex();

class ConsumerBegin {
public:
    [[noreturn]] static void consume() {
        while (true) {
            mutex.lock();
            if (list.len > 0 && list.front() % 2 == 0) list.del_front();
            else LOG("The list is empty, there is nothing to consume: Consumer Begin");
            mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};
class ConsumerEnd {
public:
    [[noreturn]] static void consume() {
        while (true) {
            mutex.lock();
            if (list.len > 0 && list.back() % 2 == 1) list.del_back();
            else LOG("The list is empty, there is nothing to consume: Consumer End");
            mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};
/*
 * Odd Integer Producers
 * */
class Producer1 {
public:
    [[noreturn]] static void produce() {
        while (true) {
            mutex.lock();
            if (list.len < 30) {
                list.add_back(2*dist25(rng)+1);
            }
            else LOG("The list is full, cannot produce: Producer1");
            list.print();
            mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};
/*
 * Even Integer Producer
 * */
class Producer2 {
public:
    [[noreturn]] static void produce() {
        while (true) {
            mutex.lock();
            if (list.len < 30) {
                list.add_back(2*dist25(rng));
            }
            else LOG("The list is full, cannot produce: Producer2");
            list.print();
            mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};




int main() {

    /* Threads to run various functions */
    std::thread produce_begin_thread = std::thread(Producer1::produce);
    std::thread produce_end_thread = std::thread(Producer2::produce);
    std::thread consume_begin_thread = std::thread(ConsumerBegin::consume);
    std::thread consume_end_thread = std::thread(ConsumerEnd::consume);

    int i = 0;
    while (true) {
        sleep(1);
        i++;
        if (i < 60) { break; }
    }

    consume_begin_thread.join();
    consume_end_thread.join();
    produce_end_thread.join();
    produce_begin_thread.join();
    return 0;
}
