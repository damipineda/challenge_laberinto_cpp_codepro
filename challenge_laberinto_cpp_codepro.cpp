#include <vector> // Incluye la biblioteca vector para utilizar vectores
#include <random> // Incluye la biblioteca random para generar números aleatorios
#include <queue> // Incluye la biblioteca queue para utilizar colas en BFS
#include <utility> // Incluye la biblioteca utility para utilizar pares (pair)
#include <iostream> // Incluye la biblioteca iostream para entrada y salida estándar
#include <algorithm> // Incluye la biblioteca algorithm para utilizar algoritmos como shuffle
#include <conio.h> // Incluye la biblioteca conio.h para usar funciones de consola como _kbhit() y _getch()

using namespace std; // Utiliza el espacio de nombres std para evitar prefijos std::

random_device rd; // Generador de números aleatorios basado en hardware
mt19937 generadorNumero(rd()); // Inicializa el generador de números aleatorios con rd()

int filas, columnas; // Variables globales para el número de filas y columnas del laberinto
pair<int, int> entrada, salida; // Variables globales para las coordenadas de la entrada y salida del laberinto

// Estructura que representa una celda del laberinto
struct Celda {
    bool celdaVisitada; // Indica si la celda ha sido visitada
    bool paredSuperior; // Indica si la pared superior de la celda existe
    bool paredInferior; // Indica si la pared inferior de la celda existe
    bool paredIzquierda; // Indica si la pared izquierda de la celda existe
    bool paredDerecha; // Indica si la pared derecha de la celda existe
    bool enCamino; // Indica si la celda es parte del camino resuelto

    // Constructor por defecto: inicializa las paredes como existentes y la celda no visitada
    Celda() : celdaVisitada(false), paredSuperior(true), paredInferior(true), paredIzquierda(true), paredDerecha(true), enCamino(false) {}
};

typedef vector<vector<Celda>> Laberinto; // Define Laberinto como un vector de vectores de celdas

// Función recursiva para generar el laberinto usando el algoritmo Depth-First Search (DFS)
void generarLaberinto(Laberinto &laberinto, int fil, int col, mt19937 &generadorNumero) {
    laberinto[fil][col].celdaVisitada = true; // Marca la celda actual como visitada

    vector<pair<int, int>> direcciones = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; // Direcciones posibles: abajo, arriba, derecha, izquierda
    shuffle(direcciones.begin(), direcciones.end(), generadorNumero); // Mezcla las direcciones aleatoriamente

    // Intenta visitar las celdas adyacentes en el orden aleatorio generado
    for (const auto &dir : direcciones) {
        int nuevaFila = fil + dir.first;
        int nuevaColumna = col + dir.second;

        // Verifica si la celda vecina está dentro de los límites y no ha sido visitada
        if ((nuevaFila >= 0 && nuevaFila < filas) && (nuevaColumna >= 0 && nuevaColumna < columnas) && (!laberinto[nuevaFila][nuevaColumna].celdaVisitada)) {
            // Elimina las paredes entre la celda actual y la celda vecina
            if (dir.first == 1) { // Hacia abajo
                laberinto[fil][col].paredInferior = false;
                laberinto[nuevaFila][nuevaColumna].paredSuperior = false;
            } else if (dir.first == -1) { // Hacia arriba
                laberinto[fil][col].paredSuperior = false;
                laberinto[nuevaFila][nuevaColumna].paredInferior = false;
            } else if (dir.second == 1) { // Hacia la derecha
                laberinto[fil][col].paredDerecha = false;
                laberinto[nuevaFila][nuevaColumna].paredIzquierda = false;
            } else if (dir.second == -1) { // Hacia la izquierda
                laberinto[fil][col].paredIzquierda = false;
                laberinto[nuevaFila][nuevaColumna].paredDerecha = false;
            }

            // Llama recursivamente para generar el laberinto desde la celda vecina
            generarLaberinto(laberinto, nuevaFila, nuevaColumna, generadorNumero);
        }
    }
}

// Función para imprimir el laberinto en la consola
void imprimirLaberinto(Laberinto &laberinto) {
    cout << endl << "=====================================================================" << endl;
    cout << "Presiona 'Enter' para regenerar un nuevo el laberinto, '1' para resolverlo, o 'Esc' para salir." << endl;
    cout << endl << "=====================================================================" << endl;

    // Imprime la representación visual del laberinto
    for (int i = 0; i < filas; ++i) {
        // Imprime las paredes superiores de las celdas
        for (int j = 0; j < columnas; ++j) {
            cout << "+";
            cout << (laberinto[i][j].paredSuperior ? "---" : "   ");
        }
        cout << "+" << endl;

        // Imprime las paredes laterales de las celdas y su contenido
        for (int j = 0; j < columnas; ++j) {
            cout << (laberinto[i][j].paredIzquierda ? "|" : " ");
            if (make_pair(i, j) == entrada || make_pair(i, j) == salida) {
                cout << " # "; // Marca la entrada y salida con '#'
            } else {
                cout << "   ";
            }
        }
        cout << "|" << endl;
    }

    // Imprime las paredes inferiores de la última fila
    for (int j = 0; j < columnas; ++j) {
        cout << "+---";
    }
    cout << "+" << endl;
}

