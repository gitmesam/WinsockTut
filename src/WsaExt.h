// Copyright (C) 2012-2018 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License. 
// See accompanying files LICENSE.

#pragma once

#include <WinSock2.h>
#include <Mswsock.h>

class WsaExt
{
public:
    WsaExt();

    static void Init(SOCKET fd);

    static LPFN_CONNECTEX              ConnectEx;
    static LPFN_ACCEPTEX               AcceptEx;
    static LPFN_ACCEPTEX               DisconnectEx;
    static LPFN_GETACCEPTEXSOCKADDRS   GetAcceptExSockaddrs;

private:
    static bool intialized_;
};
