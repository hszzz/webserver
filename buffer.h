#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>

namespace net {

/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode

class Buffer {
 public:
  static const size_t kCheapPrepend = 8;
  static const size_t kInitialSize = 1024;

  explicit Buffer(size_t initial = kInitialSize)
      : buffer_(kCheapPrepend + initial),
        reader_index_(kCheapPrepend),
        writer_index_(kCheapPrepend) {}

  ~Buffer() = default;

  size_t GetReadableBytes() const { return writer_index_ - reader_index_; }
  size_t GetWritableBytes() const { return buffer_.size() - writer_index_; }
  size_t GetPrependableBytes() const { return reader_index_; }

  char* BeginRead() { return begin() + reader_index_; }
  char* BeginWrite() { return begin() + writer_index_; }
  const char* BeginWrite() const { return begin() + writer_index_; }

  void HasWrite(size_t len);

  void append(const char* data, size_t len);
  void append(const std::string& str);
  void append(const void* data, size_t len);

  void prepend(const void* data, size_t len);

  void EnsureWritableBytes(size_t len);

  size_t ReadFd(int fd, int& saved_errno);

  const char* peek() const { return begin() + reader_index_; }

  void retrieve(size_t len);
  std::string RetrieveAllAsString();
  void RetrieveUntil(const char* end);

  const char* find(const char* pattern, int len) const;

 private:
  char* begin() { return &*buffer_.begin(); }
  const char* begin() const { return &*buffer_.begin(); }

  void MakeSpace(size_t len);

 private:
  std::vector<char> buffer_;
  size_t reader_index_;
  size_t writer_index_;
};

}  // namespace net

#endif  // BUFFER_H
