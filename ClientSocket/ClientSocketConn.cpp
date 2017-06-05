#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_SERVER_NAME "127.0.0.1"
#define DEFAULT_BUFFER_LENGTH 512
struct Input_ServerInfo {
	PCSTR serverName;
	PCSTR port;
	explicit Input_ServerInfo(
		PCSTR ServerName = DEFAULT_SERVER_NAME,
		PCSTR Port = DEFAULT_PORT) :
		serverName(ServerName),
		port(Port) {}
};

void GetInput_ServerInfo(Input_ServerInfo *insrvinfo, int argc, char* argv[]);
PCSTR GetWSAErrorCodeDesc(const int& LastErrorCode);

int main(int argc, char* argv[]) {
	int lastError = 0;

	Input_ServerInfo srvInfo;
	//Get command line input
	try {
		GetInput_ServerInfo(&srvInfo, argc, argv);
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	WSADATA wsaData;
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed: " << iResult << std::endl;
		return 1;
	}

	//create socket for the client
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC; //can be either IPv6 or IPv4 address
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	// Resolve the server address and port
	iResult = getaddrinfo(srvInfo.serverName, srvInfo.port, &hints, &result);
	if (iResult != 0) {
		std::cerr << "getaddrinfo failed! Error (" << iResult << "): " << GetWSAErrorCodeDesc(iResult) << std::endl;
		WSACleanup();
		return 1;
	}
	// Create socket object
	SOCKET ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a SOCKET for connecting to the server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			lastError = WSAGetLastError();
			std::cerr << "Error at socket()! Error (" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
			WSACleanup();
			return 1;
		}
		// Connect to server
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			lastError = WSAGetLastError();
			std::cerr << "Unable to connect to server! Error (" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return 1;
	}

	int recvbuflen = DEFAULT_BUFFER_LENGTH;
	char *sendbuf =  "this is a test";
	char recvbuf[DEFAULT_BUFFER_LENGTH];

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		lastError = WSAGetLastError();
		std::cerr << "send failed! Error (" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Bytes Sent: " << iResult << std::endl;

	// Shutdown sending
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		lastError = WSAGetLastError();
		std::cerr << "shutdown failed! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive dat until the server closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			std::cout << "Bytes received: " << iResult << std::endl;
		}
		else if (iResult == 0)
			std::cout << "Connection closed" << std::endl;
		else {
			lastError = WSAGetLastError();
			std::cout << "recv failed! Error(" << lastError << "): " << GetWSAErrorCodeDesc(lastError) << std::endl;
		}
	} while (iResult > 0);


	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
	char a;
	std::cin >> a;


	return 0;
}

void GetInput_ServerInfo(Input_ServerInfo *insrvinfo, int argc, char* argv[]) {
	switch (argc) {
	case 2:
		insrvinfo->serverName = argv[1];
		break;
	case 3:
		insrvinfo->serverName = argv[1];
		insrvinfo->port = argv[2];
		break;
	}
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
	default:
		return "A description for code is not known to this application.";
		break;
	}
}