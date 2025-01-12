#include <iostream>
#include <vector>//библиотека для работы с динамическими массивами (векторами).
#include <limits>
#include <locale>
#include <fstream>//библиотека для работы с файлами (чтение и запись в файл и из файла).
#include <cstdlib>//библиотека генерации чисел
#include <ctime>//используем для иницилизации гсч

using namespace std;

const int INF = numeric_limits<int>::max();//константа для бесконечности+ инф используем как обозначения отсутствия пути между вершинами в графе.МАКС знач тип инт

void printMenu() {
    cout << "===К=У=Р=С=О=В=О=Й==========\n";
    cout << "1. Ввести граф вручную\n";
    cout << "2. Сгенерировать ориентированную матрицу\n";
    cout << "3. Сгенерировать неориентированную матрицу\n";
    cout << "4. Ввести матрицу из файла\n";
    cout << "5. Выполнить алгоритм Флойда\n";
    cout << "6. Вывести матрицу кратчайших путей\n";
    cout << "7. Сохранить матрицу в файл\n";
    cout << "8. Выход\n";
    cout << "============================\n";
    cout << "Выберите пункт меню: ";
}//функция меню которая выдает действия которые будут доступны пользователю

void printMatrix(const vector<vector<int>>& matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] == INF) {
                cout << "ИНФА ";
            }
            else {
                cout << matrix[i][j] << " ";
            }
        }
        cout << endl;
    }
}//matrix = двумерный вектор, представляющий матрицу смежности.
//n = количество вершин-размер матрицы.
//Функция проходит по всем элементам матрицы и выводит их на экран.Если элемент равен INF, выводится "ИНФA".


int inputPositiveInteger(const string& prompt) {
    int value;
    do {
        cout << prompt;
        cin >> value;
        if (value <= 0) {
            cout << "Ошибка: число должно быть положительным. Попробуйте снова.\n";
        }
    } while (value <= 0);
    return value;
}

void inputGraph(vector<vector<int>>& graph, int& n) {
    n = inputPositiveInteger("Введите количество вершин графа: ");
    graph.assign(n, vector<int>(n, INF));

    cout << "Введите матрицу смежности графа (используйте " << INF << " для бесконечности):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> graph[i][j];
            if (i == j) graph[i][j] = 0;// Нулевая длина пути для петли
        }
    }
}//graph = ссылка на двумерный вектор, который будет хранить матрицу смежности.
//n = ссылка на переменную, которая будет хранить количество вершин.
//Функция запрашивает у пользователя количество вершин, инициализирует матрицу graph значениями INF, а затем запрашивает значения для матрицы смежности, устанавливая нулевые значения для диагонали.


void generateDirectedGraph(vector<vector<int>>& graph, int& n) {
    n = inputPositiveInteger("Введите количество вершин графа: ");
    graph.assign(n, vector<int>(n, INF));
    srand(time(0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                graph[i][j] = rand() % 10 + 1;
            }
            else {
                graph[i][j] = 0;
            }
        }
    }
    cout << "Ориентированная матрица сгенерирована:\n";
    printMatrix(graph, n);
}//Эта функция генерирует случайную ориентированную матрицу смежности.
//srand(time(0)) = инициализация генератора случайных чисел на основе текущего времени.
//Внутренние циклы заполняют матрицу случайными значениями от 1 до 10 для всех пар вершин,кроме друг друга 


void generateUndirectedGraph(vector<vector<int>>& graph, int& n) {
    n = inputPositiveInteger("Введите количество вершин графа: ");
    graph.assign(n, vector<int>(n, INF));
    srand(time(0));

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int weight = rand() % 10 + 1;// Генерация случайного веса от 1 до 10
            graph[i][j] = weight;
            graph[j][i] = weight;
        }
        graph[i][i] = 0;// Нулевая длина пути для петли
    }
    cout << "Неориентированная матрица сгенерирована:\n";
    printMatrix(graph, n);
}//Эта функция генерирует случайную неориентированную матрицу смежности.
//Здесь веса задаются симметрично = если i связано с j, то и j связано с i.



void inputGraphFromFile(vector<vector<int>>& graph, int& n) {
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла!\n";
        return;
    }

    file >> n;
    if (n <= 0) {
        cout << "Ошибка: количество вершин в файле должно быть положительным.\n";
        file.close();
        return;
    }

    graph.assign(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file >> graph[i][j];
            if (i == j) graph[i][j] = 0;// Нулевая длина пути для петли
        }
    }
    file.close();
    cout << "Матрица загружена из файла:\n";
    printMatrix(graph, n);
}//Функция загружает матрицу смежности из файла.
//Проверяет, удалось ли открыть файл, и если нет, выводит сообщение об ошибке.
//Читает количество вершин и значения матрицы из файла, устанавливая нулевые значения для диагонали.


void floydWarshall(const vector<vector<int>>& graph, vector<vector<int>>& dist, int n) {
    dist = graph;
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    cout << "Алгоритм Флойда выполнен. Матрица кратчайших путей:\n";
    printMatrix(dist, n);
}//Эта функция реализует алгоритм Флойда-Уоршелла для нахождения кратчайших путей между всеми парами вершин.
//dist инициализируется значениями из graph.
//Три вложенных цикла проверяют, можно ли улучшить кратчайший путь между вершинами i и j через вершину k.


void saveMatrixToFile(const vector<vector<int>>& matrix, int n) {
    string filename;
    cout << "Введите имя файла для сохранения: ";
    cin >> filename;

    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для записи!\n";
        return;
    }

    file << n << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file << (matrix[i][j] == INF ? INF : matrix[i][j]) << " ";
        }
        file << endl;
    }
    file.close();
    cout << "Матрица сохранена в файл.\n";
}//Функция сохраняет матрицу в файл.
//Проверяет, удалось ли открыть файл, и если нет, выводит сообщение об ошибке.
//Записывает размер матрицы и ее значения в файл.


int main() {
    setlocale(LC_ALL, "Russian");// локализуем что бы компилятор и консоль выдавала русские символы в командной строке.

    vector<vector<int>> graph;
    vector<vector<int>> dist;
    int n = 0;
    int choice;

    do {
        printMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            inputGraph(graph, n);
            break;

        case 2:
            generateDirectedGraph(graph, n);
            break;

        case 3:
            generateUndirectedGraph(graph, n);
            break;

        case 4:
            inputGraphFromFile(graph, n);
            break;

        case 5:
            if (graph.empty()) {
                cout << "Сначала введите граф!\n";
            }
            else {
                floydWarshall(graph, dist, n);
            }
            break;

        case 6:
            if (dist.empty()) {
                cout << "Сначала выполните алгоритм Флойда!\n";
            }
            else {
                cout << "Матрица кратчайших путей:\n";
                printMatrix(dist, n);
            }
            break;

        case 7:
            if (dist.empty()) {
                cout << "Сначала выполните алгоритм Флойда!\n";
            }
            else {
                saveMatrixToFile(dist, n);
            }
            break;

        case 8:
            cout << "Выход из программы.\n";
            break;

        default:
            cout << "Некорректный выбор. Попробуйте снова.\n";
        }
    } while (choice != 8);

    return 0;
}//Объявляются переменные graph (матрица смежности), dist (матрица кратчайших путей), n (количество вершин) и choice (выбор пользователя).
//Цикл do - while отображает меню и обрабатывает выбор пользователя, вызывая соответствующие функции.
//В зависимости от выбора выполняются различные действия, такие как ввод графа, генерация матриц, выполнение алгоритма Флойда и сохранение матрицы в файл.

