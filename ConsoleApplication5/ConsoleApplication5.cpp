#define WIN32_LEAN_AND_MEAN // Удаляет из заголовков Windows.h многие из часто используемых элементов, что уменьшает размер заголовка и ускоряет сборку.

#include <iostream> // Подключает библиотеку для ввода-вывода.
#include <Windows.h> // Подключает библиотеку для работы с Windows API.
#include <WinSock2.h> // Подключает библиотеку для работы с сокетами.
#include <WS2tcpip.h> // Подключает дополнительные функции для работы с TCP/IP.

using namespace std; // Использует пространство имен std.
int main() // Главная функция программы.
{
    WSADATA wsaData; // Структура для хранения информации о Winsock.
    ADDRINFO hints; // Структура для хранения подсказок для функции getaddrinfo.
    ADDRINFO* addrResult; // Указатель на структуру, которая будет содержать информацию об адресе.
    SOCKET ConnectSocket = INVALID_SOCKET; // Сокет для соединения.
    const char* sendBuffer1 = "Hello from Client. "; // Буфер для отправки данных.
    const char* sendBuffer2 = "Wazup!"; // Буфер для отправки данных.
    char recvBuffer[512]; // Буфер для приема данных.

    // Инициализирует Winsock.
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "WSAStartup failed result" << endl; // Выводит сообщение об ошибке, если инициализация не удалась.
        WSACleanup(); // Очищает Winsock.
        return 1; // Возвращает ошибку.
    }

    // Обнуляет структуру hints.
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET; // Семейство адресов.
    hints.ai_socktype = SOCK_STREAM; // Тип сокета.
    hints.ai_protocol = IPPROTO_TCP; // Протокол.

    // Получает информацию об адресе.
    result = getaddrinfo("localhost", "666", &hints, &addrResult);
    if (result != 0) {
        cout << "getaddrinfo failed error" << endl; // Выводит сообщение об ошибке, если получение информации об адресе не удалось.
        freeaddrinfo(addrResult); // Освобождает память, выделенную для addrResult.
        WSACleanup(); // Очищает Winsock.
        return 1; // Возвращает ошибку.
    }

    // Создает сокет.
    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Socket creation with" << endl; // Выводит сообщение об ошибке, если создание сокета не удалось.
        WSACleanup(); // Очищает Winsock.
        return 1; // Возвращает ошибку.
    }
    // Подключается к серверу.
    result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    if (result == SOCKET_ERROR) {
        cout << "Unable connect to server" << endl; // Выводит сообщение об ошибке, если подключение к серверу не удалось.
        ConnectSocket = INVALID_SOCKET; // Устанавливает сокет в недействительное состояние.
        freeaddrinfo(addrResult); // Освобождает память, выделенную для addrResult.
        WSACleanup(); // Очищает Winsock.
        return 1; // Возвращает ошибку.
    }
    // Отправляет данные.
    result = send(ConnectSocket, sendBuffer1, (int)strlen(sendBuffer1), 0);
    if (result == SOCKET_ERROR) {
        cout << "Send failed error " << endl; // Выводит сообщение об ошибке, если отправка данных не удалась.
        closesocket(ConnectSocket); // Закрывает сокет.
        freeaddrinfo(addrResult); // Освобождает память, выделенную для addrResult.
        WSACleanup(); // Очищает Winsock.
        return 1; // Возвращает ошибку.
    }
    cout << "Sent: " << result << "bytes" << endl; // Выводит количество отправленных байтов.

    // Отправляет данные.
    result = send(ConnectSocket, sendBuffer2, (int)strlen(sendBuffer2), 0);
    if (result == SOCKET_ERROR) {
        cout << "Send failed error " << endl; // Выводит сообщение об ошибке, если отправка данных не удалась.
        closesocket(ConnectSocket); // Закрывает сокет.
        freeaddrinfo(addrResult); // Освобождает память, выделенную для addrResult.
        WSACleanup(); // Очищает Winsock.
        return 1; // Возвращает ошибку.
    }
    cout << "Sent: " << result << "bytes" << endl; // Выводит количество отправленных байтов.

    // Закрывает сокет на отправку.
    result = shutdown(ConnectSocket, SD_SEND);
    if (result == SOCKET_ERROR) {
        cout << "shutdown error " << endl; // Выводит сообщение об ошибке, если закрытие сокета на отправку не удалось.
        freeaddrinfo(addrResult); // Освобождает память, выделенную для addrResult.
        WSACleanup(); // Очищает Winsock.
        return 1; // Возвращает ошибку.
    }
    // Принимает данные.
    do {
        ZeroMemory(recvBuffer, 512); // Обнуляет буфер для приема данных.
        result = recv(ConnectSocket, recvBuffer, 512, 0); // Принимает данные.
        if (result > 0) {
            cout << "Received " << result << "bytes" << endl; // Выводит количество принятых байтов.
            cout << "Received data " << recvBuffer << "bytes" << endl; // Выводит принятые данные.
        }
        else if (result == 0)
        {
            cout << "Connection closed" << endl; // Выводит сообщение о закрытии соединения.
        }
        else
        {
            cout << "Receive failed with error: " << WSAGetLastError() << endl; // Выводит сообщение об ошибке приема данных.
        }
    } while (result > 0);

    closesocket(ConnectSocket); // Закрывает сокет.
    freeaddrinfo(addrResult); // Освобождает память, выделенную для addrResult.
    WSACleanup(); // Очищает Winsock.

    return 0;
}