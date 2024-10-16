#include <iostream>
#include <cstdlib>  
#include <cstring>  
#include <unistd.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

void errHandler(const char * why, const int exitCode = 1) {
    cerr << why << endl;
    exit(exitCode);
}

int main() {
    sockaddr_in selfAddr;
    selfAddr.sin_family = AF_INET; 
    selfAddr.sin_port = 0; 
    selfAddr.sin_addr.s_addr = INADDR_ANY;

    sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(7); 
    remoteAddr.sin_addr.s_addr = inet_addr("172.16.40.1"); 

    char buf[256];
    strcpy(buf, "GODLESSishere"); 
    int msgLen = strlen(buf); 

    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == -1) {
        errHandler("Ошибка открытия сокета", 11);
    }

    int rc = bind(mySocket, (const sockaddr*) &selfAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Ошибка привязки локального адреса сокета", 12);
    }

    rc = connect(mySocket, (const sockaddr*) &remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Ошибка подключения сокета к удаленному серверу", 13);
    }

    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1) {
        close(mySocket);
        errHandler("Ошибка отправки сообщения", 14);
    }
    cout << "Алабастер: " << buf << endl;

    rc = recv(mySocket, buf, sizeof(buf), 0);
    if (rc == -1) {
        close(mySocket);
        errHandler("Ошибка получения ответа", 15);
    }
    buf[rc] = '0';
    cout << "итак: " << buf << endl;

    close(mySocket);

    return 0;
}