// Función para resolver el laberinto usando el algoritmo Breadth-First Search (BFS)
bool resolverLaberintoBFS(Laberinto &laberinto) {
    queue<pair<int, int>> cola; // Cola para almacenar las celdas a visitar
    cola.push(entrada); // Agrega la celda de entrada a la cola
    laberinto[entrada.first][entrada.second].celdaVisitada = true; // Marca la celda de entrada como visitada
    vector<vector<pair<int, int>>> predecesores(filas, vector<pair<int, int>>(columnas, {-1, -1})); // Vector para almacenar los predecesores de cada celda

    vector<pair<int, int>> direcciones = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; // Direcciones posibles: abajo, arriba, derecha, izquierda

    // Mientras haya celdas en la cola
    while (!cola.empty()) {
        auto [filaActual, columnaActual] = cola.front(); // Obtiene la celda actual de la cola
        cola.pop(); // Elimina la celda actual de la cola

        // Si se llega a la salida, reconstruye el camino y devuelve verdadero
        if (make_pair(filaActual, columnaActual) == salida) {
            int f = filaActual, c = columnaActual;
            while (!(f == entrada.first && c == entrada.second)) {
                laberinto[f][c].enCamino = true; // Marca la celda como parte del camino resuelto
                auto [pf, pc] = predecesores[f][c];
                f = pf;
                c = pc;
            }
            laberinto[entrada.first][entrada.second].enCamino = true; // Marca la entrada como parte del camino resuelto
            return true;
        }

        // Intenta visitar las celdas adyacentes
        for (const auto &dir : direcciones) {
            int nuevaFila = filaActual + dir.first;
            int nuevaColumna = columnaActual + dir.second;

            // Verifica si la celda vecina está dentro de los límites y no ha sido visitada
            if (nuevaFila >= 0 && nuevaColumna >= 0 && nuevaFila < filas && nuevaColumna < columnas && !laberinto[nuevaFila][nuevaColumna].celdaVisitada) {
                bool puedeMoverse = false;
                if (dir.first == 1 && !laberinto[filaActual][columnaActual].paredInferior) { // Hacia abajo y no hay pared
                    puedeMoverse = true;
                } else if (dir.first == -1 && !laberinto[filaActual][columnaActual].paredSuperior) { // Hacia arriba y no hay pared
                    puedeMoverse = true;
                } else if (dir.second == 1 && !laberinto[filaActual][columnaActual].paredDerecha) { // Hacia la derecha y no hay pared
                    puedeMoverse = true;
                } else if (dir.second == -1 && !laberinto[filaActual][columnaActual].paredIzquierda) { // Hacia la izquierda y no hay pared
                    puedeMoverse = true;
                }

                // Si puede moverse, marca la celda como visitada y agrega a la cola
                if (puedeMoverse) {
                    laberinto[nuevaFila][nuevaColumna].celdaVisitada = true;
                    cola.push({nuevaFila, nuevaColumna});
                    predecesores[nuevaFila][nuevaColumna] = {filaActual, columnaActual}; // Almacena el predecesor
                }
            }
        }
    }

    return false; // Si no se encuentra una solución, devuelve falso
}

// Función para imprimir el laberinto resuelto en la consola
void imprimirLaberintoResuelto(Laberinto &laberinto) {
    cout << endl << "=====================================================================" << endl;
    cout << "Este es el laberinto resuelto, presiona 'Enter' para regenerar un nuevo laberinto, o 'Esc' para salir." << endl;
    cout << endl << "=====================================================================" << endl;

    // Imprime la representación visual del laberinto resuelto
    for (int i = 0; i < filas; ++i) {
        // Imprime las paredes superiores de las celdas
        for (int j = 0; j < columnas; ++j) {
            cout << "+";
            cout << (laberinto[i][j].paredSuperior ? "---" : "   ");
        }
        cout << "+" << endl;

        // Imprime las paredes laterales de las celdas y su contenido
        for (int j = 0; j < columnas; ++j) {
            cout << (laberinto[i][j].paredIzquierda ? "|" : " ");
            if (laberinto[i][j].enCamino) {
                cout << " x "; // Marca el camino resuelto con '*'
            } else if (make_pair(i, j) == entrada || make_pair(i, j) == salida) {
                cout << " # "; // Marca la entrada y salida con '#'
            } else {
                cout << "   ";
            }
        }
        cout << "|" << endl;
    }

    // Imprime las paredes inferiores de la última fila
    for (int j = 0; j < columnas; ++j) {
        cout << "+---";
    }
    cout << "+" << endl;
}

