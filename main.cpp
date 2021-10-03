#include <iostream>
#include <mutex>
#include <thread>
#include <random>
#include <chrono>
#include <unistd.h>
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


[[noreturn]] static void consume_even() {
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

[[noreturn]] static void consume_odd() {
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

/*
 * Odd Integer Producers
 * */
[[noreturn]] static void* produce1() {
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
/*
 * Even Integer Producer
 * */
[[noreturn]] static void* produce2() {
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

int main() {

    // Linked List test
    // LinkedList<unsigned long>::test();

    /* compiler complained so I had to do some weird casting to get it all to work */
    // std::thread has a much better api but we weren't allowed to use it :)
    pthread_t threads[4];
    pthread_create(&threads[0], NULL, reinterpret_cast<void *(*)(void *)>(produce1), NULL);
    pthread_create(&threads[1], NULL, reinterpret_cast<void *(*)(void *)>(produce2), NULL);
    pthread_create(&threads[2], NULL, reinterpret_cast<void *(*)(void*)>(consume_even), NULL);
    pthread_create(&threads[2], NULL, reinterpret_cast<void *(*)(void*)>(consume_odd), NULL);

    int i = 0;
    while (true) {
        sleep(1);
        mutex.lock();
        list.print();
        mutex.unlock();
        i++;
        if (i > 60) { break; }
    }
    return 0;
}
