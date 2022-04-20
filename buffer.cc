#include <assert.h>
#include <sys/uio.h>

#include "buffer.h"

namespace net {

void Buffer::HasWrite(size_t len) { writer_index_ += len; }

void Buffer::append(const char* data, size_t len) {
  EnsureWritableBytes(len);
  std::copy(data, data + len, BeginWrite());
  HasWrite(len);
}

void Buffer::append(const std::string& str) { append(str.data(), str.size()); }

void Buffer::append(const void* data, size_t len) {
  append(static_cast<const char*>(data), len);
}

void Buffer::prepend(const void* data, size_t len) {
  assert(len <= GetPrependableBytes());
  reader_index_ -= len;
  const char* d = static_cast<const char*>(data);
  std::copy(d, d + len, begin() + reader_index_);
}

void Buffer::EnsureWritableBytes(size_t len) {
  if (GetWritableBytes() < len) {
    MakeSpace(len);
  }
  assert(GetWritableBytes() >= len);
}

size_t Buffer::ReadFd(int fd, int& saved_errno) {
  char extra_buf[65535];
  struct iovec vec[2];
  const size_t writable = GetWritableBytes();
  vec[0].iov_base = begin() + writer_index_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extra_buf;
  vec[1].iov_len = sizeof(extra_buf);

  const int iovcnt = (writable < sizeof(extra_buf)) ? 2 : 1;
  const ssize_t n = ::readv(fd, vec, iovcnt);
  if (n < 0) {
    saved_errno = errno;
  } else if (static_cast<size_t>(n) <= writable) {
    writer_index_ += n;
  } else {
    writer_index_ = buffer_.size();
    append(extra_buf, n - writable);
  }

  return n;
}

void Buffer::MakeSpace(size_t len) {
  // 空间不够
  if (GetWritableBytes() + GetPrependableBytes() < len + kCheapPrepend) {
    buffer_.resize(writer_index_ + len);
  } else {
    // 空间够，但是需要内部腾挪
    size_t readable = GetReadableBytes();
    std::copy(begin() + reader_index_, begin() + writer_index_,
              begin() + kCheapPrepend);
    reader_index_ = kCheapPrepend;
    writer_index_ = reader_index_ + readable;
  }
}

void Buffer::retrieve(size_t len) {
  if (len <= GetReadableBytes()) {
    reader_index_ += len;
  } else {
    reader_index_ = kCheapPrepend;
    writer_index_ = kCheapPrepend;
  }
}

std::string Buffer::RetrieveAllAsString() {
  std::string str(peek(), GetReadableBytes());
  retrieve(GetReadableBytes());
  return str;
}

}  // namespace net
