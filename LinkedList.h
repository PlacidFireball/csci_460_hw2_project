//
// Created by placidfireball on 9/29/21.
//

#ifndef CSCI_460_HW2_PROJECT_LINKEDLIST_H
#define CSCI_460_HW2_PROJECT_LINKEDLIST_H

#include <vector>
#include <array>

#define LOG(s) std::cout << s << std::endl

template <typename T>
class LinkedList {
public:
    class Node {
    public:
        std::string id;
        T data;
        Node* prev;
        Node* next;
        explicit Node();
        explicit Node(T d, Node* previous_node, Node* next_node);
    };
    Node dummy_node = Node(-1, &tail, &head);
    Node head = Node(-1, &dummy_node, &tail);
    Node tail = Node(-1, &head, &dummy_node);
public:
    [[maybe_unused]] static void test();
    Node nodes[1000];
    long num_nodes = 0;
    const long MAX_LEN = 30;
    long len;
    LinkedList();
    ~LinkedList();
    T front();
    T back();
    bool add_front(T item);
    bool add_back(T item);
    bool del_front();
    bool del_back();
    void print();
};



#endif //CSCI_460_HW2_PROJECT_LINKEDLIST_H
