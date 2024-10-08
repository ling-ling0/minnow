#include "byte_stream.hh"

using namespace std;

// use string ////////////////////////
/*
ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), error_(false), used_capacity_(0), available_capacity_(capacity), \
bytes_poped_(0), bytes_pushed_(0), is_closed_(false), is_finished_(false), stream_("") {}

bool Writer::is_closed() const
{
  return is_closed_;
}

void Writer::push( string data )
{
  const uint64_t data_size_ = data.length();

  if(data_size_==0) return;

  //if data len > available size, fill stream_ and lose others
  if(data_size_>available_capacity_)
  {
    stream_+=data.substr(0,available_capacity_);
    used_capacity_+=available_capacity_;
    bytes_pushed_+=available_capacity_;
    available_capacity_=0;
  }
  // available size is enough
  else
  {
    stream_+=data;
    used_capacity_+=data_size_;
    available_capacity_-=data_size_;
    bytes_pushed_+=data_size_;
  }
  return;
}

void Writer::close()
{
  is_closed_=true;

  if(used_capacity_==0)
    is_finished_=true;
  return;
}

uint64_t Writer::available_capacity() const
{
  return available_capacity_;
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  return is_finished_;
}

uint64_t Reader::bytes_popped() const
{
  return bytes_poped_;
}

string_view Reader::peek() const
{
  return stream_;
}

void Reader::pop( uint64_t len )
{
  // pop len < used size, get substr(len,)
  if(len<used_capacity_)
  {
    stream_=stream_.substr(len);
    used_capacity_-=len;
    available_capacity_+=len;
    bytes_poped_+=len;
  }
  else  // if pop len > used size, clear stream_
  {
    stream_={};
    available_capacity_+=used_capacity_;
    bytes_poped_+=used_capacity_;
    used_capacity_=0;
  }

  // check and set is_finished
  if(is_closed_&&used_capacity_==0) is_finished_=true;

  return;
}

uint64_t Reader::bytes_buffered() const
{
  return used_capacity_;
}
*/

// use queue<char16_t>//////////////////////////////

/*
ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), error_(false), used_capacity_(0), available_capacity_(capacity), \
bytes_poped_(0), bytes_pushed_(0), is_closed_(false), is_finished_(false), stream_() {}

bool Writer::is_closed() const
{
  return is_closed_;
}

void Writer::push( string data )
{
  const uint64_t data_size_ = data.length();

  if(data_size_==0) return;

  uint64_t i=0;
  while(i<data_size_&&available_capacity_>0)
  {
    stream_.push(data[i]);
    used_capacity_+=1;
    bytes_pushed_+=1;
    available_capacity_-=1;
    i++;
  }
  return;
}

void Writer::close()
{
  is_closed_=true;
  if(used_capacity_==0)
    is_finished_=true;
  return;
}

uint64_t Writer::available_capacity() const
{
  return available_capacity_;
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  return is_finished_;
}

uint64_t Reader::bytes_popped() const
{
  return bytes_poped_;
}

string_view Reader::peek() const
{
  static string result;
  result.clear();
  std::queue<char16_t> temp = stream_;
  for(uint64_t i=0;i<used_capacity_;i++)
  {
    result+=temp.front();
    temp.pop();
  }
  return result;
}

void Reader::pop( uint64_t len )
{
  uint64_t i=0;
  while(i<len&&used_capacity_>0)
  {
    stream_.pop();
    used_capacity_-=1;
    available_capacity_+=1;
    bytes_poped_+=1;
    i++;
  }

  if(is_closed_&&used_capacity_==0) is_finished_=true;

  return;
}

uint64_t Reader::bytes_buffered() const
{
  return used_capacity_;
}

*/

// use deque <std::string> /////////////////////////////////////
ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), error_(false), used_capacity_(0), available_capacity_(capacity), \
bytes_poped_(0), bytes_pushed_(0), is_closed_(false), is_finished_(false), stream_() {}

bool Writer::is_closed() const
{
  return is_closed_;
}

void Writer::push( string data )
{
  const uint64_t data_size_ = data.length();

  if(data_size_==0) return;

  if(data_size_>available_capacity_)
  {
    stream_.push_back(data.substr(0,available_capacity_));
    used_capacity_+=available_capacity_;
    bytes_pushed_+=available_capacity_;
    available_capacity_-=available_capacity_;
  }
  else 
  {
    stream_.push_back(data);
    used_capacity_+=data_size_;
    bytes_pushed_+=data_size_;
    available_capacity_-=data_size_;
  }
  return;
}

void Writer::close()
{
  is_closed_=true;
  if(used_capacity_==0)
    is_finished_=true;
  return;
}

uint64_t Writer::available_capacity() const
{
  return available_capacity_;
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  return is_finished_;
}

uint64_t Reader::bytes_popped() const
{
  return bytes_poped_;
}

string_view Reader::peek() const
{
  return stream_.front();
}

void Reader::pop( uint64_t len )
{
  if(len>=used_capacity_)
  {
    stream_ = {};
    available_capacity_+=used_capacity_;
    bytes_poped_+=used_capacity_;
    used_capacity_-=used_capacity_;
  }
  else
  {
    while(len>=stream_.front().size())
    {
      uint64_t poped_size = stream_.front().size();
      stream_.pop_front();
      used_capacity_-=poped_size;
      available_capacity_+=poped_size;
      bytes_poped_+=poped_size;
      len-=poped_size;
    }
    stream_.at(0) = stream_.at(0).substr(len);
    used_capacity_-=len;
    available_capacity_+=len;
    bytes_poped_+=len;
  }

  if(is_closed_&&used_capacity_==0) is_finished_=true;

  return;
}

uint64_t Reader::bytes_buffered() const
{
  return used_capacity_;
}