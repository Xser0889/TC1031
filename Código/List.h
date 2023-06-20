/**
 * EQUIPO 4
 * José Eduardo de Valle Lara - A01734957
 * Leonardo Corona Garza - A00832792
 * Raúl Kenyu Medina Takada - A01236331
 * Omar Damián Martínez Cordero - A00832849
 * César Eduardo Cortés Gaytán - A01198249
 * Kevin Susej Garza Aragón - A00833985
 */

#ifndef ALGORITMOS_LIST_H
#define ALGORITMOS_LIST_H

#include <iostream>

/** Nodo de lista enlazada genérica. */
template <class T> struct ListNode {
  T value;
  ListNode *prev;
  ListNode *next;

  ListNode() : value(T()), prev(nullptr), next(nullptr) {}
  explicit ListNode(T value) : value(value), prev(nullptr), next(nullptr) {}
};

/** Lista enlazada genérica */
template <class T> class List {
private:
  ListNode<T> *first;
  ListNode<T> *last;
  ListNode<T> *iter; // este atributo sirve para poder guardar el elemento
                     // actual en una iteración.
  int size;

public:
  List() : first(nullptr), last(nullptr), iter(nullptr), size(0) {}

  //    ~List() { this->clear(); } // el destructor dejó de funcionar :(

  /** Elimina todos los elementos de la lista y libera la memoria. */
  void clear() {
    ListNode<T> *node = this->first;
    while (node != nullptr) {
      ListNode<T> *aux = node;
      node = node->next;
      delete aux;
    }
    this->first = nullptr;
    this->last = nullptr;
    this->iter = nullptr;
    this->size = 0;
  }

  /**
   * Método que permite iterar completamente con un simple while.
   * Si la iteración termina antes de lo normal, se debe llamar la función
   * resetIterator(). Como la función getline(), este método requiere pasar
   * un elemento por referencia, al que se le serán asignados los valores
   * de la iteración.
   *
   * Ejemplo:
   *
   * std::string x;
   * while (list.iterate(x)) {
   *   std::cout << x << std::endl;
   *   ...
   * }
   *
   * @param inoutElement se asignan los elementos de la iteración a esta
   * variable.
   * @return true hasta que termine la iteración, para poder salirse del while
   * que lo contiene.
   */
  bool iterate(T &inoutElement) {
    if (this->iter == nullptr) {
      this->iter = this->first;
    } else {
      this->iter = this->iter->next;
    }

    if (this->iter == nullptr) {
      return false;
    }

    inoutElement = this->iter->value;
    return true;
  }

  bool iterate(ListNode<T> *&inoutNode) {
    if (this->iter == nullptr) {
      this->iter = this->first;
    } else {
      this->iter = this->iter->next;
    }

    if (this->iter == nullptr) {
      return false;
    }

    inoutNode = this->iter;
    return true;
  }

  void resetIterator() { this->iter = nullptr; }

  /**
   * Insertar un elemento. Esta función permite que List funcione
   * también como Stack y Queue.
   */
  void push(T value) {
    auto newNode = new ListNode<T>(value);

    if (this->first == nullptr || this->last == nullptr) {
      this->first = newNode;
      this->last = newNode;
      return;
    }

    auto *lastNode = this->last;
    this->last = newNode;
    newNode->prev = lastNode;
    lastNode->next = newNode;
    this->size++;
  }
};

#endif // ALGORITMOS_LIST_H
