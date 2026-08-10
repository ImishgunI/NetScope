#include "./capture/capture.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./bin_file <ethernet_interface>";
        return 1;
    }
    const char* interface_name = argv[1];
    EthernetFrameCapturer::captureFrames(interface_name);
    return 0;
}
