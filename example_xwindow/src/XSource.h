#pragma once

#include "ofMain.h"
#include "FboSource.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "ofGstUtils.h"

class XSource : public ofx::piMapper::FboSource {
	public:
		XSource(Display* display, Window win, const std::string& windowName);
    	void setup();
		void update();
		void draw();

	private:
	    Display* display = nullptr;
    	Window targetWindow = 0;
    	int windowWidth_ = 0;
    	int windowHeight_ = 0;
    	ofFbo fbo_;
		ofGstVideoUtils videoUtils;
		
};