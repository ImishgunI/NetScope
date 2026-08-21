#pragma once

#include <exception>
#include <string>
class SocketException : public std::exception {
  private:
    std::string msg;

  public:
    SocketException(const char* msg) : msg(msg) {};

    const char* what() const noexcept { return msg.c_str(); }
};
