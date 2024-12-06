#include <cctype>
#include <iostream>
#include <set>
#include <string>
#include "Funciones.h"

// main
int main(int argc, char *argv[]) {
  bool bandera = true;
  std::set<Nodo *> miembros;
  int anioNacimiento, condicion, opcion;
  std::string nombre, madre, padre, nombreArchivo;

  // Menú
  do {
    while (true) {
      std::cout << "\n------------------------------------------------------------------------\n";
      std::cout << "Favor ingrese la accion que desea realizar en su arbol genealogico:\n"
                << "(1) Ingresar nuevo miembro\n"
                << "(2) Eliminar miembro\n"
                << "(3) Buscar un miembro de la familia\n"
                << "(4) Imprimir arbol completo\n"
                << "(5) Imprimir relacion entre miembros familiares\n"
                << "(6) Importar o Exportar arbol genealogico\n"
                << "(7) Salir\n";
      std::cout << "------------------------------------------------------------------------\n";
      std::cin >> condicion;
      limpiarTerminal();
      if (!verificarEntrada(condicion)) {
        std::cout << "Entrada no valida. Por favor, ingrese un numero.\n";
      } else {
        break;  // Salir del bucle si la entrada es válida
      }
    }

    // Opciones del menu
    switch (condicion) {
      case 1:
        std::cout << "Ingrese el nombre del miembro: ";
        //std::cin.ignore();  // Limpiar el buffer antes de leer una línea
        std::getline(std::cin, nombre);
        while (true) {
          std::cout << "Ingrese el anio de nacimiento del miembro: ";
          std::cin >> anioNacimiento;
          if (!verificarEntrada(anioNacimiento)) {
            std::cout << "Entrada no valida. Por favor, ingrese un numero.\n";
          } else {
            break;  // Salir del bucle si la entrada es válida
          }
        }
        std::cout << "Ingrese el nombre de la madre (o deje en blanco): ";
        std::getline(std::cin, madre);
        std::cout << "Ingrese el nombre del padre (o deje en blanco): ";
        std::getline(std::cin, padre);
        std::cout << std::endl;
        limpiarTerminal();
        agregarMiembro(miembros, nombre, madre, padre, anioNacimiento);
        break;

      case 2:
        std::cout << "Miembros disponibles para eliminar:\n";
        imprimirMiembrosOrdenados(miembros);
        std::cout << "Ingrese el nombre del miembro a eliminar: ";
        std::getline(std::cin, nombre);
        limpiarTerminal();
        eliminarMiembro(miembros, nombre);
        break;

      case 3:
        std::cout << "Miembros disponibles:\n";
        imprimirMiembrosOrdenados(miembros);
        buscarMiembroFamiliar(miembros);
        break;
      case 4:
        std::cout << "Arbol genealogico completo:\n";
        for (Nodo *miembro : miembros) {
          if (miembro->madre == nullptr && miembro->padre == nullptr) {
            imprimirArbol(miembro);
          }
        }
        break;

      case 5:
        std::cout << "Miembros disponibles para consultar relaciones:\n";
        imprimirMiembrosOrdenados(miembros);
        std::cout << "Ingrese el nombre del miembro para ver sus relaciones: ";
  
        std::getline(std::cin, nombre);
        limpiarTerminal();

        if (Nodo *miembro = buscarMiembro(miembros, nombre)) {
          imprimirRelacionDirecta(miembro);
        } else {
          std::cout << "Miembro no encontrado\n";
        }
        break;
      case 6:
        opcion = solicitarOpcion();
        if (opcion == 1) {
          // Importar de un archivo .csv
          std::cout << "Ingrese el nombre del archivo a importar: ";
          std::getline(std::cin, nombreArchivo);
          importarArbolGenealogico(miembros, nombreArchivo);
        } else if (opcion == 2) {
          // Exportar a un archivo .csv
          std::string nombreArchivo, apellido;
          std::cout << "Ingrese el apellido de la familia : ";
          std::getline(std::cin, apellido);
          nombreArchivo = "Familia_" + apellido + ".csv";
          exportarArbolGenealogico(miembros, nombreArchivo);
        } else {
          std::cout << "Opcion invalida.\n";
        }
        break;
      case 7:
        bandera = false;
        break;
      default:
        std::cout << "Opcion invalida. Intente nuevamente.\n";
        break;
    }
  } while (bandera);

  // Liberar memoria antes de salir
  for (Nodo *miembro : miembros) {
    delete miembro;
  }
  std::cout << "Cerrando programa...\n";
  return 0;
}