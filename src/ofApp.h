#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "PingPong.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    PingPong pingPong;
    ofShader updatePosition, render;
    ofVboMesh mesh;
    int texRes;
    float stepX;
    float stepY;
    
    /// Camera
    ofEasyCam cam;
    
    /// GUI
    ofxPanel panel;
    ofxIntSlider mode, colorMode;
    ofxFloatSlider smoothing, a, b, m, n1, n2, n3;
};
