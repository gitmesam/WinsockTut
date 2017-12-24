// Copyright (C) 2017 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License. 
// See accompanying files LICENSE.

#include "EventLoop.h"
#include <assert.h>
#include "Common/Define.h"
#include "Select.h"

EventLoop::EventLoop(IOMode type)
{
    timeout_ = 50;
    createIOPoller(type);
    events_.rehash(1024);
    fired_.rehash(1024);
}

EventLoop::~EventLoop()
{
    delete poller_;
    poller_ = NULL;
}

void EventLoop::createIOPoller(IOMode type)
{
    switch(type)
    {
    case IOSelect:
        poller_ = new SelectPoller();
        break;
    default:
        assert(false && "invalid multiplexer type");
    }
}

void EventLoop::AddEvent(SOCKET fd, int mask, EventProc func)
{
    assert(poller_ != NULL);
    auto iter = events_.find(fd);
    if (iter == events_.end())
    {
        EventEntry entry;
        entry.fd = fd;
        entry.mask =  mask;
        events_[fd] = entry;
    }
    EventEntry* entry = &events_[fd];
    if ((mask & EV_READABLE) || (mask & EV_WRITABLE))
    {
        entry->proc = func;
    }
    poller_->AddFd(fd, mask);
}

void EventLoop::DelEvent(SOCKET fd, int mask)
{
    auto iter = events_.find(fd);
    if (iter != events_.end())
    {
        EventEntry* entry = &iter->second;
        entry->mask = entry->mask & (~mask);
        if (entry->mask == EV_NONE)
        {
            events_.erase(fd);
        }
    }
}

EventEntry* EventLoop::GetEntry(SOCKET fd)
{
    auto iter = events_.find(fd);
    if (iter != events_.end())
    {
        return &iter->second;
    }
    return NULL;
}

void EventLoop::AddFiredEvent(SOCKET fd, int mask)
{
    fired_[fd] = mask;
}

void EventLoop::Run()
{
    while (true)
    {
        RunOne();
    }
}

void EventLoop::RunOne()
{
    if (events_.empty())
    {
        return ;
    }

    fired_.clear();
    int count = poller_->Poll(this, 100);
    if (count == 0)
    {
        return;
    }
    for (auto it = fired_.begin(); it != fired_.end(); ++it)
    {
        SOCKET fd = it->first;
        auto iter = events_.find(fd);
        if (iter == events_.end())
            continue;
        EventEntry* entry = &iter->second;
        int mask = it->second;
        if ((mask & EV_READABLE) || (mask & EV_WRITABLE))
        {
            entry->proc(this, fd, mask);
        }
    }
}
