#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include "SnakeFood.h"

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _condition.wait(uLock, [this] { return !_queue.empty(); });

    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> uLock(_mutex);
    std::cout << "Food x: " << msg.x << " Food y: " << msg.y << " has been sent to the queue" << std::endl;
    _queue.clear();
    _queue.emplace_back(std::move(msg));
    _condition.notify_one();
}

SnakeFood::SnakeFood(std::size_t grid_width, std::size_t grid_height) : 
    _snake(grid_width, grid_height),
    engine(dev()),
    random_w(0, static_cast<int>(grid_width - 1)),
    random_h(0, static_cast<int>(grid_height - 1)) {
    _snakeFoodQueue = std::make_shared<MessageQueue<SnakeFoodPos>>(); 
}

void SnakeFood::simulate() {
    std::thread t(&SnakeFood::cyclePlaceFood, this);
    t.join();
}

void SnakeFood::cyclePlaceFood() {
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        placeFood();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

void SnakeFood::updateSnake(Snake snake) {
    _snake = snake;
}

SnakeFoodPos SnakeFood::receiveNewFood() {
    SnakeFoodPos p;
    while(true) {
        p = _snakeFoodQueue->receive();
        if (!_snake.SnakeCell(p.x, p.y)) {
            return p;
        }
    }
    return p;
}

void SnakeFood::placeFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!_snake.SnakeCell(x, y)) {
        SnakeFoodPos pos {x,y};
      _snakeFoodQueue->send(std::move(pos));
      return;
    }
  }
}