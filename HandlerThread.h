//
// Created by bronyna on 2024/7/28.
//

#ifndef HANDLERTHREAD_H
#define HANDLERTHREAD_H

#include <unistd.h>
#include "Looper.h"

namespace violet {
    class Handler {
        friend class HandlerThread;

    public:
        void setHandleMessageCallback(void (*handle)(Message *msg));

        void sendMessage(int what);

        void sendMessageDelayed(int what, long delayTimeInMillis);

        void sendMessage(Message *message);

        void sendMessageDelayed(Message *message, long delayTimeInMillis);

        void post(void (*run)());

        void postDelayed(void (*run)(), long delayTimeMillis);

        void removeMessage(int what);

        void removeAllMessages();

        ~Handler();

    protected:
        Handler *next;

        Handler(Looper *looper);

    private:
        std::shared_ptr<MessageQueue> mQueue;

        void (*handleMessage)(Message *msg);

        static void dispatchMessage(void *handler, Message *message);
    };

    class HandlerThread {
    public:
        HandlerThread();

        Handler *obtainHandler();

        int start();

        void quitSafely();

        ~HandlerThread();

    private:
        bool runring;

        Handler *mHandlers;

        Looper *looper;

        pthread_t thread;

        static void *run(void *pHandlerThread);
    };
} // violet

#endif //HANDLERTHREAD_H
