#ifndef SINGLY_LINKED_LIST_HPP
#define SINGLY_LINKED_LIST_HPP

#include <cstddef>
#include <functional>

// Nodo simplemente ligado genérico
template<typename T>
struct SNode {
    T data;
    SNode* next;

    explicit SNode(const T& value) : data(value), next(nullptr) {}
};

// Lista simplemente ligada genérica
template<typename T>
class SinglyLinkedList {
private:
    SNode<T>* head;
    std::size_t sz;

public:
    SinglyLinkedList() : head(nullptr), sz(0) {}

    ~SinglyLinkedList() {
        clear();
    }

    bool empty() const { return head == nullptr; }
    std::size_t size() const { return sz; }

    void clear() {
        SNode<T>* curr = head;
        while (curr) {
            SNode<T>* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
        head = nullptr;
        sz = 0;
    }

    // Inserta al inicio (no ordenado)
    void pushFront(const T& value) {
        SNode<T>* node = new SNode<T>(value);
        node->next = head;
        head = node;
        ++sz;
    }

    // Inserción ORDENADA usando comparador explícito (comp(a,b) = a<b)
    bool insertOrdered(const T& value, std::function<bool(const T&, const T&)> comp) {
        SNode<T>* node = new SNode<T>(value);

        // Lista vacía o va antes de la cabeza
        if (!head || comp(value, head->data)) {
            node->next = head;
            head = node;
            ++sz;
            return true;
        }

        // Buscar posición donde value va antes de curr->next
        SNode<T>* curr = head;
        while (curr->next && !comp(value, curr->next->data)) {
            curr = curr->next;
        }

        node->next = curr->next;
        curr->next = node;
        ++sz;
        return true;
    }

    // Elimina el primer elemento que cumpla el predicado
    bool removeFirstIf(std::function<bool(const T&)> pred) {
        if (!head) return false;

        if (pred(head->data)) {
            SNode<T>* tmp = head;
            head = head->next;
            delete tmp;
            --sz;
            return true;
        }

        SNode<T>* curr = head;
        while (curr->next && !pred(curr->next->data)) {
            curr = curr->next;
        }
        if (!curr->next) return false;

        SNode<T>* tmp = curr->next;
        curr->next = tmp->next;
        delete tmp;
        --sz;
        return true;
    }

    // Acceso por índice (O(n), pero suficiente para este proyecto)
    T& at(std::size_t index) {
        SNode<T>* curr = head;
        for (std::size_t i = 0; i < index && curr; ++i) {
            curr = curr->next;
        }
        return curr->data; // asume que index es válido
    }

    const T& at(std::size_t index) const {
        SNode<T>* curr = head;
        for (std::size_t i = 0; i < index && curr; ++i) {
            curr = curr->next;
        }
        return curr->data;
    }

    // Recorrido aplicando función a cada elemento
    void forEach(std::function<void(T&)> fn) {
        SNode<T>* curr = head;
        while (curr) {
            fn(curr->data);
            curr = curr->next;
        }
    }

    void forEach(std::function<void(const T&)> fn) const {
        SNode<T>* curr = head;
        while (curr) {
            fn(curr->data);
            curr = curr->next;
        }
    }
};

#endif // SINGLY_LINKED_LIST_HPP