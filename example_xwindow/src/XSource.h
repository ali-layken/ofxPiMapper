#pragma once

#include "ofMain.h"
#include "FboSource.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "ofGstUtils.h"

class XSource : public ofx::piMapper::FboSource {
	public:
		static bool initDisplay();        
    	static void shutdownDisplay();     
    	static Display* getDisplay(); 

		XSource(Window win, const std::string& windowName);
		~XSource();

    	void setup();
		void update();
		void draw();

		std::vector<ofRectangle> rects;
		std::vector<float> rectSpeeds;

	private:
		static Display* sharedDisplay;

		Window targetWindow;
		ofGstVideoUtils videoUtils;
		ofTexture videoTexture;
		ofPixels videoPixels;
		
};