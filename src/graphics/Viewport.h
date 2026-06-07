#ifndef VIEWPORT_H
#define VIEWPORT_H

namespace xen
{
    struct Viewport
    {
    public:
        Viewport(int width, int height, int mode);
        ~Viewport(void);

        int width;
        int height;
        int mode;
    };
}

#endif //VIEWPORT_H
