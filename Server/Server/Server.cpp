#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // подключение библиотеки

using namespace std;

int fibo(int num); // функция для вычисления чисел Фибоначчи

int main() {
    setlocale(LC_ALL, "rus");
    WSADATA wsaData; // структура для хранения информации о Winsock
    SOCKET serverSocket, clientSocket; // сокеты для сервера и клиента
    SOCKADDR_IN serverAddr, clientAddr; // структуры для адресов сервера и клиента
    int port = 2000; // порт для соединения
    char buffer[1024]; // буфер для передачи данных
    int clientAddrSize = sizeof(clientAddr); // размер структуры адреса клиента
    int num; // число для вычисления чисел Фибоначчи

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // инициализация Winsock
        cout << "Ошибка инициализации Winsock\n";
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // создание сокета для сервера

    if (serverSocket == INVALID_SOCKET) { // проверка на ошибку создания сокета
        cout << "Ошибка создания сокета\n";
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET; // установка семейства адресов
    serverAddr.sin_port = htons(port); // установка порта
    serverAddr.sin_addr.s_addr = INADDR_ANY; // установка IP-адреса

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) { // привязка сокета к адресу
        cout << "Ошибка привязки сокета\n";
        shutdown(serverSocket, 2);
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) { // перевод сокета в режим ожидания соединения
        cout << "Ошибка перевода сокета в режим ожидания соединения\n";
        shutdown(serverSocket, 2);
        closesocket(serverSocket);
        WSACleanup();
        exit(1);
    }

    printf("Сервер запущен по адресу 127.0.0.1:%d\n", port);

    while (true) {
        clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize); // ожидание соединения клиента

        if (clientSocket == INVALID_SOCKET) { // проверка на ошибку соединения
            cout << "Ошибка соединения с клиентом\n";
            shutdown(serverSocket, 2);
            closesocket(serverSocket);
            WSACleanup();
            exit(1);
        }

        cout << "Присоединился новый клиент\n";

        while (true) {
            memset(buffer, 0, sizeof(buffer)); // очистка буфера
            recv(clientSocket, buffer, sizeof(buffer), 0); // получение данных от клиента

            if (strstr(buffer, "exit") != NULL) { // проверка на команду выхода
                shutdown(clientSocket, 2);
                shutdown(serverSocket, 2);
                closesocket(clientSocket);
                closesocket(serverSocket);
                exit(0);
            }

            try {
                num = stoi(buffer); // преобразование строки в число
                string result = to_string(fibo(num)) + "\n"; // вычисление числа Фибоначчи и преобразование его в строку
                send(clientSocket, result.c_str(), result.length(), 0); // отправка результата клиенту
            }
            catch (invalid_argument e) {
                memset(buffer, 0, sizeof(buffer));
                string errorMessage = "Введите число корректно\n";
                send(clientSocket, errorMessage.c_str(), errorMessage.length(), 0);
            }
        }
    }
    shutdown(serverSocket, 2);
    closesocket(serverSocket); // закрытие сокета сервера
    WSACleanup(); // завершение работы с Winsock

    return 0;
}

int fibo(int num) { // функция для вычисления чисел Фибоначчи
    if (num <= 0) {
        return 0;
    }
    else if (num == 1) {
        return 1;
    }
    else {
        return fibo(num - 1) + fibo(num - 2);
    }
}
