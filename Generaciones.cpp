#include "Generaciones.h"
#include <cstring>
#include <vector>
#include <algorithm> 
#include <iomanip>

extern void rc4(const char* key, const char* text, char* output, int text_len);


std::string generarCorreo(Nodo* cabeza, Persona& persona, bool esCircular) {
    Nodo* auxiliar = cabeza;
    std::string inicialesNombre = "";
    inicialesNombre.push_back(tolower(persona.nombre[0]));
    if (!persona.segundoNombre.empty()) {
        inicialesNombre.push_back(tolower(persona.segundoNombre[0]));
    }
    std::string correo = inicialesNombre + persona.apellido;
    std::string correoBase = correo;
    int contador = 0;

    while (auxiliar != nullptr) {
        if (auxiliar->dato.getCorreo() == correo) {
            contador++;
            correo = correoBase + std::to_string(contador);
        }
        auxiliar = auxiliar->siguiente;
        if (esCircular && auxiliar == cabeza) break;
    }

    return correo;
}


void printAsHex(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(data[i]));
    }
    std::cout << std::endl;
}

// Función para generar la contraseña cifrada en hexadecimal
std::string generarContrasena(Persona& persona) {
    static bool primerUsuario = true;
    static int contador = 1;

    std::string apellido = persona.getApellido();
    std::string primerNombre = persona.getNombre();
    std::string segundoNombre = persona.getSegundoNombre();

    char ultimaLetraApellido = std::tolower(apellido.back());
    char primeraLetraPrimerNombre = std::tolower(primerNombre.front());
    char ultimaLetraSegundoNombre = !segundoNombre.empty() ? std::tolower(segundoNombre.back()) : '\0';

    std::string restoApellido = apellido.substr(0, apellido.size() - 1);
    std::reverse(restoApellido.begin(), restoApellido.end());
    std::transform(restoApellido.begin(), restoApellido.end(), restoApellido.begin(), ::tolower);

    std::string contrasenaBase = std::string(1, ultimaLetraApellido) + primeraLetraPrimerNombre;
    if (ultimaLetraSegundoNombre != '\0') {
        contrasenaBase += ultimaLetraSegundoNombre;
    }
    contrasenaBase += restoApellido;

    if (!primerUsuario) {
        contrasenaBase += std::to_string(contador);
        contador++;
    }

    primerUsuario = false;

    // Cifrar la contraseña utilizando RC4
    std::string contrasenaEncriptada(contrasenaBase.size(), '\0');
    const char* key = "secretkey";
    rc4(key, contrasenaBase.c_str(), &contrasenaEncriptada[0], contrasenaBase.size());

    // Convertir a hexadecimal para almacenamiento
    std::stringstream hexStream;
    for (unsigned char byte : contrasenaEncriptada) {
        hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::string contrasenaEnHex = hexStream.str();

    // Imprimir en hexadecimal para depuración
    std::cout << "Encrypted password (hex): " << contrasenaEnHex << std::endl;

    persona.setContrasena(contrasenaEnHex);
    persona.setContrasenaInicial(contrasenaBase);

    return contrasenaEnHex;
}

std::string desencriptarContrasena(Persona& persona) {
    // Obtener la contraseña encriptada en hexadecimal
    std::string contrasenaEncriptadaHex = persona.getContrasena();

    // Convertir la cadena hexadecimal a bytes directamente dentro de la función
    std::string contrasenaEncriptada(contrasenaEncriptadaHex.size() / 2, '\0');
    for (size_t i = 0; i < contrasenaEncriptadaHex.length(); i += 2) {
        std::string byteString = contrasenaEncriptadaHex.substr(i, 2);
        char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
        contrasenaEncriptada[i / 2] = byte;
    }

    // Preparar el búfer de salida para la contraseña desencriptada
    std::string contrasenaDesencriptada(contrasenaEncriptada.size(), '\0');

    // Usar la misma clave utilizada en la función de cifrado
    const char* key = "secretkey";
    
    // Desencriptar utilizando RC4
    rc4(key, contrasenaEncriptada.c_str(), &contrasenaDesencriptada[0], contrasenaEncriptada.size());

    // Guardar la contraseña desencriptada en el objeto Persona
    persona.setContrasenaDes(contrasenaDesencriptada);

    return contrasenaDesencriptada;
}

void contarVocalesYConsonantesRec(Nodo* nodo, int& vocales, int& consonantes) {
    if (nodo == nullptr) return;

    std::string nombreCompleto = nodo->dato.nombre + nodo->dato.segundoNombre + nodo->dato.apellido;
    for (char c : nombreCompleto) {
        c = std::tolower(c);
        if (std::isalpha(c)) {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vocales++;
            }
            else {
                consonantes++;
            }
        }
    }
    contarVocalesYConsonantesRec(nodo->siguiente, vocales, consonantes);
}

void contarVocalesYConsonantes(Nodo* nodo, int& vocales, int& consonantes) {
    vocales = 0;
    consonantes = 0;
    contarVocalesYConsonantesRec(nodo, vocales, consonantes);
}
