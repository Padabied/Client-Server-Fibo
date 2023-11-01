#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");
    // Инициализация библиотеки Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Создание сокета
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Установка адреса сервера и порта
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2000); // Порт сервера
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP-адрес сервера

    // Установка соединения с сервером
    result = connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        std::cerr << "connect failed: " << WSAGetLastError() << std::endl;
        shutdown(clientSocket, 2);
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    while (true) {
    // Ввод номера числа Фибоначчи
    std::cout << "Введите число Фибоначчи: ";
    char number[255];
    cin.getline(number, 255);

    if (strcmp(number, "exit") == 0) break;

    // Отправка номера на сервер
    result = send(clientSocket, reinterpret_cast<const char*>(&number), sizeof(number), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        shutdown(clientSocket, 2);
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Получение числа Фибоначчи от сервера
    char fibonacci[255];
    result = recv(clientSocket, fibonacci, sizeof(fibonacci), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
        shutdown(clientSocket, 2);
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Вывод числа Фибоначчи на экран
    std::cout << "Число Фибоначчи = " << stoi(fibonacci) << endl;
}

    // Закрытие сокета и освобождение ресурсов Winsock
    shutdown(clientSocket, 2);
    closesocket(clientSocket);
    WSACleanup(); 

    return 0; 
}