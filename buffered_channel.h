#ifndef BUFFERED_CHANNEL__BUFFERED_CHANNEL_H_
#define BUFFERED_CHANNEL__BUFFERED_CHANNEL_H_

#include <queue>
#include <mutex>
#include <utility>
#include <condition_variable>

template<class T>
class BufferedChannel {
 public:
  explicit BufferedChannel(size_t size) : buffer_size(size), IsClosed(false) {}

  void Send(T value) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (IsClosed) {
      throw std::runtime_error("Channel is close");
    }
    if (queue_.size() < buffer_size) {
      queue_.push(std::move(value));
      recv_var.notify_one();
    } else {
      while (queue_.size() == buffer_size && !IsClosed) {
        send_var.wait(lock);
      }
      if (IsClosed) {
        throw std::runtime_error("Channel is close");
      } else {
        queue_.push(std::move(value));
        recv_var.notify_one();
      }
    }
  }

  std::pair<T, bool> Recv() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.size() == 0 && !IsClosed) {
      recv_var.wait(lock);
    }
    if (queue_.size() == 0) {
      return std::make_pair(T(), false);
    }
    T res = queue_.front();
    queue_.pop();
    send_var.notify_one();
    return std::make_pair(res, true);
  }

  void Close() {
    std::unique_lock<std::mutex> lock(mutex_);
    IsClosed = true;
    send_var.notify_all();
    recv_var.notify_all();
  }

 private:
  size_t buffer_size;
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable send_var;
  std::condition_variable recv_var;
  bool IsClosed;
};

#endif //BUFFERED_CHANNEL__BUFFERED_CHANNEL_H_
