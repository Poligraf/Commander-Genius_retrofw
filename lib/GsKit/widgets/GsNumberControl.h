/*
 * CGUINumberControl.h
 *
 *  Created on: 06.03.2012
 *      Author: gerstrong
 */

#ifndef CGUINUMBERCONTROL_H_
#define CGUINUMBERCONTROL_H_

#include "GsControl.h"

#include <string>
#include <graphics/GsSurface.h>

class CGUINumberControl : public CGUIControl
{
public:

	CGUINumberControl(	const std::string& text,
			const int startValue,
			const int endValue,
			const int deltaValue,
			const int value,
            const int fontid,
            const bool slider);


	void increment();
	void decrement();

	bool sendEvent(const InputCommands command);

	const int getSelection();
	void setSelection( const int value );

    void processLogic();

    std::string sliderStr();

	void processRender(const GsRect<float> &RectDispCoordFloat);

protected:
    bool mIncSel;
    bool mDecSel;

    GsSurface mTextDarkSfc;
    GsSurface mTextLightSfc;
    GsSurface mTextLightSfcR;
    GsSurface mTextLightSfcL;
    GsSurface mTextDisabledSfc;

    std::string	mText;
    int mValue;
    const bool mSlider;

    int mLightRatio; // This will blend between selected and unselected item.

    const int mStartValue;
    const int mEndValue;
    bool mMustRedraw;

private:

	const int mDeltaValue;

	static int mTwirliconID;    
};

#endif /* CGUINUMBERCONTROL_H_ */
