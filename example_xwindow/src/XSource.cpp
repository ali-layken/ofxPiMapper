#include "XSource.h"


XSource::XSource(Display* d, Window win, const std::string& windowName) {
    display = d;
    targetWindow = win;
    name = windowName;

    XWindowAttributes gwa;
    XGetWindowAttributes(display, targetWindow, &gwa);
    windowWidth_ = gwa.width;
    windowHeight_ = gwa.height;

    // Allocate FBO once
    fbo_.allocate(windowWidth_, windowHeight_, GL_RGBA);
}

void XSource::setup(){
    XWindowAttributes windowAttributes;
    XGetWindowAttributes(display, targetWindow, &windowAttributes);
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
    XImage* image = XGetImage(display, targetWindow, 0, 0, windowWidth_, windowHeight_, AllPlanes, ZPixmap);
    ofPixels pixels;

    pixels.allocate(windowWidth_, windowHeight_, OF_PIXELS_RGB);
    memcpy(pixels.getData(), image->data, windowWidth_ * windowHeight_ * 4); // Assuming 32bpp

    fbo_.begin();
    ofClear(0, 0, 0, 0);
    ofTexture tex;
    tex.loadData(pixels);
    tex.draw(0, 0);
    fbo_.end();

    XDestroyImage(image);
 
}


void XSource::draw() {
    fbo_.draw(0,0);
}
