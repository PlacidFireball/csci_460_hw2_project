//
// Created by placidfireball on 9/29/21.
//

#include <iostream>
#include "LinkedList.h"
template<typename T>
LinkedList<T>::~LinkedList() = default;

template<typename T>
LinkedList<T>::LinkedList() {
    len = 0;
    for (auto & node : nodes) node = Node();
    head = Node(-1, &dummy_node, &tail);
    head.id = "HEAD";
    tail = Node(-1, &head, &dummy_node);
    tail.id = "TAIL";
}

template<typename T>
bool LinkedList<T>::add_front(T item) {
    if (len < MAX_LEN) {
        nodes[num_nodes] = Node(item, &dummy_node, &dummy_node);;
        nodes[num_nodes].prev = &head;
        nodes[num_nodes].next = head.next;
        head.next->prev = &nodes[num_nodes];
        head.next = &nodes[num_nodes];
        num_nodes++;
        len++;
        return true;
    }
    return false;
}

template<typename T>
bool LinkedList<T>::add_back(T item) {
    if (len < MAX_LEN) {
        nodes[num_nodes] = Node(item, &dummy_node, &dummy_node);
        nodes[num_nodes].next = &tail;
        nodes[num_nodes].prev = tail.prev;
        tail.prev->next = &nodes[num_nodes];
        tail.prev = &nodes[num_nodes];
        num_nodes++;
        len++;
        return true;
    }
    return false;
}

template<typename T>
bool LinkedList<T>::del_front() {
    Node* first = head.next;
    head.next = first->next;
    head.next->prev = &head;
    len--;
    return true;
}

template<typename T>
bool LinkedList<T>::del_back() {
    Node* last = tail.prev;
    tail.prev = last->prev;
    tail.prev->next = &tail;
    len--;
    return true;
}

template<typename T>
void LinkedList<T>::print() {
    Node* curr_node = head.next;
    std::cout << "[";
    while(curr_node->data != -1) {
        std::cout << curr_node->data;
        if (curr_node->next->data != -1) std::cout << ", ";
        curr_node = curr_node->next;
    }
    std::cout << "]" << std::endl;
}

template<typename T>
LinkedList<T>::Node::Node(T d, LinkedList::Node *previous_node, LinkedList::Node *next_node) {
    data = d;
    prev = previous_node;
    next = next_node;
}

template<typename T>
LinkedList<T>::Node::Node() {
    data = -1;
}

