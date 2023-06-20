/**
 * EQUIPO 4
 * José Eduardo de Valle Lara - A01734957
 * Leonardo Corona Garza - A00832792
 * Raúl Kenyu Medina Takada - A01236331
 * Omar Damián Martínez Cordero - A00832849
 * César Eduardo Cortés Gaytán - A01198249
 * Kevin Susej Garza Aragón - A00833985
 */

#ifndef ALGORITMOS_HASHTABLE_H
#define ALGORITMOS_HASHTABLE_H

#include <functional>
#include <iostream>
#include <string>
#include <utility>

// Esta cantidad se puede hacer más corta o más larga para probar la búsqueda de
// espacios vacíos en las tablas de hash cerradas. En este momento está en su
// valor mínimo: la cantidad de elementos de la bitácora.
const int ARRAY_LENGTH = 13370;

/**
 * Este objeto se coloca en cada celda de la HashTable. Es casi como una
 * tupla, que solamente almacena la llave, el valor, y el booleano hasValue.
 */
template <class T> struct HashCell {
  std::string key;
  T value;
  bool hasValue;

  HashCell() : hasValue(false){};
};

using HashFunc = std::function<int(std::string)>; // hacer el código más legible

/**
 * Tabla de hash. Esta estructura resuelve los conflictos de hash buscando en
 * índices subsiguientes del arreglo hasto encontrar un lugar vacío. Cuando se
 * llega al final del arreglo, darle la vuelta y seguir en el índice cero.
 * Cuando se inicializa una tabla, se debe pasar como objeto la función hash que
 * se usará para convertir una llave a un índice.
 */
template <class T> class HashTable {
private:
  HashCell<T> cells[ARRAY_LENGTH];
  HashFunc hashFunc; // función hash guardada como objeto
                     // debe aceptar un string y regresar un int
public:
  explicit HashTable(HashFunc hashFunc)
      : hashFunc(std::move(hashFunc)) {
  } // el constructor recibe la función hash

  HashCell<T> *getCells() { return this->cells; }

  /**
   * Regresa un valor, con una llave de entrada. Complejidad: O(1 + n/m)
   *
   * @param key string de la llave del objeto deseado.
   * @param verbose pasar true para imprimir detalles sobre el índice del
   * objeto. Por default es false.
   * @param os si verbose es true, qué output stream utilizar. Por default es
   * std::cout.
   * @return El valor que corresponde con la llave recibida.
   */
  T get(const std::string &key, bool verbose = false,
        std::ostream &os = std::cout) const {

    if (verbose) {
      os << "Llave: " << key << std::endl;
    }

    int index = this->hashFunc(key) % ARRAY_LENGTH;
    int ogIndex = index;
    if (verbose) {
      os << "Índice hash: " << index << std::endl;
    }

    while (this->cells[index].key != key) {
      index = (index + 1) % ARRAY_LENGTH;
      
      // Checar si ya dio una vuelta completa.
      if (index == ogIndex) {
        std::cout << "No se encontró elemento con llave " << key << std::endl;
        throw std::exception();
      }
    }
    if (verbose) {
      os << "Índice real: " << index << std::endl;
    }

    return this->cells[index].value;
  }

  /** Inserta un nuevo par de llave y valor. Complejidad: (1 + n/m) */
  void insert(const std::string &key, T value) {
    int index = this->hashFunc(key) % ARRAY_LENGTH;
    int ogIndex = index;

    while (this->cells[index].hasValue) {
      index = (index + 1) % ARRAY_LENGTH;

      // Checar si ya dio una vuelta completa.
      if (index == ogIndex) {
        std::cout << "No se encontró elemento con llave " << key << std::endl;
        throw std::exception();
      }
    }

    this->cells[index].key = key;
    this->cells[index].value = value;
    this->cells[index].hasValue = true;
  }

  /** Elimina un elemento, según la llave de entrada. Complejidad: O(1 + n/m) */
  void remove(const std::string &key) {
    int index = this->hashFunc(key) % ARRAY_LENGTH;

    while (this->cells[index].key != key) {
      index = (index + 1) % ARRAY_LENGTH;
    }

    this->cells[index].key = std::string();
    this->cells[index].value = T();
    this->cells[index].hasValue = false;
  }

  /** Cambiar el valor de un elemento ya existente. Complejidad: O(1 + n/m) */
  void update(const std::string &key, T newValue) {
    int index = this->hashFunc(key) % ARRAY_LENGTH;

    while (this->cells[index].key != key) {
      index = (index + 1) % ARRAY_LENGTH;
    }

    this->cells[index].value = newValue;
  }
};

#endif // ALGORITMOS_HASHTABLE_H
