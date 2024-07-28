//
// Created by bronyna on 2024/7/28.
//

#ifndef MESSAGE_H
#define MESSAGE_H
#include <memory>
#include <pthread.h>

namespace violet {

#define  MAX_MESSAGE_SIZE  50

    class Message {
    public:
        int what; // please use numbers greater than 0
        int arg1;
        int arg2;
        long when; // used to mark the time the message should be processed

        void *target; // the handler that sends the messge
        Message *next; // next message

        void (*run)();

        void (*dispatchMessage)(void *target, Message *msg);

        Message();

        ~Message();

        static Message *obtain();

        static void recycle(Message *msg);
    };

    class MessagePool {
    public:
        MessagePool();

        Message *obtain();

        void recycle(Message *msg);

        ~MessagePool();

    private:
        pthread_mutex_t mutex;

        Message *messages;

        int size;
    };
} // violet

#endif //MESSAGE_H
