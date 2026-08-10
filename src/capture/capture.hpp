#pragma once

#include <string>

class EthernetFrameCapturer {
  public:
    EthernetFrameCapturer() = default;
    ~EthernetFrameCapturer() = default;

    static void captureFrames(const std::string& interface_name);
};
