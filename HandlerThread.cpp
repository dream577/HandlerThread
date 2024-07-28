//
// Created by bronyna on 2024/7/28.
//

#include "HandlerThread.h"

#include "vtime.h"

namespace violet {
    Handler::Handler(Looper *looper) {
        this->mQueue = looper->mQueue;
        this->handleMessage = nullptr;
        this->next = nullptr;
    }

    Handler::~Handler() {
        this->mQueue.reset();
        this->handleMessage = nullptr;
        this->next = nullptr;
    }

    void Handler::setHandleMessageCallback(void (*handle)(Message *msg)) {
        this->handleMessage = handle;
    }

    void Handler::sendMessage(int what) {
        sendMessageDelayed(what, 0);
    }

    void Handler::sendMessageDelayed(int what, long delayTimeInMillis) {
        Message *message = Message::obtain();
        message->what = what;
        sendMessageDelayed(message, delayTimeInMillis);
    }

    void Handler::sendMessage(Message *message) {
        sendMessageDelayed(message, 0);
    }

    void Handler::sendMessageDelayed(Message *message, long delayTimeInMillis) {
        std::shared_ptr<MessageQueue> queue = mQueue;
        if (queue == nullptr) {
            Message::recycle(message);
            return;
        }
        message->when = CURRENT_MONOTONIC_TIME + delayTimeInMillis;
        message->target = this;
        message->dispatchMessage = dispatchMessage;
        queue->enqueueMessage(message);
    }

    void Handler::post(void (*run)()) {
        Message *message = Message::obtain();
        message->run = run;
        sendMessageDelayed(message, 0);
    }

    void Handler::postDelayed(void (*run)(), long delayTimeMillis) {
        Message *message = Message::obtain();
        message->run = run;
        sendMessageDelayed(message, delayTimeMillis);
    }

    void Handler::removeMessage(int what) {
        std::shared_ptr<MessageQueue> queue = mQueue;
        if (queue == nullptr) return;
        queue->removeMessage(what);
    }

    void Handler::removeAllMessages() {
        std::shared_ptr<MessageQueue> queue = mQueue;
        if (queue == nullptr) return;
        queue->removeAllMessages();
    }

    void Handler::dispatchMessage(void *handler, Message *message) {
        if (message->run != nullptr) {
            message->run();
        } else if (handler != nullptr) {
            auto *_handler = static_cast<Handler *>(handler);
            if (_handler->handleMessage != nullptr) {
                _handler->handleMessage(message);
            }
        }
    }

    Handler *HandlerThread::obtainHandler() {
        if (!runring || looper == nullptr) return nullptr;
        auto *handler = new Handler(looper);
        if (mHandlers == nullptr) {
            mHandlers = handler;
        } else {
            handler->next = mHandlers;
        }
        return handler;
    }

    HandlerThread::HandlerThread() {
        this->looper = new Looper();
        this->runring = false;
        this->mHandlers = nullptr;
    }


    int HandlerThread::start() {
        return pthread_create(&thread, nullptr, &run, this);
    }

    void HandlerThread::quitSafely() {
        looper->quitSafely();
        while (runring) {
            usleep(1000);
        }
    }

    void *HandlerThread::run(void *pHandlerThread) {
        HandlerThread *thread = static_cast<HandlerThread *>(pHandlerThread);
        thread->runring = true;
        thread->looper->loop();
        thread->runring = false;
        return nullptr;
    }

    HandlerThread::~HandlerThread() {
        while (runring) {
            usleep(1000);
        }
        Handler *handler;
        while (mHandlers != nullptr) {
            handler = mHandlers;
            mHandlers = mHandlers->next;
            delete handler;
        }
        delete looper;
    }
} // violet
