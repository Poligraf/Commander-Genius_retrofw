#ifndef VIDEOSETTINGS_H
#define VIDEOSETTINGS_H

#include "FusionMenu.h"

#include <widgets/GsNumberControl.h>
#include <widgets/GsSwitch.h>
#include <widgets/GsComboSelection.h>
#include <widgets/GsSwitch.h>

namespace dbfusion
{

class VideoSettings : public FusionMenu
{
public:
    VideoSettings();

    void refresh();

    void release();

    CVidConfig mUserVidConf;

private:


    CGUINumberControl	*mpFPSSelection;

#if defined(USE_OPENGL)
    CGUISwitch			*mpOpenGLSwitch;    
#endif

    CGUIComboSelection	*mpRenderScaleQualitySel;

    CGUIComboSelection	*mpFilterSelection;
    CGUIComboSelection	*mpAspectSelection;
    CGUISwitch		*mpVSyncSwitch;
    GsButton		*mpFullScreenSwitch;
    CGUIComboSelection	*mpResolutionSelection;

};

}

#endif // VIDEOSETTINGS_H
