#include <gtest/gtest.h>
#include "singly_linked_list.hpp"
#include <string>

auto strComp = [](const std::string& a, const std::string& b) { return a < b; };
auto intComp = [](const int& a, const int& b) { return a < b; };

// ─── Estado inicial ───────────────────────────────────────────────────────────

TEST(SinglyLinkedList, NuevaListaEstaVacia) {
    SinglyLinkedList<int> list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0u);
}

// ─── pushFront ────────────────────────────────────────────────────────────────

TEST(SinglyLinkedList, PushFrontInsertaAlInicio) {
    SinglyLinkedList<int> list;
    list.pushFront(1);
    list.pushFront(2);
    EXPECT_EQ(list.at(0), 2);
    EXPECT_EQ(list.at(1), 1);
    EXPECT_EQ(list.size(), 2u);
}

// ─── insertOrdered ───────────────────────────────────────────────────────────

TEST(SinglyLinkedList, InsertOrderedMantienOrdenAlfabetico) {
    SinglyLinkedList<std::string> list;
    list.insertOrdered("zanahoria", strComp);
    list.insertOrdered("ajo",       strComp);
    list.insertOrdered("cebolla",   strComp);
    EXPECT_EQ(list.at(0), "ajo");
    EXPECT_EQ(list.at(1), "cebolla");
    EXPECT_EQ(list.at(2), "zanahoria");
}

TEST(SinglyLinkedList, InsertOrderedNumerosAscendente) {
    SinglyLinkedList<int> list;
    list.insertOrdered(5, intComp);
    list.insertOrdered(1, intComp);
    list.insertOrdered(3, intComp);
    EXPECT_EQ(list.at(0), 1);
    EXPECT_EQ(list.at(1), 3);
    EXPECT_EQ(list.at(2), 5);
}

TEST(SinglyLinkedList, InsertOrderedElementoMenorQueCabeza) {
    SinglyLinkedList<int> list;
    list.insertOrdered(10, intComp);
    list.insertOrdered(5,  intComp);
    EXPECT_EQ(list.at(0), 5);
    EXPECT_EQ(list.at(1), 10);
}

TEST(SinglyLinkedList, InsertOrderedElementoMayorQueColaVaAlFinal) {
    SinglyLinkedList<int> list;
    list.insertOrdered(1, intComp);
    list.insertOrdered(2, intComp);
    list.insertOrdered(99, intComp);
    EXPECT_EQ(list.at(2), 99);
}

TEST(SinglyLinkedList, InsertOrderedDuplicadosSeInsertan) {
    SinglyLinkedList<int> list;
    list.insertOrdered(5, intComp);
    list.insertOrdered(5, intComp);
    EXPECT_EQ(list.size(), 2u);
}

// ─── clear ───────────────────────────────────────────────────────────────────

TEST(SinglyLinkedList, ClearVaciaLaLista) {
    SinglyLinkedList<int> list;
    list.pushFront(1);
    list.pushFront(2);
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0u);
}

// ─── removeFirstIf ───────────────────────────────────────────────────────────

TEST(SinglyLinkedList, RemoveFirstIfEliminaElementoDelMedio) {
    SinglyLinkedList<int> list;
    list.insertOrdered(1, intComp);
    list.insertOrdered(2, intComp);
    list.insertOrdered(3, intComp);
    bool removed = list.removeFirstIf([](const int& x) { return x == 2; });
    EXPECT_TRUE(removed);
    EXPECT_EQ(list.size(), 2u);
    EXPECT_EQ(list.at(0), 1);
    EXPECT_EQ(list.at(1), 3);
}

TEST(SinglyLinkedList, RemoveFirstIfEliminaCabeza) {
    SinglyLinkedList<int> list;
    list.insertOrdered(1, intComp);
    list.insertOrdered(2, intComp);
    list.removeFirstIf([](const int& x) { return x == 1; });
    EXPECT_EQ(list.at(0), 2);
    EXPECT_EQ(list.size(), 1u);
}

TEST(SinglyLinkedList, RemoveFirstIfRetornaFalseSiNoEncuentra) {
    SinglyLinkedList<int> list;
    list.insertOrdered(1, intComp);
    bool removed = list.removeFirstIf([](const int& x) { return x == 99; });
    EXPECT_FALSE(removed);
    EXPECT_EQ(list.size(), 1u);
}

TEST(SinglyLinkedList, RemoveUnicoElementoDejaListaVacia) {
    SinglyLinkedList<std::string> list;
    list.pushFront("ajo");
    list.removeFirstIf([](const std::string& s) { return s == "ajo"; });
    EXPECT_TRUE(list.empty());
}

// ─── forEach ─────────────────────────────────────────────────────────────────

TEST(SinglyLinkedList, ForEachRecorreTodosLosElementos) {
    SinglyLinkedList<int> list;
    list.insertOrdered(1, intComp);
    list.insertOrdered(2, intComp);
    list.insertOrdered(3, intComp);
    int suma = 0;
    list.forEach([&suma](const int& x) { suma += x; });
    EXPECT_EQ(suma, 6);
}

TEST(SinglyLinkedList, ForEachMutabileModificaElementos) {
    SinglyLinkedList<int> list;
    list.pushFront(1);
    list.pushFront(2);
    list.forEach([](int& x) { x *= 10; });
    // Los valores deben haberse multiplicado
    int suma = 0;
    list.forEach([&suma](const int& x) { suma += x; });
    EXPECT_EQ(suma, 30); // (2*10) + (1*10) = 30
}
