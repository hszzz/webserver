#include <iostream>

#include "../buffer.h"

void debug(const net::Buffer& buffer) {
  std::cout << "++debug: \n"
            << "++prependable: " << buffer.GetPrependableBytes()
            << " readable: " << buffer.GetReadableBytes()
            << " writable: " << buffer.GetWritableBytes() << std::endl;
}

int main() {
  std::cout << "buffer test" << std::endl;
  net::Buffer buffer(16);
  debug(buffer);
  for (int i = 0; i < 10; ++i) {
    buffer.append("123456");
    std::cout << buffer.RetrieveAllAsString() << std::endl;
  }

  debug(buffer);
  for (int i = 0; i < 3; ++i) {
    buffer.append("123456789987654321");
    std::cout << buffer.RetrieveAllAsString() << std::endl;
  }
  debug(buffer);

  net::Buffer buffer1(16);
  debug(buffer1);
  for (int i = 0; i < 3; ++i) {
    buffer1.prepend("12345678", 8);
    buffer1.append("123321123321");
    std::cout << buffer1.RetrieveAllAsString() << std::endl;
  }
  debug(buffer1);
}
