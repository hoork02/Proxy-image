#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <vector>
#include <mutex>
#include <queue>
#include <algorithm>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "ws2_32.lib")

// Define the packet structure
struct Packet {
    uint32_t packet_id;
    std::vector<uint8_t> data;
};

// Comparison function for the heap
struct ComparePackets {
    bool operator()(const Packet& packet1, const Packet& packet2) const {
        return packet1.packet_id > packet2.packet_id;
    }
};

// Global variables
std::priority_queue<Packet, std::vector<Packet>, ComparePackets> packet_heap;
std::mutex packet_heap_mutex;
std::condition_variable data_cv;

void send_ack(SOCKET client_socket) {
    char ack = 0x01;
    send(client_socket, &ack, 1, 0);
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Server socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9989); // Port to listen on
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Binding failed." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listening failed." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    while (true) {

        std::cout << " server listenining............."; std::cout << std::endl;
        SOCKET client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Client connection failed." << std::endl;
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }

        int num_rows;
        int num_cols;

        recv(client_socket, reinterpret_cast<char*>(&num_rows), sizeof(num_rows), 0);
        recv(client_socket, reinterpret_cast<char*>(&num_cols), sizeof(num_cols), 0);

        // Process the received image dimensions (num_rows and num_cols)

        while (true) {
            Packet packet;
            recv(client_socket, reinterpret_cast<char*>(&packet.packet_id), sizeof(packet.packet_id), 0);

            // Receive the image data
            std::vector<uint8_t> image_data;
            char buffer[1024]; // Adjust buffer size as needed
            int bytes_received = 0;

            while (bytes_received < num_rows * num_cols * 3) {
                int recv_result = recv(client_socket, buffer, sizeof(buffer), 0);
                if (recv_result <= 0) {
                    std::cerr << "Failed to receive image data for packet " << packet.packet_id << std::endl;
                    break;
                }
                image_data.insert(image_data.end(), buffer, buffer + recv_result);
                bytes_received += recv_result;
                std::cout << "recieved bytes are::" << bytes_received << std::endl;
            }

            // Populate the received packet and insert it into the heap
            packet.data = image_data;
            {
                std::lock_guard<std::mutex> lock(packet_heap_mutex);
                packet_heap.push(packet);
            }

            // Send acknowledgment to the client
            send_ack(client_socket);
        }

        closesocket(client_socket);
     
        
    }
   
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
