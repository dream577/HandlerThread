//
// Created by bronyna on 2024/7/28.
//

#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H


#include "Message.h"

#define  MAX_BLOCK_TIME_INS_MILLIS  15000

namespace violet {
    class MessageQueue {
    public:
        MessageQueue();

        void enqueueMessage(Message *msg);

        Message *next();

        void quitSafely();

        void removeAllMessages();

        void removeMessage(int what);

        ~MessageQueue();

    private:
        bool quitting;
        Message *messages;

        pthread_mutex_t mutex;
        pthread_cond_t cond;
    };
} // violet

#endif //MESSAGEQUEUE_H
