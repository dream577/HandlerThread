//
// Created by bronyna on 2024/7/28.
//

#include "MessageQueue.h"
#include "vtime.h"

namespace violet {
    MessageQueue::MessageQueue() {
        quitting = false;
        messages = nullptr;
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
    }

    void MessageQueue::enqueueMessage(Message *message) {
        if (message == nullptr) return;
        long when = message->when;
        Message *p = messages;

        pthread_mutex_lock(&mutex);
        if (!quitting) {
            if (p == nullptr || when == 0 || when < p->when) {
                message->next = p;
                messages = message;
            } else {
                Message *prev;
                for (;;) {
                    prev = p;
                    p = p->next;
                    if (p == nullptr || when < p->when) {
                        break;
                    }
                }
                message->next = p;
                prev->next = message;
            }
        } else {
            Message::recycle(messages);
        }

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    Message *MessageQueue::next() {
        long now = 0L;
        long timeout = 0;
        bool find = false;
        Message *msg = nullptr;;

        while (!quitting && !find) {
            pthread_mutex_lock(&mutex);
            if (timeout > 0) {
                struct timespec t = {timeout / 1000L, timeout % 1000L * 1000000L};
                pthread_cond_timedwait(&cond, &mutex, &t);
            } else if (timeout < 0) {
                pthread_cond_wait(&cond, &mutex);
            }

            now = CURRENT_MONOTONIC_TIME;
            msg = messages;
            if (msg != nullptr) {
                if (now < msg->when) {
                    timeout = msg->when - now;
                } else {
                    messages = messages->next;
                    msg->next = nullptr;
                    find = true;
                }
            } else {
                timeout = MAX_BLOCK_TIME_INS_MILLIS;
            }

            pthread_mutex_unlock(&mutex);
        }

        return msg;
    }

    void MessageQueue::quitSafely() {
        pthread_mutex_lock(&mutex);
        quitting = true;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    void MessageQueue::removeAllMessages() {
        pthread_mutex_lock(&mutex);
        Message *msg;
        for (;;) {
            if (messages == nullptr) break;
            msg = messages;
            messages = messages->next;
            Message::recycle(msg);
        }
        pthread_mutex_unlock(&mutex);
    }

    void MessageQueue::removeMessage(int what) {
        pthread_mutex_lock(&mutex);
        Message *msg = messages;
        Message *prev = nullptr;;
        for (;;) {
            if (msg == nullptr) break;
            if (msg->what == what) {
                Message *next = msg->next;
                Message::recycle(msg);
                if (prev != nullptr) {
                    prev->next = next;
                }
                msg = next;
            } else {
                prev = msg;
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    MessageQueue::~MessageQueue() {
        quitting = true;
        Message *msg;
        for (;;) {
            if (messages == nullptr) break;
            msg = messages;
            messages = messages->next;
            Message::recycle(msg);
        }
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }
} // violet
