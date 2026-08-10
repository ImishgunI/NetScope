#include "capture.hpp"
#include <arpa/inet.h>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <format>
#include <iostream>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/sockios.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

void EthernetFrameCapturer::captureFrames(const std::string& interface_name) {
    // socket нужен для создания конечного соединения и возврата файлового дескриптора(описателя)
    // domain - параметр для указания семейства протоколов, которые будут использоваться для создания соединения
    // AF_PACKET - низкоуровневый пакетный интерфейс. По факту как раз для низких уровней модели оси типо Ethernet
    // кадров type - параметр который задает семантику коммуникаций. SOCK_RAW обеспечивает доступ к низкоуровневому
    // сетевому протоколу protocol - задает конкретный протокол с которым работает сокет. ETH_P_ALL - это протокол
    // который указывает, что мы принимаем все пакеты до того как они попадут в ядро htons функция которая переводит
    // хостовой порядок байт в сетевой порядок байт. Потому что в интернете используется Big-endian
    int file_d = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (file_d < 0) {
        std::cerr << "Failed to open socket";
        return;
    }

    struct sockaddr_ll sll{};
    std::memset(&sll, 0, sizeof(sll));
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = static_cast<int>(if_nametoindex(interface_name.c_str()));
    sll.sll_protocol = htons(ETH_P_ALL);

    // bind позволяет принимать пакеты для сокета от конкретного интерфейса
    if (bind(file_d, (struct sockaddr*)&sll, sizeof(sll)) < 0) {
        std::cerr << "Failed to bind interface to socket";
        close(file_d);
        return;
    }

    const unsigned int buffer_size = 4096;
    std::array<uint8_t, buffer_size> buffer;
    socklen_t socket_len = sizeof(sll);
    uint32_t frame_number = 0;
    const uint64_t milliseconds_to_sleep = 500;
    while (true) {
        ssize_t frame_length = recvfrom(file_d, buffer.data(), buffer_size, 0, (struct sockaddr*)&sll, &socket_len);
        if (frame_length < 0) {
            std::cerr << "Failed to get data from socket";
            break;
        }
        std::cout << "#" << ++frame_number << " " << frame_length << " " << std::format("{:02x}", buffer[0]) << " "
                  << std::format("{:02x}", buffer[1]) << " " << std::format("{:02x}", buffer[2]) << " "
                  << std::format("{:02x}", buffer[3]) << " " << std::format("{:02x}", buffer[4]) << " "
                  << std::format("{:02x}", buffer[5]) << " " << std::format("{:02x}", buffer[6]) << " "
                  << std::format("{:02x}", buffer[7]) << " " << std::format("{:02x}", buffer[8]) << " "
                  << std::format("{:02x}", buffer[9]) << " " << std::format("{:02x}", buffer[10]) << " "
                  << std::format("{:02x}", buffer[11]) << " " << std::format("{:02x}", buffer[12]) << " "
                  << std::format("{:02x}", buffer[13]) << " " << std::format("{:02x}", buffer[14]) << " "
                  << std::format("{:02x}", buffer[15]) << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_to_sleep));
    }
    close(file_d);
}
