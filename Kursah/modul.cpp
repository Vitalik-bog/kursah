#include <iostream>
#include <vector>
#include <limits>
#include <locale>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int INF = numeric_limits<int>::max();

void printMenu() {
    cout << "===К=У=Р=С=О=В=О=Й==========\n";
    cout << "1. Ввести граф вручную\n";
    cout << "2. Сгенерировать ориентированную матрицу\n";
    cout << "3. Сгенерировать неориентированную матрицу\n";
    cout << "4. Ввести матрицу из файла\n";
    cout << "5. Выполнить алгоритм Флойда\n";
    cout << "6. Вывести матрицу кратчайшего расстояния\n";
    cout << "7. Вывести пути графа\n";
    cout << "8. Сохранить матрицу в файл\n";
    cout << "9. Выход\n";
    cout << "============================\n";
    cout << "Выберите пункт меню: ";
}


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
}


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
            if (i == j) graph[i][j] = 0;
        }
    }
}
void generateDirectedGraph(vector<vector<int>>& graph, int& n) {
    n = inputPositiveInteger("Введите количество вершин графа: ");
    graph.assign(n, vector<int>(n, INF));
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) graph[i][j] = rand() % 10 + 1;
            else graph[i][j] = 0;
        }
    }
    cout << "Ориентированная матрица сгенерирована:\n";
    printMatrix(graph, n);
}

void generateUndirectedGraph(vector<vector<int>>& graph, int& n) {
    n = inputPositiveInteger("Введите количество вершин графа: ");
    graph.assign(n, vector<int>(n, INF));
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int weight = rand() % 10 + 1;
            graph[i][j] = weight;
            graph[j][i] = weight;
        }
        graph[i][i] = 0;
    }
    cout << "Неориентированная матрица сгенерирована:\n";
    printMatrix(graph, n);
}

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
            if (i == j) graph[i][j] = 0;
        }
    }
    file.close();
    cout << "Матрица загружена из файла:\n";
    printMatrix(graph, n);
}

// Функция для вывода пути
void printPath(int u, int v, const vector<vector<int>>& next) {
    if (next[u][v] == -1) {
        cout << "Путь недоступен\n";
        return;
    }
    vector<int> path;
    int current = u;
    while (current != v) {
        path.push_back(current);
        current = next
            [current][v];
        if (current == -1) {
            cout << "Нет пути\n";
            return;
        }
    }
    path.push_back(v);
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i] + 1;
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << endl;
}

// Алгоритм Флойда-Уоршалла с восстановлением путей
void floydWarshall(const vector<vector<int>>& graph, vector<vector<int>>& dist, vector<vector<int>>& next, int n) {
    dist = graph;
    next.assign(n, vector<int>(n, -1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (graph[i][j] != INF) next[i][j] = j;
        }
    }

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
    cout << "Алгоритм Флойда выполнен. Матрица кратчайших путей:\n";
    printMatrix(dist, n);
}

// Сохранение матрицы в файл
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
}

// Основной код программы
int main() {
    setlocale(LC_ALL, "Russian");
    vector<vector<int>> graph;
    vector<vector<int>> dist;
    vector<vector<int>> next;
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
                floydWarshall(graph, dist, next, n);
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
                cout << "Пути между вершинами:\n";
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < n; ++j) {
                        cout << "Путь из " << i + 1 << " в " << j + 1 << ": ";
                        printPath(i, j, next);
                    }
                }
            }
            break;
        case 8:
            if (dist.empty()) {
                cout << "Сначала выполните алгоритм Флойда!\n";
            }
            else {
                saveMatrixToFile(dist, n);
            }
            break;
        case 9:
            cout << "Выход из программы.\n";
            break;
        default:
            cout << "Некорректный выбор. Попробуйте снова.\n";
        }
    } while (choice != 9);

    return 0;
}