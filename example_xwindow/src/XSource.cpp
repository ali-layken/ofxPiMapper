#include "XSource.h"

Display* XSource::sharedDisplay = nullptr;

bool XSource::initDisplay() {
    if (!sharedDisplay) {
        sharedDisplay = XOpenDisplay(nullptr);
    }
    return sharedDisplay != nullptr;
}

void XSource::shutdownDisplay() {
    if (sharedDisplay) {
        XCloseDisplay(sharedDisplay);
        sharedDisplay = nullptr;
    }
}

Display* XSource::getDisplay() {
    return sharedDisplay;
}

XSource::XSource(Window win, const std::string& windowName) {
    targetWindow = win;
    name = windowName;
}

void XSource::setup(){
    XWindowAttributes windowAttributes;
    XGetWindowAttributes(XSource::getDisplay(), targetWindow, &windowAttributes);
    allocate(windowAttributes.width, windowAttributes.height); 

    std::string pipeline = "ximagesrc xid=" + std::to_string(targetWindow) + " use-damage=false ! "
                           "video/x-raw,format=BGRx,framerate=60/1 ! queue";

    ofLogNotice() << "Initializing GStreamer pipeline: " << pipeline;

    if (!videoUtils.setPipeline(pipeline, OF_PIXELS_BGRX, true, windowAttributes.width, windowAttributes.height)) {
        ofLogError() << "GStreamer pipeline failed to initialize!";
        return;
    }
    
    videoUtils.startPipeline();
}

// Don't do any drawing here
void XSource::update(){
    videoUtils.update();
    if (videoUtils.isFrameNew()) {
        videoPixels = videoUtils.getPixels();
        
        if (videoPixels.isAllocated()) {
            // Allocate texture once
            if (!videoTexture.isAllocated()) {
                videoTexture.allocate(videoPixels.getWidth(), videoPixels.getHeight(), GL_RGBA);
            }
            // Upload new frame to texture
            videoTexture.loadData(videoPixels);
        }
    }
 
}


void XSource::draw(){
    ofClear(0);

    if (videoTexture.isAllocated()) {
        videoTexture.draw(0, 0, fbo->getWidth(), fbo->getHeight());
    } else {
        ofLogError() << "Video texture is not allocated!";
    }

}
