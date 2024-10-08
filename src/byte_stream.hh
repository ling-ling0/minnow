#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include <queue>
#include <iostream>
#include <cstring>

class Reader;
class Writer;

class ByteStream
{
public:
  explicit ByteStream( uint64_t capacity );

  // Helper functions (provided) to access the ByteStream's Reader and Writer interfaces
  Reader& reader();
  const Reader& reader() const;
  Writer& writer();
  const Writer& writer() const;

  void set_error() { error_ = true; };       // Signal that the stream suffered an error.
  bool has_error() const { return error_; }; // Has the stream had an error?

protected:
  // Please add any additional state to the ByteStream here, and not to the Writer and Reader interfaces.
  uint64_t capacity_;     // 构建时默认的容量
  bool error_ {};         // 是否出现 error

  uint64_t used_capacity_; // 使用的容量，也就是当前 bytes 的数量
  uint64_t available_capacity_;// 可用的容量
  uint64_t bytes_poped_;  // 总共 pop 的 bytes
  uint64_t bytes_pushed_; // 总共 push 的 bytes
  bool is_closed_;        // 是否关闭流，由输出方决定
  bool is_finished_;      // 是否结束联通，由接收方结束

  // use string to store bytes
  // std::string stream_{};
  // std::queue<char16_t> stream_;
  std::deque<std::string> stream_;

};

class Writer : public ByteStream
{
public:
  void push( std::string data ); // Push data to stream, but only as much as available capacity allows.
  void close();                  // Signal that the stream has reached its ending. Nothing more will be written.

  bool is_closed() const;              // Has the stream been closed?
  uint64_t available_capacity() const; // How many bytes can be pushed to the stream right now?
  uint64_t bytes_pushed() const;       // Total number of bytes cumulatively pushed to the stream
};

class Reader : public ByteStream
{
public:
  std::string_view peek() const; // Peek at the next bytes in the buffer
  void pop( uint64_t len );      // Remove `len` bytes from the buffer

  bool is_finished() const;        // Is the stream finished (closed and fully popped)?
  uint64_t bytes_buffered() const; // Number of bytes currently buffered (pushed and not popped)
  uint64_t bytes_popped() const;   // Total number of bytes cumulatively popped from stream
};

/*
 * read: A (provided) helper function thats peeks and pops up to `len` bytes
 * from a ByteStream Reader into a string;
 */
void read( Reader& reader, uint64_t len, std::string& out );
