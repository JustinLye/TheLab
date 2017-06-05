#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFFER_LENGTH 512

PCSTR GetWSAErrorCodeDesc(const int& LastErrorCode);

int main(int argc, char* argv[]) {
	// Check cmd line for port#
	PCSTR port = nullptr;
	argc < 2 ? port = DEFAULT_PORT : port = argv[1];

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	int lastError = 0;
	if (iResult != 0) {
		lastError = WSAGetLastError();
		std::cerr << "WSAStartup failed! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
		return 1;
	}

	struct addrinfo *result = nullptr, *ptr = nullptr, hints;

	// Create socket for server
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(nullptr, port, &hints, &result);
	if (iResult != 0) {
		lastError = WSAGetLastError();
		std::cerr << "getaddrinfo failed! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for the server to listen for client connections
	SOCKET ListenSocket = INVALID_SOCKET;
	
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		lastError = WSAGetLastError();
		std::cerr << "Error at socket()! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		lastError = WSAGetLastError();
		std::cerr << "bind failed! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	//Listen on socket
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		lastError = WSAGetLastError();
		std::cerr << "listen failed! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	SOCKET ClientSocket = INVALID_SOCKET;

	// Accept client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		lastError = WSAGetLastError();
		std::cerr << "accept failed! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ListenSocket);

	// Typically servers would hand this off to a worker thread

	char recvbuf[DEFAULT_BUFFER_LENGTH];
	int iSendResult = 0;
	int recvbuflen = DEFAULT_BUFFER_LENGTH;

	// Receive until the peer shuts down the connection
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			std::cout << "Bytes received: " << iResult << std::endl;

			// Echo buffer back to sender
			iSendResult = send(ClientSocket, recvbuf, recvbuflen, 0);
			if (iSendResult == SOCKET_ERROR) {
				lastError = WSAGetLastError();
				std::cerr << "send failed! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			std::cout << "Bytes sent: " << iSendResult << std::endl;
		} else if (iResult == 0) {
			std::cout << "Connection closing..." << std::endl;
		} else {
			lastError = WSAGetLastError();
			std::cerr << "recv failed! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);

	// shutdown send
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		lastError = WSAGetLastError();
		std::cerr << "shutdown failed! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}

PCSTR GetWSAErrorCodeDesc(const int& LastErrorCode) {
	switch (LastErrorCode) {
	case WSAEFAULT:
		return "Bad address. The server address is invalid (i.e. the address is the wrong size or of a different form than was expected).";
		break;
	case WSAHOST_NOT_FOUND:
		return "Host not found. No such host is known. The name is not an offical host name or alias, or it cannot be found in the database(s) being queried.";
		break;
	case WSAECONNREFUSED:
		return "Connection refused. This usually results from trying to connect to a service that is inactive on the foreign host--that is, one with no server application running.";
		break;
	case WSAENOTSOCK:
		return "Socket operation on nonsocket. Handle to socket is not valid.";
		break;
	default:
		return "A description for code is not known to this application.";
		break;
	}
}