/**
 * EQUIPO 4
 * José Eduardo de Valle Lara - A01734957
 * Leonardo Corona Garza - A00832792
 * Raúl Kenyu Medina Takada - A01236331
 * Omar Damián Martínez Cordero - A00832849
 * César Eduardo Cortés Gaytán - A01198249
 * Kevin Susej Garza Aragón - A00833985
 */

#if defined(__APPLE__)
#define BITACORA \
  "/Users/leonardo/CLionProjects/Algoritmos/SP Graphs/bitacoraGrafos.txt"
#else
#define BITACORA "bitacoraGrafos.txt"
#endif

#include "HashTable.h"
#include "List.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

const int divFactor = 255255255255 / ARRAY_LENGTH;

/**
 * Función de hash que convierte la IP a entero y divide su valor
 * de forma que la IP más grande posible (255255255255) sea mapeada al valor de
 * ARRAY_LENGTH, que es igual a la cantidad de nodos en la bitácora.
 */
int ipHash(const std::string &key) {
  std::stringstream stream(key);
  std::string ipSegment;
  long long ipInt = 0;
  for (long long power = 1000000000; power != 0; power /= 1000) {
    getline(stream, ipSegment, '.');
    ipInt += std::stoi(ipSegment) * power;
  }
  return static_cast<int>(ipInt / divFactor);
}

/**
 * Estos objetos se almacenan como valores en la hash table. Contienen la IP
 * y la lista de intentos de conexión a esa ip. Esta lista contiene tanto las
 * IPs y la cantidad de intentos de esa IP. Se almacenan ambos en un mismo
 * objeto como std::pair.
 */
struct LogItem {
  std::string ip;
  // Lista de pares. Cada par contiene la IP y
  // la cuenta de intentos de acceso de esa IP
  List<std::pair<std::string, int>> connections;
  LogItem() = default;
  // ~LogItem() { this->connections.clear(); }
};

/**
 * Función especificada en el punto 3 de la actividad. Recibe la llave de la
 * entrada de bitácora deseada y despliega sus detalles. Por default, se
 * imprimen en std::cout, pero se le puede pasar un flile stream para escribirlo
 * en un archivo, como es el caso en main().
 *
 * @param table la hash table en donde se va a realizar la búsqueda.
 * Se debe pasar como referencia, es un objeto muy grande.
 * @param key la llave, que en este caso es la IP de la entrada de bitácora.
 * @param os el stream de salida. Por default es std::cout.
 */
void showIpInfo(const HashTable<LogItem> &table, const std::string &key,
                std::ostream &os = std::cout) {

  LogItem value = table.get(key, true, os);
  os << "IP: " << value.ip << std::endl << "Accesos:" << std::endl;
  std::pair<std::string, int> connection;
  while (value.connections.iterate(connection)) {
    os << connection.first << " " << connection.second
       << (connection.second == 1 ? " vez" : " veces") << std::endl;
  }
  os << std::endl;
}

/**
 * Lee el archivo de bitácora y coloca en una hash table cada entrada. Se
 * almacenan las IPs y los intentos de conexión a esa IP con objetos de LogItem.
 * Al final registra todos estos elementos de la hash table en un archivo de
 * salida.
 */
int main() {
  std::cout << "Inicializando tabla" << std::endl;
  HashTable<LogItem> table(ipHash);

  std::cout << "Leyendo archivo" << std::endl;
  std::fstream stream(BITACORA, std::ios::in);
  std::string line;
  std::string ip1;
  std::string ip2;
  int lineCounter = 0;
  int nodeCount;
  bool alreadyExists;

  // Omitir los encabezados del archivo. Esto incluye posibles comentarios
  // y después la cantidad de elementos.
  while (getline(stream, line)) {
    if (line.substr(0, 1) != "%") {
      std::stringstream sstream(line);
      std::string countStr;
      getline(sstream, countStr, ' ');
      nodeCount = std::stoi(countStr); // guardar la cantidad de elementos
                                       // para el siguiente while
      break;
    }
  }

  // Almacenar los elementos (primera mitad de la bitácora)
  while (getline(stream, line)) {
    if (line.substr(0, 1) == "%") {
      continue;
    }

    LogItem value;
    value.ip = line;
    table.insert(line, value);

    if (++lineCounter == nodeCount) {
      break;
    }
  }

  // Almacenar los intentos de conexión en cada elemento (segunda mitad)
  std::cout << "Agregando intentos de conexión" << std::endl;
  while (getline(stream, line)) {
    if (line.substr(0, 1) == "%") {
      continue;
    }

    std::stringstream sstream(line);
    for (int i = 0; i < 3; ++i) {
      getline(sstream, ip1, ' '); // truncar todo antes de la IP
    }
    getline(sstream, ip1, ':'); // truncar el puerto
    getline(sstream, ip2, ' ');
    getline(sstream, ip2, ':');

    LogItem value = table.get(ip1);

    // Revisar si ya existe esa IP en los intentos de conexión.
    // Se debe iterar en la lista de value.connections. Se tuvieron que usar
    // punteros a los nodos para poder sumar directamente 1 a la cantidad de
    // intentos
    alreadyExists = false;
    ListNode<std::pair<std::string, int>> *node;
    while (value.connections.iterate(node)) {
      auto &connection = node->value;
      if (connection.first == ip2) { // revisar si esa IP ya existe
        connection.second++;         // sumar uno a la cuenta
        alreadyExists = true;
        value.connections.resetIterator();
        break;
      }
    }

    // Si no existe esa IP, agregarla.
    if (!alreadyExists) {
      value.connections.push(std::make_pair(ip2, 1));
    }

    table.update(ip1, value);
  }

  stream.close();

  // Recorrer hash table y llamar showIpInfo()
  std::cout << "Guardando nueva bitácora" << std::endl;
  std::fstream output("salida.txt", std::ios::out);
  auto cells = table.getCells();
  for (int i = 0; i < ARRAY_LENGTH; ++i) {
    if (cells[i].hasValue) {
      showIpInfo(table, cells[i].key, output);
    }
  }

  std::cout << "Guardado en 'salida.txt'" << std::endl;
  output.close();

  std::string ipRequest;
  while (true) {
    std::cout << "Teclea una IP para buscar y desplegar detalles:" << std::endl;
    std::cout << "(presiona enter para salir)" << std::endl;
    std::cin >> ipRequest;

    if (ipRequest == "") {
      break;
    }

    showIpInfo(table, ipRequest);
  }

  return 0;
}
