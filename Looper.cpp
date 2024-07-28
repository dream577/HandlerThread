//
// Created by bronyna on 2024/7/28.
//

#include "Looper.h"

#include <memory>

namespace violet {
    Looper::Looper() {
        mQueue = std::make_shared<MessageQueue>();
    }

    void Looper::loop() {
        for (;;) {
            if (!loopOnce()) break;
        }
    }

    bool Looper::loopOnce() {
        std::shared_ptr<MessageQueue> queue = mQueue;
        if (queue == nullptr) return false;
        Message *msg = queue->next();
        if (msg == nullptr || msg->target == nullptr || msg->dispatchMessage == nullptr) return false;
        msg->dispatchMessage(msg->target, msg);
        return true;
    }

    void Looper::quitSafely() {
        std::shared_ptr<MessageQueue> queue = mQueue;
        queue->quitSafely();
    }

    Looper::~Looper() {
        mQueue.reset();
    }
} // violet
