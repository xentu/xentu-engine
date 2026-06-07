//
// Created by conta on 28/06/2025.
//

#ifndef MOUSE_STATE_H
#define MOUSE_STATE_H

namespace xen
{
    class MouseState
    {
    public:
        MouseState();
        void Reset();
        int m_x;
        int m_y;
    };
}

#endif //MOUSE_STATE_H
