/*
 * CSCI 460 Homework 2 - Completed by Jared Weiss
 * For Dr. Binhai Zhu on 10/08/21
 *
 * Implements the Consumer/Producer problem using threads
 * (specifically pthreads) to simulate the problem.
 * For this project I implemented a doubly-linked list, where
 * producer threads add either odd or even random numbers to the back
 * of the list and consumer threads consume either odd or even numbers
 * from the front of the list.
 *
 * The code for the Linked List can be found in LinkedList.h and
 * LinkedList.cpp.
 *
 * Compilation instructions:
 * From the command line: (any c++ compiler should work, I hope, but I used g++ on Linux)
 * $ g++ -pthread main.cpp LinkedList.cpp -o main
 * $ ./main > output.txt # send the output to output.txt
 * NOTE: by default the program will run for ~60 seconds, but you can stop it with Ctrl+C.
 * $ cat output.txt # see the output
 *
 * If you want to build using CMake, this code is found at:
 * https://github.com/PlacidFireball/csci_460_hw2_project
 * You can clone it and build it using the included CMakeLists.txt
 * I used Clion to develop this, so it should work in CLion,
 * otherwise, good luck.
 *
 * */

// TODO: Currently all threads are executing in the same order making predictable behaviour for the list. Kinda sucks.

#include <pthread.h>
#include <iostream>
#include <mutex>
#include <random>
#include <unistd.h>
#include "LinkedList.h"
#include "LinkedList.cpp"

#define LOG(s) std::cout << s << std::endl

/* Random Number generation */
static std::random_device dev;
static std::mt19937 rng(dev());
static std::uniform_int_distribution<std::mt19937::result_type> dist25(0,24);
/* List and Mutex */
static LinkedList<unsigned long> list = LinkedList<unsigned long>(); // list of unsigned longs for our threads to mess around with
static std::mutex mutex; // enforce mutual exclusion on the list.

/* even consumer function for one of our threads to run */
[[noreturn]] static void* consume_even() {
    while (true) {
        mutex.lock(); // lock the mutex
        LOG("\n| >> EVEN CONSUMER << |");
        std::cout << "| List Before: ";
        list.print();
        std::cout << "Attempting to consume: " << list.front() << std::endl;
        if (list.len > 0 && list.front() % 2 == 0) { // check if the list is empty and the front is even
            list.del_front(); // if so delete the node at the front of the list
            LOG("| EVEN CONSUMER >> Consumed front"); // log what we did
            list.print();
            mutex.unlock(); // unlock the mutex
        }
        else if (list.len == 0) {
            LOG("| EVEN CONSUMER >> List is empty: waiting"); // log that we are waiting for something to be in the list
            mutex.unlock();
        }
        else {
            LOG("| EVEN CONSUMER >> Cannot consume: front is odd"); // log that we are waiting for the front to be even
            mutex.unlock();
        }
        sleep(1);
    }
}

/* second verse, same as the first */
[[noreturn]] static void* consume_odd() {
    while (true) {
        mutex.lock();
        LOG("\n| >> ODD CONSUMER << |");
        std::cout << "| List Before: ";
        list.print();
        std::cout << "| Attempting to consume: " << list.front() << std::endl;
        list.print();
        if (list.len > 0 && list.front() % 2 == 1) {
            list.del_front();
            LOG("| ODD CONSUMER >> Consumed front");
            //std::cout <<
            list.print();
            mutex.unlock();
        }
        else if (list.len == 0) {
            LOG("| ODD CONSUMER >> List empty, cannot consume: waiting");
            mutex.unlock();
        }
        else {
            LOG("| ODD CONSUMER >> Cannot consume: front is even, waiting");
            mutex.unlock();
        }
        sleep(1);
    }
}

/* producer function for one of our threads to run */
[[noreturn]] static void* produce1() {
    while (true) {
        mutex.lock();
        LOG("\n| >> ODD PRODUCER << |");
        std::cout << "| List Before: ";
        list.print();
        if (list.len < list.MAX_LEN) { // check to make sure the list isn't full
            unsigned long gen = 2*dist25(rng)+1;
            std::cout << "| ODD PRODUCER >> produce: " << gen << std::endl;
            list.add_back(gen); // add an odd number to the back of the list
            list.print();
            mutex.unlock();
        }
        else {
            LOG("| ODD PRODUCER >> The list is full: waiting."); // notify the console that we are waiting
            mutex.unlock();
        }
        sleep(1);
    }
}

/* second verse, same as the first */
[[noreturn]] static void* produce2() {
    while (true) {
        mutex.lock();
        LOG("\n| >> EVEN PRODUCER << |");
        std::cout << "| List Before: ";
        list.print();
        if (list.len < list.MAX_LEN) {
            unsigned long gen = 2*dist25(rng);
            std::cout << "| EVEN PRODUCER >> produce: " << gen << std::endl;
            list.add_back(gen); // except we are using even integers in this guy
            list.print();
            mutex.unlock();
        }
        else {
            LOG("| EVEN PRODUCER >> The list is full: waiting");
            mutex.unlock();
        }
        sleep(1);
    }
}

int main() {

    // Linked List test
    //LinkedList<unsigned long>::test();

    // populate the list with some items
    //for (int i = 0; i < 10; i++) list.add_back((i % 2 + 1)*dist25(dev));

    pthread_t threads[4];
    int thread_create[4];
    // initialize all of our threads and send them off to their respective loops
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
        i++;
        if (i > 60) { break; }
    }
    return 0;
}
