#include <iostream>
#include <mutex>
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
static LinkedList<unsigned long> list = LinkedList<unsigned long>(); // list of unsigned longs for our threads to mess around with
static std::mutex mutex = std::mutex(); // enforce mutual exclusion on the list.

/* even consumer function for one of our threads to run */
[[noreturn]] static void* consume_even() {
    while (true) {
        mutex.lock(); // lock the mutex
        if (list.len > 0 && list.front() % 2 == 0) { // check if the list is empty and the front is even
            list.del_front(); // if so delete the node at the front of the list
            LOG("EVEN CONSUMER | Consumed front"); // log what we did
            mutex.unlock(); // unlock the mutex
        }
        else if (list.len == 0) {
            LOG("EVEN CONSUMER | List is empty: waiting"); // log that we are waiting for something to be in the list
            mutex.unlock();
        }
        else {
            LOG("EVEN CONSUMER | Cannot consume: front is odd"); // log that we are waiting for the front to be even
            mutex.unlock();
        }
        sleep(1);
    }
}

/* second verse, same as the first */
[[noreturn]] static void* consume_odd() {
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
        sleep(1);
    }
}

/* producer function for one of our threads to run */
[[noreturn]] static void* produce1() {
    while (true) {
        mutex.lock();
        if (list.len < list.MAX_LEN) { // check to make sure the list isn't full
            list.add_back(2*dist25(rng)+1); // add an odd number to the back of the list
            LOG("PRODUCER1 | Add odd to list"); // log what we did
            mutex.unlock();
        }
        else {
            LOG("PRODUCER1 | The list is full: waiting."); // notify the console that we are waiting
            mutex.unlock();
        }
        sleep(1);
    }
}

/* second verse, same as the first */
[[noreturn]] static void* produce2() {
    while (true) {
        mutex.lock();
        if (list.len < list.MAX_LEN) {
            LOG("PRODUCER2 | Add even to list");
            list.add_back(2*dist25(rng)); // except we are using even integers in this guy
            mutex.unlock();
        }
        else {
            LOG("PRODUCER2 | The list is full: waiting");
            mutex.unlock();
        }
        sleep(1);
    }
}

int main() {

    // Linked List test
    // LinkedList<unsigned long>::test();

    /* compiler complained, so I had to do some weird casting to get it all to work */
    // std::thread has a much better api, but we weren't allowed to use it :)
    pthread_t threads[4];
    int thread_create[4];
    /* initialize all of our threads and send them off to their respective loops */
    thread_create[0] = pthread_create(&threads[0], NULL, reinterpret_cast<void *(*)(void *)>(produce1), NULL);
    thread_create[1] = pthread_create(&threads[1], NULL, reinterpret_cast<void *(*)(void *)>(produce2), NULL);
    thread_create[2] = pthread_create(&threads[2], NULL, reinterpret_cast<void *(*)(void*)>(consume_even), NULL);
    thread_create[3] = pthread_create(&threads[2], NULL, reinterpret_cast<void *(*)(void*)>(consume_odd), NULL);

    // error handling
    for (int thread : thread_create)
        if (thread) {
            LOG("Failed init threads");
            exit(1);
        }

    // run the program for ~60 seconds
    int i = 0;
    while (true) {
        sleep(1);
        mutex.lock(); // make sure we don't try to print anything while the threads (not main) are reading/writing from it
        list.print();
        mutex.unlock();
        i++;
        if (i > 60) { break; }
    }
    return 0;
}
