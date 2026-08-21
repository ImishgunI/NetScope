#pragma once

#include <cstdint>
#include <linux/if_packet.h>
#include <string>

class EthernetFrameCapturer {
  public:
    EthernetFrameCapturer(std::string interface_name);
    ~EthernetFrameCapturer();
    void requestStop();
    void captureLoop(void (*handler)(std::array<uint8_t, 4096>&));

  private:
    void openSocket();
    void closeSocket() const;

  private:
    int file_d;
    std::string interface_name;
    bool isStop = true;
    struct sockaddr_ll sll;
};
