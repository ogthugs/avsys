#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "beatDetect.h"
#include "ofxBox2d.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

    ofTrueTypeFont  BlackoutMidnight;
	void audioReceived(float* input, int bufferSize, int nChannels);

   	beatDetect bd;
	float volume;
	float * inputBufferCopy;
	
private:
	vector <ofVec2f> loadPoints(string file);

	
	ofxBox2d box2d;			 

	ofPolyline shape;
	vector <ofxBox2dRect> boxes;
	vector <ofxBox2dCircle> circles;
	vector <ofxBox2dPolygon> triangles;
	ofxBox2dCircle paddle;
	
	vector <ofxBox2dCircle> bullets;
	
	int paddlex;

	long int timerlow;
	long int timermid;
	long int timerhigh;
	
	ofImage pizza;
	ofSoundPlayer beats;
	float beatSpeed;
	bool killit;


};

#endif
