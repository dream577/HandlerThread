//
// Created by bronyna on 2024/7/28.
//

#include "Message.h"

namespace violet {
    static std::shared_ptr<MessagePool> mPool = std::make_shared<MessagePool>();

    Message::Message() {
        what = arg1 = arg2 = 0;
        when = 0;
        target = nullptr;
        next = nullptr;
        run = nullptr;
        dispatchMessage = nullptr;
    }

    Message *Message::obtain() {
        std::shared_ptr<MessagePool> pool = mPool;
        return pool->obtain();
    }

    void Message::recycle(Message *msg) {
        std::shared_ptr<MessagePool> pool = mPool;
        pool->recycle(msg);
    }


    Message::~Message() {
        what = arg1 = arg2 = 0;
        when = 0;
        target = nullptr;
        next = nullptr;
        run = nullptr;
        dispatchMessage = nullptr;
    }


    MessagePool::MessagePool() {
        pthread_mutex_init(&mutex, nullptr);
        messages = nullptr;
        size = 0;
    }

    Message *MessagePool::obtain() {
        Message *msg = nullptr;
        pthread_mutex_lock(&mutex);
        if (messages == nullptr) {
            msg = new Message;
        } else {
            msg = messages;
            messages = msg->next;
            msg->next = nullptr;
            size--;
        }
        pthread_mutex_unlock(&mutex);
        return msg;
    }

    void MessagePool::recycle(Message *msg) {
        if (msg == nullptr) return;
        pthread_mutex_lock(&mutex);

        if (size < MAX_MESSAGE_SIZE) {
            msg->next = messages;
            messages = msg;
            size++;
        } else {
            delete msg;
        }

        pthread_mutex_unlock(&mutex);
    }

    MessagePool::~MessagePool() {
        while (messages != nullptr) {
            Message *msg = messages;
            messages = msg->next;
            msg->next = nullptr;
            delete msg;
            size--;
        }

        pthread_mutex_destroy(&mutex);
    }
} // violet
