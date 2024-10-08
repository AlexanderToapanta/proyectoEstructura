#ifndef LISTA_H
#define LISTA_H

#include <iostream>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <vector>
#include "ArbolB.h"
#include "Nodo.h"
#include "Persona.h"
#include "Ordenamientos.h"
#include "Generaciones.h"
#include "I_Mezclar.h"
#include "ArbolRadix.h"  // Incluye la clase ArbolRadix

class Lista : public Mezclar {
private:
    Nodo* cabeza;
    Nodo* cola;
    bool esCircular;
    std::unordered_set<std::string> cedulasRegistradas;
    std::string nombreArchivo;
    ArbolRadix arbolRadix; // Añade una instancia del árbol Radix

    void crearBackup(const std::string& nombreArchivo, const std::string& contenido);

public:
    Lista(bool circular = false, const std::string& nombreArchivo = "Datos_Personas.txt");
    ~Lista();

    bool verificarCedula(const std::string& cedula);
    void insertar(Persona dato);
    void insertarEn(int indice, Persona dato);
    void ordenar(int criterio);
    void ordenarCaracteres();
    void ordenarRadix(int criterio);

    void ordenarBurbuja(int criterio);
    void ordenarQuicksort(int criterio);
    void ordenarCaracteresQuicksort();
    void ordenarIntercambio(int criterio);
    void ordenarCaracteresIntercambio();
    std::string obtenerContenidoOrdenado() const;
    void imprimir() const;
    void guardarEnArchivo(const std::string& nombreArchivo) const;
    void cargarDesdeArchivo(const std::string& nombreArchivo);
    void insertarDesdeArchivo(Persona dato);
    void agregarAlArchivo(const Persona& dato) const;

    void ordenarInverso(int criterio);
    void eliminar(int);
    int cantidad();
    Persona datoEn(int);
    Persona* nodoEn(int); // Metodo para devolver NodoArbolBs
    void recorrer(std::function<void(Persona)> callback);
    void insertarOrdenado(Persona per, int);
    Persona eliminarPrimero();
    void insertarUltimo(Persona persona);
    void ordenarDistribucion(int criterio);
    void InertirPersonas(int criterio);
   
    Nodo* getCabeza();
    Nodo* getCola();
    void setCabeza(Nodo* newCabeza);
    void setCola(Nodo* newCola);
    void contarVocalesYConsonantes(int& vocales, int& consonantes);
    void contarVocalesYConsonantes(const std::string& texto, int index, int& vocales, int& consonantes) const;
    void MezclaPares() override;
    Nodo* combinarListas(Nodo* cabeza1, Nodo* cabeza2, int criterio);
    void leerArchivoEInsertarEnArbolB(ArbolB& arbol);
    
    // Nueva función para cargar el archivo en el árbol Radix
    void cargarArchivoEnArbolRadix(ArbolRadix& arbol);
};

#endif // LISTA_H
