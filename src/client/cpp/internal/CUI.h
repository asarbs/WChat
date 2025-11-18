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

#ifndef CUI_H
#define CUI_H

#pragma once

class CUI {
    public:
        CUI();
        virtual ~CUI();
        CUI(const CUI& rhs);
        CUI(const CUI&& rhs);
        CUI& operator=(const CUI& rhs);
        CUI& operator=(const CUI&& rhs);

    protected:
    private:
};

#endif
