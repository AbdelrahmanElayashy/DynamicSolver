cmake_minimum_required(VERSION 3.10.2)
project(lotka2 VERSION 1.0.0)
set(GCC_EXPECTED_VERSION7 8.4.0)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)


add_executable(program ApproxHeun.cpp
ApproxRK3.cpp
ApproxRK4.cpp
BlockingQueue.cpp
EquationContainer.cpp
EquationsInScheduler.cpp
ExplicitEuler.cpp
ExplicitFourStep.cpp
ExplicitThreeStep.cpp
ExplicitTwoStep.cpp
LotkaModell.cpp
main.cpp
MethodContainer.cpp
MethodsInScheduler.cpp
MyLibrary.cpp
Priority_SafeQueue.cpp
RK2.cpp
RK3.cpp
RK4.cpp
Scheduler.cpp
ThreadsPool.cpp
EinspurModell.cpp
main.cpp)

target_link_libraries(program Threads::Threads)