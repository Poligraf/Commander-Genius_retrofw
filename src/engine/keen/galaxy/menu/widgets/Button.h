#ifndef __GALAXYBUTTON_H__
#define __GALAXYBUTTON_H__

#include <widgets/GsButton.h>

class GalaxyButton : public GsButton
{
public:
    GalaxyButton(const std::string& text,
                 CEvent *ev);

    void drawEnabledButton(GsWeakSurface &blitsfc,
                           const SDL_Rect &lRect,
                           const bool alternate);

    virtual void processRender(const GsRect<float> &RectDispCoordFloat);

private:

    void setupButtonSurface();
};

#endif /* __GALAXYBUTTON_H__ */
