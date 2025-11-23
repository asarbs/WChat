/*
 * World VTT
 *
 * Copyright (C) 2025, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 *
 */

#ifndef MESSAGESRECEIVER_H
#define MESSAGESRECEIVER_H

#pragma once

class MessagesReceiver
{
    public:
        MessagesReceiver();
        virtual ~MessagesReceiver();
        MessagesReceiver(const MessagesReceiver& rhs);
        MessagesReceiver(const MessagesReceiver&& rhs);
        MessagesReceiver& operator=(const MessagesReceiver& rhs);
        MessagesReceiver& operator=(const MessagesReceiver&& rhs);
protected:

private:
};

#endif