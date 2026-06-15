#include <gtest/gtest.h>
#include "doubly_linked_list.hpp"
#include <string>

// ─── Tamaño y estado inicial ─────────────────────────────────────────────────

TEST(DoublyLinkedList, NuevaListaEstaVacia) {
    DoublyLinkedList<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0u);
}

// ─── pushBack / size ─────────────────────────────────────────────────────────

TEST(DoublyLinkedList, PushBackAumentaSize) {
    DoublyLinkedList<int> list;
    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    EXPECT_EQ(list.size(), 3u);
    EXPECT_FALSE(list.empty());
}

TEST(DoublyLinkedList, PushBackMantienOrden) {
    DoublyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    EXPECT_EQ(list.at(0), 1);
    EXPECT_EQ(list.at(1), 2);
    EXPECT_EQ(list.at(2), 3);
}

// ─── clear ───────────────────────────────────────────────────────────────────

TEST(DoublyLinkedList, ClearDejaSizeEnCero) {
    DoublyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.clear();
    EXPECT_EQ(list.size(), 0u);
    EXPECT_TRUE(list.empty());
}

TEST(DoublyLinkedList, ClearEnListaVaciaNoExplota) {
    DoublyLinkedList<int> list;
    EXPECT_NO_THROW(list.clear());
}

// ─── findIndexIf ─────────────────────────────────────────────────────────────

TEST(DoublyLinkedList, FindIndexIfEncuentraElemento) {
    DoublyLinkedList<int> list;
    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    int idx = list.findIndexIf([](const int& x) { return x == 20; });
    EXPECT_EQ(idx, 1);
}

TEST(DoublyLinkedList, FindIndexIfReturnaMenosUnoSiNoEncuentra) {
    DoublyLinkedList<int> list;
    list.pushBack(10);
    list.pushBack(20);
    int idx = list.findIndexIf([](const int& x) { return x == 99; });
    EXPECT_EQ(idx, -1);
}

TEST(DoublyLinkedList, FindIndexIfEnListaVaciaReturnaMenosUno) {
    DoublyLinkedList<int> list;
    int idx = list.findIndexIf([](const int& x) { return x == 1; });
    EXPECT_EQ(idx, -1);
}

// ─── removeFirstIf ───────────────────────────────────────────────────────────

TEST(DoublyLinkedList, RemoveFirstIfEliminaElementoDelMedio) {
    DoublyLinkedList<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    bool removed = list.removeFirstIf([](const int& x) { return x == 2; });
    EXPECT_TRUE(removed);
    EXPECT_EQ(list.size(), 2u);
    EXPECT_EQ(list.at(0), 1);
    EXPECT_EQ(list.at(1), 3);
}

TEST(DoublyLinkedList, RemoveFirstIfEliminaLaCabeza) {
    DoublyLinkedList<int> list;
    list.pushBack(10);
    list.pushBack(20);
    list.removeFirstIf([](const int& x) { return x == 10; });
    EXPECT_EQ(list.size(), 1u);
    EXPECT_EQ(list.at(0), 20);
}

TEST(DoublyLinkedList, RemoveFirstIfEliminaElUltimo) {
    DoublyLinkedList<int> list;
    list.pushBack(10);
    list.pushBack(20);
    list.removeFirstIf([](const int& x) { return x == 20; });
    EXPECT_EQ(list.size(), 1u);
    EXPECT_EQ(list.at(0), 10);
}

TEST(DoublyLinkedList, RemoveFirstIfRetornaFalsesSiNoEncuentra) {
    DoublyLinkedList<int> list;
    list.pushBack(1);
    bool removed = list.removeFirstIf([](const int& x) { return x == 99; });
    EXPECT_FALSE(removed);
    EXPECT_EQ(list.size(), 1u);
}

TEST(DoublyLinkedList, RemoveUnicoElementoDejaListaVacia) {
    DoublyLinkedList<int> list;
    list.pushBack(42);
    list.removeFirstIf([](const int& x) { return x == 42; });
    EXPECT_TRUE(list.empty());
}

// ─── sortRecursive (merge sort) ───────────────────────────────────────────────

TEST(DoublyLinkedList, SortRecursiveOrdenaNumerosAscendente) {
    DoublyLinkedList<int> list;
    list.pushBack(3);
    list.pushBack(1);
    list.pushBack(4);
    list.pushBack(1);
    list.pushBack(5);
    list.sortRecursive([](const int& a, const int& b) { return a < b; });
    EXPECT_EQ(list.at(0), 1);
    EXPECT_EQ(list.at(1), 1);
    EXPECT_EQ(list.at(2), 3);
    EXPECT_EQ(list.at(3), 4);
    EXPECT_EQ(list.at(4), 5);
}

TEST(DoublyLinkedList, SortRecursiveOrdenaStrings) {
    DoublyLinkedList<std::string> list;
    list.pushBack("zanahoria");
    list.pushBack("ajo");
    list.pushBack("cebolla");
    list.sortRecursive([](const std::string& a, const std::string& b) { return a < b; });
    EXPECT_EQ(list.at(0), "ajo");
    EXPECT_EQ(list.at(1), "cebolla");
    EXPECT_EQ(list.at(2), "zanahoria");
}

TEST(DoublyLinkedList, SortRecursiveListaVaciaNoExplota) {
    DoublyLinkedList<int> list;
    EXPECT_NO_THROW(
        list.sortRecursive([](const int& a, const int& b) { return a < b; })
    );
    EXPECT_EQ(list.size(), 0u);
}

TEST(DoublyLinkedList, SortRecursiveUnElementoNoExplota) {
    DoublyLinkedList<int> list;
    list.pushBack(42);
    EXPECT_NO_THROW(
        list.sortRecursive([](const int& a, const int& b) { return a < b; })
    );
    EXPECT_EQ(list.at(0), 42);
}

TEST(DoublyLinkedList, SortRecursiveMantieneSizeCorrect) {
    DoublyLinkedList<int> list;
    list.pushBack(5);
    list.pushBack(3);
    list.pushBack(8);
    list.pushBack(1);
    list.sortRecursive([](const int& a, const int& b) { return a < b; });
    EXPECT_EQ(list.size(), 4u);
}
