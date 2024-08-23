#pragma once
#include "NodoArbolRadix.h"
#include "Persona.h"

class ArbolRadix {
    NodoArbolRadix* raizNombre;
    NodoArbolRadix* raizApellido;
    NodoArbolRadix* raizNombre2;

    void insertar(NodoArbolRadix* nodo, const std::string& palabra);
    bool buscar(NodoArbolRadix* nodo, const std::string& palabra);
    void mostrar(NodoArbolRadix* nodo, const std::string& prefijo, int nivel) const;
    void limpiar(NodoArbolRadix* nodo);
    int calcularAltura(NodoArbolRadix* nodo) const;
    int calcularNivel(NodoArbolRadix* nodo, const std::string& palabra, int nivelActual) const;
    int calcularNivelMaximo(NodoArbolRadix* nodo, int nivelActual) const;
    void Num_Abuelos(NodoArbolRadix* nodo, int& contador) const;
    int countAbuelos(NodoArbolRadix* nodo) const;

    // Función de Fibonacci utilizando programación dinámica
    int fibonacci(int n) const;
    
    // Búsqueda de Fibonacci
    bool busquedaFibonacci(NodoArbolRadix* nodo, const std::string& palabra);

public:
    ArbolRadix();
    ~ArbolRadix();
    void insertar(const Persona& persona);
    bool buscar(const std::string& nombre);
    bool buscarApellido(const std::string& apellido);
    bool buscarNombre2(const std::string& segundoNombre);
    void mostrarApellidos() const;
    void mostrarNombres() const;
    void mostrarNombre2() const;
    int calcularAlturaTotal() const;
    int calcularNivelMaximoTotal() const;
    void mostrarAlturaYNivelGeneral() const;
    bool busquedaFibonacci(const std::string& nombre) ;

    // Función de reporte para números de Fibonacci
    void mostrarReporteFibonacci() const;
};
