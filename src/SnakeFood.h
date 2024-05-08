#ifndef SNAKEFOOD_H
#define SNAKEFOOD_H

#include <random>
#include <mutex>
#include <deque>
#include <condition_variable>
#include "SDL.h"
#include "snake.h"
#include "SnakeFood.h"


struct SnakeFoodPos
{
    int x = 0;
    int y = 0;
};

// Memory Management :
// 1. The project uses smart pointers instead of raw pointers.
// 2. The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
// 3. The project uses move semantics to move data instead of copying it, where possible.
// 4. The project uses smart pointers instead of raw pointers.

// Concurrency :
// 1. The project uses multithreading.
// 2. A promise and future is used in the project.
// 3. A mutex or lock is used in the project.
// 4. A condition variable is used in the project.

template <class T>
class MessageQueue
{
    public:
        T receive();
        void send(T &&msg);
    private:
        std::deque<SnakeFoodPos> _queue;
        std::condition_variable _condition;
        std::mutex _mutex;
};

// Object Oriented Programming :
// 1. One or more classes are added to the project with appropriate access specifiers for class members.
// 2. Class constructors utilize member initialization lists.
// 3. Classes abstract implementation details from their interfaces.
// 4. Overloaded functions allow the same function to operate on different parameters.
// 5. Classes follow an appropriate inheritance hierarchy with virtual and override functions.
// 6. Templates generalize functions or classes in the project.

class SnakeFood
{
    public:
        SnakeFood(std::size_t grid_width, std::size_t grid_height);

        void simulate();
        void cyclePlaceFood();
        void updateSnake(Snake snake);
        void placeFood();
        SnakeFoodPos receiveNewFood();
    private:
        

        std::random_device dev;
        std::mt19937 engine;
        std::uniform_int_distribution<int> random_w;
        std::uniform_int_distribution<int> random_h;

        Snake _snake;
        std::shared_ptr<MessageQueue<SnakeFoodPos>> _snakeFoodQueue;
};

#endif