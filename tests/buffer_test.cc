#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

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

  net::Buffer buffer2(16);
  buffer2.append("123xx123");
  const char* xx = buffer2.find("xx", 2);
  std::cout << xx << std::endl;
  std::cout << buffer2.peek() << std::endl;
  buffer2.RetrieveUntil(xx);
  std::cout << buffer2.peek() << std::endl;

  net::Buffer buffer3(1024000);
  int fd = ::open("/home/hszzz/webserver/tests/test.txt", O_RDONLY);
  if (fd <= 0) ::perror("error:");
  debug(buffer3);
  int err = 0;
  buffer3.ReadFd(fd, err);
  if (err != 0) ::perror("error:");
  debug(buffer3);
  std::cout << buffer3.RetrieveAllAsString() << std::endl;
  debug(buffer3);
}
