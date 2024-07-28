//
// Created by bronyna on 2024/7/28.
//

#ifndef LOOPER_H
#define LOOPER_H

#include "MessageQueue.h"

namespace violet {
    class Looper {

    public:
        std::shared_ptr<MessageQueue> mQueue;

        Looper();

        void loop();

        void quitSafely();

        ~Looper();
    private:

        bool loopOnce();
    };
} // violet

#endif //LOOPER_H
