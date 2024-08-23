#include "ArbolRadix.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

ArbolRadix::ArbolRadix() 
    : raizNombre(new NodoArbolRadix()), 
      raizApellido(new NodoArbolRadix()), 
      raizNombre2(new NodoArbolRadix()) {}

ArbolRadix::~ArbolRadix() {
    limpiar(raizNombre);
    limpiar(raizApellido);
    limpiar(raizNombre2);
    delete raizNombre;
    delete raizApellido;
    delete raizNombre2;
}
int ArbolRadix::fibonacci(int n) const {
    if (n <= 1) return n;

    // Tabla para almacenar los números de Fibonacci
    int fib0 = 0;
    int fib1 = 1;
    int fibN = 0;

    for (int i = 2; i <= n; ++i) {
        fibN = fib0 + fib1;
        fib0 = fib1;
        fib1 = fibN;
    }

    return fibN;
}
bool ArbolRadix::busquedaFibonacci(const std::string& nombre) {
    return busquedaFibonacci(raizNombre, nombre);
}

bool ArbolRadix::busquedaFibonacci(NodoArbolRadix* nodo, const std::string& palabra) {
    int n = palabra.size();
    
    // Encontrar el número de Fibonacci más pequeño mayor o igual al tamaño de la palabra
    int fibMMm2 = 0;   // (m-2)'th Fibonacci number
    int fibMMm1 = 1;   // (m-1)'th Fibonacci number
    int fibM = fibMMm2 + fibMMm1;  // m'th Fibonacci number

    while (fibM < n) {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    // Fijar un offset inicial
    int offset = -1;

    // Mientras haya elementos para ser examinados
    while (fibM > 1) {
        // Calcula el índice de Fibonacci
        int i = std::min(offset + fibMMm2, n - 1);

        // Compara la clave en el nodo con la palabra en la posición i
        if (palabra.compare(0, nodo->clave.size(), nodo->clave) == 0) {
            return buscar(nodo, palabra.substr(nodo->clave.size()));
        }

        // Si la clave es mayor que la palabra, mueve el rango de búsqueda
        if (palabra.compare(0, nodo->clave.size(), nodo->clave) > 0) {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        } else {
            // Si la clave es menor, mueve el rango de búsqueda a los menores de Fibonacci
            fibM = fibMMm2;
            fibMMm1 -= fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        }
    }

    // Compara el último elemento con la palabra
    if (fibMMm1 && offset + 1 < n && palabra.compare(nodo->clave) == 0) {
        return buscar(nodo, palabra.substr(nodo->clave.size()));
    }

    return false;
}

void ArbolRadix::insertar(const Persona& persona) {
    insertar(raizNombre, persona.nombre);
    insertar(raizApellido, persona.apellido);
    insertar(raizNombre2, persona.segundoNombre);
}

void ArbolRadix::insertar(NodoArbolRadix* nodo, const std::string& palabra) {
    if (palabra.empty()) {
        nodo->finPalabra = true;
        return;
    }

    NodoArbolRadix* actual = nodo->hijo;
    NodoArbolRadix* anterior = nullptr;

    while (actual) {
        size_t pos = 0;
        
        while (pos < actual->clave.size() && pos < palabra.size() && actual->clave[pos] == palabra[pos]) {
            ++pos;
        }

        // Si se encontró un prefijo común
        if (pos > 0) {
            std::string prefijoComun = actual->clave.substr(0, pos);
            std::string hijoRestante = actual->clave.substr(pos);
            std::string palabraRestante = palabra.substr(pos);

            // Caso donde la clave del nodo es completamente un prefijo de la palabra
            if (hijoRestante.empty()) {
                if (palabraRestante.empty()) {
                    actual->finPalabra = true; // La palabra completa ya está en el árbol
                } else {
                    // Crear un nuevo nodo para la parte restante de la palabra
                    NodoArbolRadix* nuevoHijo = new NodoArbolRadix(palabraRestante, true);
                    nuevoHijo->hijo = actual->hijo; // Conectar los hijos del nodo actual al nuevo hijo
                    actual->hijo = nuevoHijo;
                }
                return;
            } else {
                // Dividir el nodo actual
                NodoArbolRadix* nuevoHijo = new NodoArbolRadix(hijoRestante, actual->finPalabra);
                nuevoHijo->hijo = actual->hijo;
                actual->clave = prefijoComun;
                actual->hijo = nuevoHijo;
                actual->finPalabra = false;
                
                // Insertar el resto de la palabra en el nuevo hijo
                if (!palabraRestante.empty()) {
                    NodoArbolRadix* nuevoNodo = new NodoArbolRadix(palabraRestante, true);
                    nuevoHijo->siguiente = nuevoNodo;
                }
                return;
            }
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    // Si no se encontró un nodo con un prefijo común, añadir un nuevo nodo
    NodoArbolRadix* nuevoNodo = new NodoArbolRadix(palabra, true);
    if (anterior) {
        anterior->siguiente = nuevoNodo;
    } else {
        nodo->hijo = nuevoNodo;
    }
}


bool ArbolRadix::buscar(const std::string& nombre) {
    return buscar(raizNombre, nombre);
}

bool ArbolRadix::buscarNombre2(const std::string& segundoNombre) {
    return buscar(raizNombre2, segundoNombre);
}

bool ArbolRadix::buscarApellido(const std::string& apellido) {
    return buscar(raizApellido, apellido);
}

bool ArbolRadix::buscar(NodoArbolRadix* nodo, const std::string& palabra) {
    if (palabra.empty()) {
        return nodo->finPalabra;
    }

    NodoArbolRadix* actual = nodo->hijo;
    while (actual) {
        if (palabra.find(actual->clave) == 0) {
            return buscar(actual, palabra.substr(actual->clave.size()));
        }
        actual = actual->siguiente;
    }

    return false;
}

void ArbolRadix::mostrarNombres() const {
    std::cout << "===========NOMBRES==============" << std::endl;
    mostrar(raizNombre, "", 0);
    std::cout << "Numero de abuelos: " << countAbuelos(raizNombre) << std::endl;

    int n = 10; 
    std::cout << "El " << n << "-esimo numero de Fibonacci es: " << fibonacci(n) << std::endl;
    
    std::cout << std::endl;
}

void ArbolRadix::mostrarNombre2() const {
    std::cout << "===========SEGUNDO NOMBRE==============" << std::endl;
    mostrar(raizNombre2, "", 0);
    std::cout << "Numero de abuelos: " << countAbuelos(raizNombre2) << std::endl;
    std::cout << std::endl;
}

void ArbolRadix::mostrarApellidos() const {
    std::cout << "===========APELLIDOS=============" << std::endl;
    mostrar(raizApellido, "", 0);
    std::cout << "Numero de abuelos: " << countAbuelos(raizApellido) << std::endl;
    std::cout << std::endl;
}
void ArbolRadix::mostrarReporteFibonacci() const {
    std::cout << "===========REPORTE FIBONACCI==============" << std::endl;

    for (int i = 0; i <= 10; ++i) {
        std::cout << "Fibonacci(" << i << ") = " << fibonacci(i) << std::endl;
    }
    
    std::cout << std::endl;
}

void ArbolRadix::mostrar(NodoArbolRadix* nodo, const std::string& prefijo, int nivel) const {
    if (!nodo) return;

    NodoArbolRadix* actual = nodo->hijo;
    while (actual) {
        std::string nuevoPrefijo = prefijo + actual->clave;
        int altura = calcularAltura(actual);
        
        
        std::cout << std::setw(nivel * 4) << "" << actual->clave;
        if (actual->finPalabra) {
            std::cout << " (Fin)";
        }
        std::cout << " - Nivel: " << nivel << ", Altura: " << altura << std::endl;

        
        mostrar(actual, nuevoPrefijo, nivel + 1);
        actual = actual->siguiente;
    }
}

void ArbolRadix::limpiar(NodoArbolRadix* nodo) {
    NodoArbolRadix* actual = nodo->hijo;
    while (actual) {
        NodoArbolRadix* siguiente = actual->siguiente;
        limpiar(actual);
        delete actual;
        actual = siguiente;
    }
}

int ArbolRadix::calcularAltura(NodoArbolRadix* nodo) const {
    if (!nodo) return -1; 
    int alturaMaxima = -1;
    NodoArbolRadix* actual = nodo->hijo;

    while (actual) {
        int alturaActual = calcularAltura(actual);
        alturaMaxima = std::max(alturaMaxima, alturaActual);
        actual = actual->siguiente;
    }

    return alturaMaxima + 1;
}



int ArbolRadix::calcularNivel(NodoArbolRadix* nodo, const std::string& palabra, int nivelActual) const {
    if (palabra.empty() || !nodo) return nivelActual;

    NodoArbolRadix* actual = nodo->hijo;
    while (actual) {
        if (palabra.find(actual->clave) == 0) { 
            return calcularNivel(actual, palabra.substr(actual->clave.size()), nivelActual + 1);
        }
        actual = actual->siguiente;
    }

    return nivelActual;
}



int ArbolRadix::calcularAlturaTotal() const {
    return calcularAltura(raizNombre); 
}

int ArbolRadix::calcularNivelMaximoTotal() const {
    return calcularNivelMaximo(raizNombre, 0); 
}

int ArbolRadix::calcularNivelMaximo(NodoArbolRadix* nodo, int nivelActual) const {
    if (!nodo) return nivelActual;

    int nivelMaximo = nivelActual;
    NodoArbolRadix* actual = nodo->hijo;

    while (actual) {
        nivelMaximo = std::max(nivelMaximo, calcularNivelMaximo(actual, nivelActual + 1));
        actual = actual->siguiente;
    }

    return nivelMaximo;
}

void ArbolRadix::mostrarAlturaYNivelGeneral() const {
    std::cout << "Altura del árbol Radix (Nombres): " << calcularAlturaTotal() << std::endl;
    std::cout << "Nivel máximo del árbol Radix (Nombres): " << calcularNivelMaximoTotal() << std::endl;
}
void ArbolRadix::Num_Abuelos(NodoArbolRadix* nodo, int& contador) const {
    if (nodo == nullptr) return;

    for (NodoArbolRadix* actual = nodo->hijo; actual != nullptr; actual = actual->siguiente) {
        if (actual->hijo != nullptr && actual->hijo->hijo != nullptr) {
            contador++;
        }
        Num_Abuelos(actual, contador);
    }
}

int ArbolRadix::countAbuelos(NodoArbolRadix* nodo) const {
    int abuelos = 0;
    if (nodo != nullptr) {
        Num_Abuelos(nodo, abuelos);
    }
    return abuelos;
}
