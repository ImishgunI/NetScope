#include "./capture/capture.hpp"
#include <array>
#include <cstdint>
#include <format>
#include <iostream>

void print(std::array<uint8_t, 4096>& arr) {
    std::cout << std::format("{:02x}", arr[0]) << " " << std::format("{:02x}", arr[1]) << " "
              << std::format("{:02x}", arr[2]) << " " << std::format("{:02x}", arr[3]) << " "
              << std::format("{:02x}", arr[4]) << " " << std::format("{:02x}", arr[5]) << " "
              << std::format("{:02x}", arr[6]) << " " << std::format("{:02x}", arr[7]) << " "
              << std::format("{:02x}", arr[8]) << " " << std::format("{:02x}", arr[9]) << " "
              << std::format("{:02x}", arr[10]) << " " << std::format("{:02x}", arr[11]) << " "
              << std::format("{:02x}", arr[12]) << " " << std::format("{:02x}", arr[13]) << " "
              << std::format("{:02x}", arr[14]) << " " << std::format("{:02x}", arr[15]) << '\n';
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./bin_file <ethernet_interface>";
        return 1;
    }
    const char* interface_name = argv[1];
    EthernetFrameCapturer enp11s0(interface_name);
    enp11s0.captureLoop(print);
    return 0;
}
