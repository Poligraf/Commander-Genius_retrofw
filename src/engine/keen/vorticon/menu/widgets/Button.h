#ifndef __VORTICONBUTTON_H__
#define __VORTICONBUTTON_H__

#include <widgets/GsButton.h>

namespace vorticon
{

class Button : public GsButton
{
public:
    Button(const std::string& text,
                 CEvent *ev);

    void processRender(const GsRect<float> &RectDispCoordFloat);

private:

    void setupButtonSurface();

    void drawVorticonStyle(SDL_Rect& lRect);

};

}

#endif /* __VORTICONBUTTON_H__ */