// Función para inicializar el laberinto
void inicializarLaberinto(Laberinto &laberinto) {
    filas = 5 + generadorNumero() % 16; // Genera un número aleatorio entre 5 y 20 para el número de filas
    columnas = 5 + generadorNumero() % 16; // Genera un número aleatorio entre 5 y 20 para el número de columnas
    laberinto = Laberinto(filas, vector<Celda>(columnas)); // Crea el laberinto con el número de filas y columnas generadas

    // Genera una posición aleatoria para la entrada en una de las paredes del laberinto
    int posicion = generadorNumero() % (2 * filas + 2 * columnas);
    if (posicion < columnas) { // Si la posición es menor que el número de columnas, la entrada está en la pared superior
        entrada = {0, posicion};
    } else if (posicion < columnas + filas) { // Si la posición es menor que el número de columnas más el número de filas, la entrada está en la pared derecha
        entrada = {posicion - columnas, columnas - 1};
    } else if (posicion < 2 * columnas + filas) { // Si la posición es menor que el doble del número de columnas más el número de filas, la entrada está en la pared inferior
        entrada = {filas - 1, columnas - 1 - (posicion - columnas - filas)};
    } else { // Si la posición es mayor que el doble del número de columnas más el número de filas, la entrada está en la pared izquierda
        entrada = {filas - 1 - (posicion - 2 * columnas - filas), 0};
    }

    // Genera una posición aleatoria para la salida en una de las paredes del laberinto, diferente a la entrada
    do {
        posicion = generadorNumero() % (2 * filas + 2 * columnas);
        if (posicion < columnas) { // Si la posición es menor que el número de columnas, la salida está en la pared superior
            salida = {0, posicion};
        } else if (posicion < columnas + filas) { // Si la posición es menor que el número de columnas más el número de filas, la salida está en la pared derecha
            salida = {posicion - columnas, columnas - 1};
        } else if (posicion < 2 * columnas + filas) { // Si la posición es menor que el doble del número de columnas más el número de filas, la salida está en la pared inferior
            salida = {filas - 1, columnas - 1 - (posicion - columnas - filas)};
        } else { // Si la posición es mayor que el doble del número de columnas más el número de filas, la salida está en la pared izquierda
            salida = {filas - 1 - (posicion - 2 * columnas - filas), 0};
        }
    } while (entrada == salida); // Repite el proceso mientras la salida sea igual a la entrada

    generarLaberinto(laberinto, entrada.first, entrada.second, generadorNumero); // Llama a la función para generar el laberinto a partir de la entrada
}

int main() {
    Laberinto laberinto; // Crea el laberinto
    inicializarLaberinto(laberinto); // Llama a la función para inicializar el laberinto
    imprimirLaberinto(laberinto); // Llama a la función para imprimir el laberinto

    while (true) { // Crea un bucle infinito para el menú
        if (_kbhit()) { // Verifica si se ha presionado una tecla
            char tecla = _getch(); // Obtiene la tecla presionada
            if (tecla == 27) { // Si la tecla presionada es 'Esc'
                cout << "¡Gracias por jugar!" << endl; // Imprime un mensaje de despedida
                break; // Sale del bucle infinito
            } else if (tecla == 13) { // Si la tecla presionada es 'Enter'
                inicializarLaberinto(laberinto); // Llama a la función para inicializar el laberinto
                imprimirLaberinto(laberinto); // Llama a la función para imprimir el laberinto
            } else if (tecla == '1') { // Si la tecla presionada es '1'
                // Reiniciar el laberinto y resolverlo
                for (int i = 0; i < filas; ++i) { // Recorre cada fila del laberinto
                    for (int j = 0; j < columnas; ++j) { // Recorre cada columna de la fila actual
                        laberinto[i][j].celdaVisitada = false; // Reinicia la celda como no visitada
                    }
                }
                resolverLaberintoBFS(laberinto); // Llama a la función para resolver el laberinto con el algoritmo BFS
                imprimirLaberintoResuelto(laberinto); // Llama a la función para imprimir el laberinto resuelto
            }
        }
    }

    return 0; // Devuelve 0 indicando que el programa terminó correctamente
}
