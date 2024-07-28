#include <iostream>

#include "HandlerThread.h"
#include "vtime.h"

using namespace violet;

void handleMessage(Message *message) {
    if (message == nullptr) return;
    std::cout << "now=" << CURRENT_MONOTONIC_TIME << " ,what=" << message->what << std::endl;
    Message::recycle(message);
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto *thread = new HandlerThread();
    Handler *handler = nullptr;
    if (thread->start() == 0) {
        for (;;) {
            handler = thread->obtainHandler();
            if (handler == nullptr) {
                usleep(50);
            } else {
                handler->setHandleMessageCallback(&handleMessage);
                break;
            }
        }
    }
    std::cout << "now=" << CURRENT_MONOTONIC_TIME << std::endl;
    int count = 0;;
    if (handler != nullptr) {
        while (true) {
            handler->sendMessageDelayed(count, 1000);
            handler->sendMessageDelayed(count, 1500);
            count++;
            if (count > 100) break;
            sleep(2);
        }
    }
    thread->quitSafely();
    delete thread;
    std::cout << "stop at " << CURRENT_MONOTONIC_TIME << std::endl;
    return 0;
}
