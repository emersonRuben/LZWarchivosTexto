#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

vector<int> comprimirLZW(const string &mensaje) {
    unordered_map<string, int> diccionario;

    for (int i = 0; i < 256; i++) {
        diccionario[string(1, char(i))] = i;
    }

    string cadena;
    vector<int> codificado;
    int codigo_actual = 256;

    for (char caracter : mensaje) {
        string cadena_caracter = cadena + caracter;

        if (diccionario.find(cadena_caracter) != diccionario.end()) {
            cadena = cadena_caracter;
        } else {
            codificado.push_back(diccionario[cadena]);
            diccionario[cadena_caracter] = codigo_actual++;
            cadena = string(1, caracter);
        }
    }

    if (!cadena.empty()) {
        codificado.push_back(diccionario[cadena]);
    }

    return codificado;
}

string descomprimirLZW(const vector<int> &codificado) {
    unordered_map<int, string> diccionario;

    for (int i = 0; i < 256; i++) {
        diccionario[i] = string(1, char(i));
    }

    string cadena = diccionario[codificado[0]];
    string resultado = cadena;
    int codigo_actual = 256;

    for (size_t i = 1; i < codificado.size(); i++) {
        int codigo = codificado[i];
        string entrada;

        if (diccionario.find(codigo) != diccionario.end()) {
            entrada = diccionario[codigo];
        } else if (codigo == codigo_actual) {
            entrada = cadena + cadena[0];
        } else {
            throw invalid_argument("Código incorrecto en la secuencia codificada");
        }

        resultado += entrada;
        diccionario[codigo_actual++] = cadena + entrada[0];
        cadena = entrada;
    }

    return resultado;
}

string leerArchivo(const string &nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo.");
    }

    string contenido((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
    archivo.close();
    return contenido;
}

void escribirArchivoCodificado(const string &nombreArchivo, const vector<int> &codificado) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo.");
    }

    for (int codigo : codificado) {
        archivo.write(reinterpret_cast<const char *>(&codigo), sizeof(int));
    }

    archivo.close();
}

vector<int> leerArchivoCodificado(const string &nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo.");
    }

    vector<int> codificado;
    int codigo;

    while (archivo.read(reinterpret_cast<char *>(&codigo), sizeof(int))) {
        codificado.push_back(codigo);
    }

    archivo.close();
    return codificado;
}

void escribirArchivoTexto(const string &nombreArchivo, const string &contenido) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo.");
    }

    archivo << contenido;
    archivo.close();
}

int main() {
    try {
        string nombreArchivoEntrada, nombreArchivoComprimido, nombreArchivoDescomprimido;

        cout << "Ingrese el nombre del archivo a comprimir: ";
        cin >> nombreArchivoEntrada;

        string contenido = leerArchivo(nombreArchivoEntrada);
        vector<int> codificado = comprimirLZW(contenido);

        cout << "Ingrese el nombre del archivo donde se guardara la compresion: ";
        cin >> nombreArchivoComprimido;

        escribirArchivoCodificado(nombreArchivoComprimido, codificado);

        cout << "Archivo comprimido guardado en: " << nombreArchivoComprimido << endl;

        cout << "Ingrese el nombre del archivo descomprimido: ";
        cin >> nombreArchivoDescomprimido;

        vector<int> codificadoLeido = leerArchivoCodificado(nombreArchivoComprimido);
        string contenidoDescomprimido = descomprimirLZW(codificadoLeido);

        escribirArchivoTexto(nombreArchivoDescomprimido, contenidoDescomprimido);

        cout << "Archivo descomprimido guardado en: " << nombreArchivoDescomprimido << endl;

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
