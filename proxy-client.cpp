#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "ws2_32.lib")

// Define the packet structure
struct Packet {
    uint32_t packet_id;
    std::vector<uint8_t> data;
};

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

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9988); // Port of the proxy server
    if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) <= 0) {
        std::cerr << "Invalid address/Address not supported" << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Open the image file for reading
    std::string image_path = "C:\\Users\\hoork\\Desktop\\semester 3\\OpenCV-client\\book.jpg"; // Replace with the actual image path
    cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);

    if (image.empty()) {
        std::cerr << "Failed to open the image file." << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    int num_rows = image.rows;
    int num_cols = image.cols;

    // Send image dimensions
    send(client_socket, reinterpret_cast<const char*>(&num_rows), sizeof(num_rows), 0);
    send(client_socket, reinterpret_cast<const char*>(&num_cols), sizeof(num_cols), 0);

    // Create and send packets with packet_id and image data
    uint32_t packet_id = 0;
    std::vector<uint8_t> buffer;

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            Packet packet;
            packet.packet_id = packet_id;
            packet_id++;

            buffer.push_back(static_cast<uint8_t>(image.at<cv::Vec3b>(i, j)[0]));
            buffer.push_back(static_cast<uint8_t>(image.at<cv::Vec3b>(i, j)[1]));
            buffer.push_back(static_cast<uint8_t>(image.at<cv::Vec3b>(i, j)[2]));

            packet.data = buffer;

            send(client_socket, reinterpret_cast<const char*>(&packet.packet_id), sizeof(packet.packet_id), 0);
            send(client_socket, reinterpret_cast<const char*>(packet.data.data()), packet.data.size(), 0);

            // Wait for acknowledgment from the server
            char ack;
            recv(client_socket, &ack, 1, 0);

            if (ack != 0x01) {
                std::cerr << "Packet " << packet.packet_id << " was not acknowledged." << std::endl;
                break;
            }
            else
            {
                std::cout << "Packet " << packet.packet_id << " was acknowledged." << std::endl;

            }
            buffer.clear();
        }
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
