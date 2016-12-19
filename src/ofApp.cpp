#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0);
    
    cam.setupPerspective();
    
    /*
     Shader
     */
    updatePosition.load("", "shader/updatePosition.frag");
    render.load("shader/render.vert", "shader/render.frag");
    
    /*
     Initialize mesh
     */
    int vertexNum = 100000;
    texRes = ceil(sqrt(vertexNum));
    
    for (int i = 0; i < texRes; i++) {
        for (int j = 0; j < texRes; j++) {
            
            int index = i * texRes + j;
            
            mesh.addVertex(ofVec3f(0, 0, 0));
            mesh.addTexCoord(ofVec2f(i, j));
            mesh.addColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
        }
    }
    
    /*
     Add Indexes to mesh of Particle
     */
    vector<int> lastRow;
    for (int x = 0; x < texRes; x++) {
        for (int y = 0; y < texRes-1; y++) {
            if (x == texRes - 1) {
                int idx1 = x*texRes+y;
                int idx2 = x*texRes+y+1;
                int idx3 = y+1;
                int idx4 = y;
                mesh.addTriangle(idx1, idx2, idx3);
                mesh.addTriangle(idx1, idx3, idx4);
            } else {
                int idx1 = x*texRes+y;
                int idx2 = x*texRes+y+1;
                int idx3 = (x+1)*texRes+y+1;
                int idx4 = (x+1)*texRes+y;
                mesh.addTriangle(idx1, idx2, idx3);
                mesh.addTriangle(idx1, idx3, idx4);
                if (y == texRes-2) {
                    lastRow.push_back(idx2);
                }
            }
        }
    }
    int lastVertex = mesh.getNumVertices() - 1;
    for (int i = 0; i<lastRow.size()-1; i++) {
        mesh.addTriangle(lastRow[i], lastRow[i+1], lastVertex);
        mesh.addTriangle(0, lastRow[i]+1, lastRow[i+1]+1);
    }
    mesh.addTriangle(lastRow[lastRow.size()-1], lastRow[0], lastVertex);
    
    /*
     setup PingPong
     */
    pingPong.allocate(texRes, texRes, GL_RGBA32F, 1);
    
    float * positions = new float[texRes * texRes * 4];
    for (int x = 0; x < texRes; x++) {
        for (int y = 0; y < texRes; y++) {
            int index = y * texRes + x;
            positions[index * 4 + 0] = 0;
            positions[index * 4 + 1] = 0;
            positions[index * 4 + 2] = 0;
            positions[index * 4 + 3] = 0;
        }
    }
    pingPong.src->getTexture(0).loadData(positions, texRes, texRes, GL_RGBA);
    delete [] positions;
    
    stepX = 2.0*PI / texRes;
    stepY = PI / texRes;
    
    /*
     GUI
     */
    panel.setup();
    panel.add(mode.setup("Mode", 0, 0, 2));
    panel.add(colorMode.setup("Color", 0, 0, 1));
    panel.add(smoothing.setup("smoothing", 5, 1, 15));
    panel.add(a.setup("a", 1, 0, 5));
    panel.add(b.setup("b", 1, 0, 5));
    panel.add(m.setup("m", 1, 0, 40));
    panel.add(n1.setup("n1", 1, 0, 5));
    panel.add(n2.setup("n2", 1, 0, 5));
    panel.add(n3.setup("n3", 1, 0, 5));
}

//--------------------------------------------------------------
void ofApp::update(){
    
    pingPong.dst->begin();
    updatePosition.begin();
    updatePosition.setUniformTexture("u_positionsTex", pingPong.src->getTexture(0), 0);
    updatePosition.setUniform1f("stepX", stepX);
    updatePosition.setUniform1f("stepY", stepY);
    updatePosition.setUniform1f("smoothing", smoothing);
    updatePosition.setUniform1f("a", a);
    updatePosition.setUniform1f("b", b);
    updatePosition.setUniform1f("m", m);
    updatePosition.setUniform1f("n1", n1);
    updatePosition.setUniform1f("n2", n2);
    updatePosition.setUniform1f("n3", n3);
    pingPong.src->draw(0, 0);
    updatePosition.end();
    pingPong.dst->end();
    
    pingPong.swap();
    
    switch (mode) {
        case 0:
            mesh.setMode(OF_PRIMITIVE_POINTS);
            break;
        default:
            mesh.setMode(OF_PRIMITIVE_TRIANGLES);
            break;
    }
    switch (colorMode) {
        case 0:
            for (int x = 0; x < texRes; x++) {
                for (int y = 0; y < texRes; y++) {
                    int index = y*texRes+x;
                    mesh.setColor(index, ofColor(ofMap(abs(y-texRes/2), 0, texRes/2, 0, 255), ofMap(abs(x-texRes/2), 0, texRes/2, 0, 255), 120));
                }
            }
            break;
        case 1:
            for (int x = 0; x < texRes; x++) {
                for (int y = 0; y < texRes; y++) {
                    int index = y*texRes+x;
                    mesh.setColor(index, ofFloatColor(ofMap(y%3, 0, 2, 0.0, 1.0)));
                }
            }
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    ofPushStyle();
    cam.begin();
    
    switch (colorMode) {
        case 0:
            glShadeModel(GL_SMOOTH);
            break;
        case 1:
            glShadeModel(GL_FLAT);
            break;
        default:
            break;
    }
    ofEnableDepthTest();
    
    render.begin();
    render.setUniformTexture("u_positionsTex", pingPong.src->getTexture(0), 0);
    switch (mode) {
        case 0:
            mesh.draw();
            break;
        case 1:
            mesh.drawFaces();
            break;
        default:
            mesh.drawWireframe();
            break;
    }
    render.end();
    ofDisableDepthTest();
    
    cam.end();
    ofPopStyle();
    
    
    panel.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
