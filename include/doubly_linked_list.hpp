#ifndef DOUBLY_LINKED_LIST_HPP
#define DOUBLY_LINKED_LIST_HPP

#include <cstddef>
#include <functional>

// Nodo doblemente ligado genérico
template<typename T>
struct DNode {
    T data;
    DNode* prev;
    DNode* next;

    explicit DNode(const T& value) : data(value), prev(nullptr), next(nullptr) {}
};

// Lista doblemente ligada genérica
template<typename T>
class DoublyLinkedList {
protected:
    DNode<T>* head;
    DNode<T>* tail;
    std::size_t sz;

    // --- Helpers para merge sort recursivo ---
    static DNode<T>* split(DNode<T>* head) {
        DNode<T>* fast = head;
        DNode<T>* slow = head;
        while (fast && fast->next && fast->next->next) {
            fast = fast->next->next;
            slow = slow->next;
        }
        DNode<T>* second = slow->next;
        if (second) second->prev = nullptr;
        slow->next = nullptr;
        return second;
    }

    static DNode<T>* mergeLists(DNode<T>* a, DNode<T>* b,
                                std::function<bool(const T&, const T&)> comp) {
        if (!a) return b;
        if (!b) return a;

        if (comp(a->data, b->data)) {
            a->next = mergeLists(a->next, b, comp);
            if (a->next) a->next->prev = a;
            a->prev = nullptr;
            return a;
        } else {
            b->next = mergeLists(a, b->next, comp);
            if (b->next) b->next->prev = b;
            b->prev = nullptr;
            return b;
        }
    }

    static DNode<T>* mergeSortRecursive(DNode<T>* node,
                                        std::function<bool(const T&, const T&)> comp) {
        if (!node || !node->next) return node;
        DNode<T>* second = split(node);
        node = mergeSortRecursive(node, comp);
        second = mergeSortRecursive(second, comp);
        return mergeLists(node, second, comp);
    }

    void rebuildTailFromHead() {
        tail = head;
        while (tail && tail->next) {
            tail = tail->next;
        }
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), sz(0) {}

    ~DoublyLinkedList() {
        clear();
    }

    bool empty() const { return head == nullptr; }
    std::size_t size() const { return sz; }

    void clear() {
        DNode<T>* curr = head;
        while (curr) {
            DNode<T>* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
        head = tail = nullptr;
        sz = 0;
    }

    void pushBack(const T& value) {
        DNode<T>* node = new DNode<T>(value);
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        ++sz;
    }

    T& at(std::size_t index) {
        DNode<T>* curr = head;
        for (std::size_t i = 0; i < index && curr; ++i) {
            curr = curr->next;
        }
        return curr->data;
    }

    const T& at(std::size_t index) const {
        DNode<T>* curr = head;
        for (std::size_t i = 0; i < index && curr; ++i) {
            curr = curr->next;
        }
        return curr->data;
    }

    int findIndexIf(std::function<bool(const T&)> pred) const {
        DNode<T>* curr = head;
        int idx = 0;
        while (curr) {
            if (pred(curr->data)) return idx;
            curr = curr->next;
            ++idx;
        }
        return -1;
    }

    bool removeFirstIf(std::function<bool(const T&)> pred) {
        DNode<T>* curr = head;
        while (curr && !pred(curr->data)) {
            curr = curr->next;
        }
        if (!curr) return false;

        if (curr->prev) curr->prev->next = curr->next;
        else head = curr->next;

        if (curr->next) curr->next->prev = curr->prev;
        else tail = curr->prev;

        delete curr;
        --sz;
        return true;
    }

    // Ordenamiento recursivo con comparador explícito
    void sortRecursive(std::function<bool(const T&, const T&)> comp) {
        if (!head || !head->next) return;
        head = mergeSortRecursive(head, comp);
        rebuildTailFromHead();
    }
};

#endif // DOUBLY_LINKED_LIST_HPP