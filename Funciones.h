#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// Estructura para cada nodo del árbol genealógico
struct Nodo {
  std::string nombre;
  int anioNacimiento;
  Nodo *madre;
  Nodo *padre;
  std::set<Nodo *> hijos;

  Nodo(const std::string &nombre, int anioNacimiento) : nombre(nombre), anioNacimiento(anioNacimiento), madre(nullptr), padre(nullptr) {
  }
};

// Crear un nuevo nodo y añadirlo al conjunto de miembros
Nodo *crearNodo(std::set<Nodo *> &miembros, const std::string &nombre, int anioNacimiento) {
  Nodo *nuevo = new Nodo(nombre, anioNacimiento);
  miembros.insert(nuevo);
  return nuevo;
}

// Buscar un miembro en el conjunto de nodos
Nodo *buscarMiembro(std::set<Nodo *> &miembros, const std::string &nombre) {
  for (Nodo *nodo : miembros) {
    if (nodo->nombre == nombre) return nodo;
  }
  return nullptr;
}

// Funcion para limpiar terminal
void limpiarTerminal() {
#ifdef _WIN32
  system("cls");  // windows
#else
  system("clear");  // mac
#endif
}

// Función para verificar si un número ingresado es válido
bool verificarEntrada(int &numero) {
  if (std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;  // Entrada inválida
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return true;  // Entrada válida
}

// Función para agregar un miembro al árbol
void agregarMiembro(std::set<Nodo *> &miembros, const std::string &nombre, const std::string &madre, const std::string &padre, const int anioNacimiento) {
  Nodo *miembro = buscarMiembro(miembros, nombre);
  if (!miembro) {
    miembro = crearNodo(miembros, nombre, anioNacimiento);
  }

  Nodo *nodoMadre = nullptr;
  if (!madre.empty())  // Verifica si madre no está vacío
  {
    nodoMadre = buscarMiembro(miembros, madre);
    if (!nodoMadre) {
      int anioNacimientoMadre;
      while (true) {
        std::cout << "Ingrese el anio de nacimiento de la madre (" << madre << "): ";
        std::cin >> anioNacimientoMadre;
        if (!verificarEntrada(anioNacimientoMadre)) {
          std::cout << "Entrada no valida. Por favor, ingrese un numero.\n";
        } else {
          break;  // Salir del bucle si la entrada es válida
        }
      }
      nodoMadre = crearNodo(miembros, madre, anioNacimientoMadre);
    }
  }

  Nodo *nodoPadre = nullptr;
  if (!padre.empty())  // Verifica si padre no está vacío
  {
    nodoPadre = buscarMiembro(miembros, padre);
    if (!nodoPadre) {
      int anioNacimientoPadre;
      while (true) {
        std::cout << "Ingrese el anio de nacimiento del padre (" << padre << "): ";
        std::cin >> anioNacimientoPadre;
        if (!verificarEntrada(anioNacimientoPadre)) {
          std::cout << "Entrada no valida. Por favor, ingrese un numero.\n";
        } else {
          break;  // Salir del bucle si la entrada es válida
        }
      }
      nodoPadre = crearNodo(miembros, padre, anioNacimientoPadre);
    }
  }

  miembro->madre = nodoMadre;
  miembro->padre = nodoPadre;

  if (nodoMadre) {
    nodoMadre->hijos.insert(miembro);
  }
  if (nodoPadre) {
    nodoPadre->hijos.insert(miembro);
  }
  limpiarTerminal();
  std::cout << "Miembro " << nombre << " agregado exitosamente con madre " << (madre.empty() ? "N/A" : madre) << " y padre " << (padre.empty() ? "N/A" : padre) << ".\n";
}

// Obtener el año actual
int obtenerAnioActual() {
  time_t t = time(0);
  struct tm *now = localtime(&t);
  return now->tm_year + 1900;
}

// Calcular la edad
int calcularEdad(int anioNacimiento) {
  int anioActual = obtenerAnioActual();
  return anioActual - anioNacimiento;
}

// Función para ordenar los miembros por edad
std::vector<Nodo *> ordenarMiembrosPorEdad(const std::set<Nodo *> &miembros) {
  std::vector<Nodo *> ordenados(miembros.begin(), miembros.end());
  std::sort(ordenados.begin(), ordenados.end(), [](Nodo *a, Nodo *b) { return calcularEdad(a->anioNacimiento) > calcularEdad(b->anioNacimiento); });
  return ordenados;
}

// Imprimir los miembros disponibles ordenados por edad
void imprimirMiembrosOrdenados(const std::set<Nodo *> &miembros) {
  std::vector<Nodo *> miembrosOrdenados = ordenarMiembrosPorEdad(miembros);
  for (Nodo *miembro : miembrosOrdenados) {
    std::cout << "  - " << miembro->nombre << " (" << calcularEdad(miembro->anioNacimiento) << " anios)\n";
  }
}

// Función para eliminar un miembro del árbol
void eliminarMiembro(std::set<Nodo *> &miembros, const std::string &nombre) {
  Nodo *miembro = buscarMiembro(miembros, nombre);
  if (!miembro) {
    std::cout << "El miembro no existe.\n";
    return;
  }

  // Quitar al miembro de los hijos de su madre y padre
  if (miembro->madre) miembro->madre->hijos.erase(miembro);
  if (miembro->padre) miembro->padre->hijos.erase(miembro);

  // Quitar referencias de hijos hacia este miembro
  for (Nodo *hijo : miembro->hijos) {
    if (hijo->madre == miembro) hijo->madre = nullptr;
    if (hijo->padre == miembro) hijo->padre = nullptr;
  }

  // Eliminar el nodo del conjunto
  miembros.erase(miembro);
  delete miembro;

  std::cout << "Miembro eliminado exitosamente.\n";
}

// Función Buscar miembro de la familia
void buscarMiembroFamiliar(std::set<Nodo *> &miembros) {
  std::string nombre;
  std::cout << "Ingrese el nombre del miembro a buscar: ";
  std::cin >> nombre;
  if (Nodo *miembro = buscarMiembro(miembros, nombre)) {
    std::cout << "\nMiembro encontrado: " << miembro->nombre << " (nacido en " << miembro->anioNacimiento << ", edad " << calcularEdad(miembro->anioNacimiento) << " anios)\n";
  } else {
    std::cout << "\nMiembro no encontrado\n";
  }
}

// Imprimir las relaciones directas del miembro
void imprimirRelacionDirecta(Nodo *miembro) {
  if (miembro == nullptr) {
    std::cout << "Miembro no encontrado.\n";
    return;
  }

  int edad = calcularEdad(miembro->anioNacimiento);
  std::cout << "Miembro: " << miembro->nombre << " (" << edad << " anios)\n";

  if (miembro->madre) {
    int edadMadre = calcularEdad(miembro->madre->anioNacimiento);
    std::cout << "  Madre: " << miembro->madre->nombre << " (" << edadMadre << " anios)\n";
  }

  if (miembro->padre) {
    int edadPadre = calcularEdad(miembro->padre->anioNacimiento);
    std::cout << "  Padre: " << miembro->padre->nombre << " (" << edadPadre << " anios)\n";
  }

  if (!miembro->hijos.empty()) {
    std::cout << "  Hijos: ";
    for (Nodo *hijo : miembro->hijos) {
      int edadHijo = calcularEdad(hijo->anioNacimiento);
      std::cout << hijo->nombre << " (" << edadHijo << " anios), ";
    }
    std::cout << std::endl;
  }
}
// Imprimir el árbol genealógico recursivamente (completo)

void imprimirArbol(Nodo *nodo, int nivel = 0) {
  if (nodo == nullptr) return;

  for (int i = 0; i < nivel; ++i) std::cout << "  ";

  int edad = calcularEdad(nodo->anioNacimiento);
  std::cout << nodo->nombre << " (" << edad << " anios)\n";

  for (Nodo *hijo : nodo->hijos) {
    imprimirArbol(hijo, nivel + 1);
  }
}

// Función para solicitar opcion
int solicitarOpcion() {
  int opcion;
  while (true) {
    std::cout << "Ingrese la accion que desea realizar: \n";
    std::cout << "(1) Importar Arbol\n";
    std::cout << "(2) Exportar Arbol\n";
    std::cin >> opcion;
    if (!verificarEntrada(opcion)) {
      std::cout << "Entrada no valida. Por favor, ingrese un numero.\n";
    } else {
      return opcion;
      break;  // Salir del bucle si la entrada es válida
    }
  }
}

// Función para importar el árbol desde un archivo
void importarArbolGenealogico(std::set<Nodo *> &miembros, const std::string &nombreArchivo) {
  std::ifstream archivo(nombreArchivo);

  if (!archivo.is_open()) {
    std::cout << "Error al abrir el archivo para importar.\n";
    return;
  }

  std::string linea;
  bool primeraLinea = true;

  // Leer cada línea del archivo CSV
  while (std::getline(archivo, linea)) {
    // Ignorar la primera línea (encabezado)
    if (primeraLinea) {
      primeraLinea = false;
      continue;
    }

    std::stringstream ss(linea);
    std::string padre, hijo, anioStr;
    int anioNacimiento = 0;

    // Leer los valores separados por coma
    std::getline(ss, padre, ',');
    std::getline(ss, hijo, ',');
    std::getline(ss, anioStr, ',');

    // Convertir el año de nacimiento a entero si no está vacío
    if (!anioStr.empty()) {
      anioNacimiento = std::stoi(anioStr);
    }

    // Buscar o crear el nodo hijo
    Nodo *nodoHijo = buscarMiembro(miembros, hijo);
    if (!nodoHijo) {
      nodoHijo = crearNodo(miembros, hijo, anioNacimiento);
    } else if (nodoHijo->anioNacimiento == 0)  // Actualizar año de nacimiento si no estaba definido
    {
      nodoHijo->anioNacimiento = anioNacimiento;
    }

    // Si hay un padre/madre definido, buscar o crear el nodo padre/madre
    if (!padre.empty()) {
      Nodo *nodoPadreMadre = buscarMiembro(miembros, padre);
      if (!nodoPadreMadre) {
        nodoPadreMadre = crearNodo(miembros, padre, 0);  // Año de nacimiento desconocido
      }

      // Actualizar relaciones
      if (!nodoHijo->madre)  // Si no tiene madre, asignarlo como tal
      {
        nodoHijo->madre = nodoPadreMadre;
        nodoPadreMadre->hijos.insert(nodoHijo);
      } else if (!nodoHijo->padre)  // Si no tiene padre, asignarlo como tal
      {
        nodoHijo->padre = nodoPadreMadre;
        nodoPadreMadre->hijos.insert(nodoHijo);
      }
    }
  }

  archivo.close();
  std::cout << "Archivo actualizado exitosamente: " << nombreArchivo << ".\n";
}

// Función para exportar el árbol a un archivo csv
void exportarArbolGenealogico(const std::set<Nodo *> &miembros, const std::string &nombreArchivo) {
  std::ofstream archivo(nombreArchivo);

  if (!archivo.is_open()) {
    std::cout << "Error al abrir el archivo para exportar.\n";
    return;
  }

  // Escribir encabezados
  archivo << "padres,hijos,año\n";

  // Procesar cada nodo en el conjunto de miembros
  for (Nodo *miembro : miembros) {
    // Registrar relación madre-hijo
    if (miembro->madre) {
      archivo << miembro->madre->nombre << "," << miembro->nombre << "," << miembro->anioNacimiento << "\n";
    } else {
      archivo << "," << miembro->nombre << "," << miembro->anioNacimiento << "\n";  // Sin madre
    }

    // Registrar relación padre-hijo
    if (miembro->padre) {
      archivo << miembro->padre->nombre << "," << miembro->nombre << "," << miembro->anioNacimiento << "\n";
    } else {
      archivo << "," << miembro->nombre << "," << miembro->anioNacimiento << "\n";  // Sin padre
    }
  }

  archivo.close();
  std::cout << "Arbol exportado exitosamente a " << nombreArchivo << ".\n";
}
