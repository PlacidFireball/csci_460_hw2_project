#include <iostream>
#include <mutex>
#include <thread>
#include <random>
#include <chrono>
#include "LinkedList.h"
#include "LinkedList.cpp"

#define LOG(s) std::cout << s << std::endl
#define GLOBAL_DELAY 1000

/* Random Number generation */
static std::random_device dev;
static std::mt19937 rng(dev());
static std::uniform_int_distribution<std::mt19937::result_type> dist25(0,24);
/* List and Mutex */
static LinkedList<unsigned long> list = LinkedList<unsigned long>();
static std::mutex mutex = std::mutex();

class EvenConsumer {
public:
    [[noreturn]] static void consume() {
        while (true) {
            mutex.lock();
            if (list.len > 0 && list.front() % 2 == 0) {
                list.del_front();
                LOG("EVEN CONSUMER | Consumed front");
                mutex.unlock();
            }
            else if (list.len == 0) {
                LOG("EVEN CONSUMER | List is empty: waiting");
                mutex.unlock();
            }
            else {
                LOG("EVEN CONSUMER | Cannot consume: front is odd");
                mutex.unlock();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(GLOBAL_DELAY));
        }
    }
};
class OddConsumer {
public:
    [[noreturn]] static void consume() {
        while (true) {
            mutex.lock();
            if (list.len > 0 && list.front() % 2 == 1) {
                list.del_front();
                LOG("ODD CONSUMER | Consumed front");
                mutex.unlock();
            }
            else if (list.len == 0) {
                LOG("ODD CONSUMER | List empty, cannot consume: waiting");
                mutex.unlock();
            }
            else {
                LOG("ODD CONSUMER | Cannot consume: front is even");
                mutex.unlock();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(GLOBAL_DELAY));
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
            if (list.len < list.MAX_LEN) {
                LOG("PRODUCER1 | Add odd to list");
                list.add_back(2*dist25(rng)+1);
                mutex.unlock();
            }
            else {
                LOG("PRODUCER1 | The list is full: waiting.");
                mutex.unlock();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(GLOBAL_DELAY));
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
                LOG("PRODUCER2 | Add even to list");
                list.add_back(2*dist25(rng));
                mutex.unlock();
            }
            else {
                LOG("PRODUCER2 | The list is full: waiting");
                mutex.unlock();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(GLOBAL_DELAY));
        }
    }
};




int main() {

    // Linked List test
    LinkedList<unsigned long>::test();

    // Threads to run various functions
    std::thread produce_begin_thread = std::thread(Producer1::produce);
    std::thread produce_end_thread = std::thread(Producer2::produce);
    std::thread consume_begin_thread = std::thread(EvenConsumer::consume);
    std::thread consume_end_thread = std::thread(OddConsumer::consume);

    int i = 0;
    while (true) {
        sleep(1);
        mutex.lock();
        list.print();
        mutex.unlock();
        i++;
        if (i > 60) { break; }
    }
    produce_begin_thread.join();
    produce_end_thread.join();
    consume_begin_thread.join();
    consume_end_thread.join();

    return 0;
}
