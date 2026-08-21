#include "capture.hpp"
#include "../util/SocketException.hpp"
#include <arpa/inet.h>
#include <array>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <linux/if_ether.h>
#include <linux/sockios.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

EthernetFrameCapturer::EthernetFrameCapturer(std::string interface_name) : interface_name(std::move(interface_name)) {
    try {
        openSocket();
    } catch (const SocketException& se) {
        std::cerr << se.what();
        requestStop();
    }
}

EthernetFrameCapturer::~EthernetFrameCapturer() { closeSocket(); }

void EthernetFrameCapturer::openSocket() {
    // socket нужен для создания конечного соединения и возврата файлового дескриптора(описателя)
    // domain - параметр для указания семейства протоколов, которые будут использоваться для создания соединения
    // AF_PACKET - низкоуровневый пакетный интерфейс. По факту как раз для низких уровней модели оси типо Ethernet
    // кадров type - параметр который задает семантику коммуникаций. SOCK_RAW обеспечивает доступ к низкоуровневому
    // сетевому протоколу protocol - задает конкретный протокол с которым работает сокет. ETH_P_ALL - это протокол
    // который указывает, что мы принимаем все пакеты до того как они попадут в ядро htons функция которая переводит
    // хостовой порядок байт в сетевой порядок байт. Потому что в интернете используется Big-endian
    file_d = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (file_d < 0) {
        throw SocketException("Failed to open socket\n");
    }

    struct sockaddr_ll sll{};
    std::memset(&sll, 0, sizeof(sll));
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = static_cast<int>(if_nametoindex(interface_name.c_str()));
    sll.sll_protocol = htons(ETH_P_ALL);

    // bind позволяет принимать пакеты для сокета от конкретного интерфейса
    if (bind(file_d, (struct sockaddr*)&sll, sizeof(sll)) < 0) {
        throw SocketException("Failed to bind interface to socket\n");
    }
}

void EthernetFrameCapturer::closeSocket() const {
    if (file_d < 0) {
        return;
    }
    int isClose = close(file_d);
    if (isClose < 0) {
        std::cerr << strerror(errno);
    }
}

void EthernetFrameCapturer::requestStop() { isStop = false; }

void EthernetFrameCapturer::captureLoop(void (*handler)(std::array<uint8_t, 4096>&)) {
    const unsigned int buffer_size = 4096;
    std::array<uint8_t, buffer_size> buffer;
    socklen_t socket_len = sizeof(sll);
    int milliseconds_to_sleep = 500;
    while (isStop) {
        ssize_t frame_length = recvfrom(file_d, buffer.data(), buffer_size, 0, (struct sockaddr*)&sll, &socket_len);
        if (frame_length < 0) {
            std::cerr << "Failed to get data from socket";
            break;
        }
        handler(buffer);
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_to_sleep));
    }
}
