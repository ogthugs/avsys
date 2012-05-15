#include "testApp.h"
#include "beatDetect.h"

int testApp_buffer_size = 1024;
int testApp_fft_size = 512;


bool detect=true;
bool drumVisible = true;
bool snareVisible = true;
bool hihatVisible = true;

static int pts[] = {257,219,257,258,259,274,263,325,266,345,266,352,269,369,276,387,286,415,291,425,302,451,308,462,316,472,321,480,328,488,333,495,339,501,345,505,350,507,365,515,370,519,377,522,382,525,388,527,405,534,426,538,439,539,452,539,468,540,485,540,496,541,607,541,618,539,625,537,641,530,666,513,682,500,710,476,723,463,727,457,729,453,732,450,734,447,738,440,746,423,756,404,772,363,779,343,781,339,784,327,789,301,792,278,794,267,794,257,795,250,795,232,796,222,796,197,797,195,797,188,796,188};
static int nPts  = 61*2;

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		SETUP
//////////////////////////////////////////////////////////////////////////
void testApp::setup(){

	ofSetFrameRate(60);
	
    // Output->0 Input->mono  sampleRate->44100  bufSize->1024
	ofSoundStreamSetup(0, 1, this, 44100, testApp_buffer_size, 4);
    inputBufferCopy = new float [512 *2];
	
    BlackoutMidnight.loadFont("BlackoutMidnight.ttf", 72, true, true);
	ofBackground(20,20,20);
    printf("testApp setup() OK!\n");

//box2d physics objects

ofEnableSmoothing();
ofEnableAlphaBlending();

box2d.init();
box2d.setGravity(0.0,5.0);
//box2d.createGround();


box2d.setFPS(30.0);
box2d.registerGrabbing();
	


//load the save shape (triangles)
vector <ofVec2f> pts = loadPoints("shape.dat");
ofxBox2dPolygon poly;

	// loop and add vertex points
	for (int i=0; i<pts.size(); i++) {
		poly.addVertex(pts[i]);
	}
	poly.setAsEdge(false);
	poly.setPhysics(1.0, 0.3, 0.3);
	poly.create(box2d.getWorld());
	triangles.push_back(poly);
	
paddle.setPhysics(1.2, 0.50, 0.1);
paddle.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()-20,10);
	//pizza.loadImage("pizza.png");
	
beats.loadSound("rock.wav");
beats.setVolume(0.75f);
beats.setLoop(TRUE);
	
	beats.play();
	killit = false;


	
	
}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		VECTOR 
//////////////////////////////////////////////////////////////////////////
vector <ofVec2f> testApp::loadPoints(string file) {
	ifstream f;
	vector <ofVec2f> pts;
	f.exceptions ( ifstream::failbit | ifstream::badbit );
	try {
		f.open(ofToDataPath(file).c_str());
		string strLines;
		while (!f.eof()) {
			string ptStr;
			getline(f, ptStr);
			strLines+=ptStr;
		}
		f.close();
		
		vector <string>  ptsStr = ofSplitString(strLines, ",");
		for (int i=0; i<ptsStr.size(); i+=2) {
			float x = ofToFloat(ptsStr[i]);
			float y = ofToFloat(ptsStr[i+1]);
			pts.push_back(ofVec2f(x, y));
		}
	}
	catch (ifstream::failure e) {
		printf("no file to load...\n");
	}
	return pts;
}


//////////////////////////////////////////////////////////////////////////
//---------------------------------------		UPDATE
//////////////////////////////////////////////////////////////////////////
void testApp::update() {

	ofSoundUpdate();
   
    bd.updateFFT();
	
	box2d.update();
	
	beatSpeed += 0.0003;
	beats.setSpeed(beatSpeed);

	
	paddle.setPosition(paddlex, ofGetHeight()-20);
	//cout << "fuck" << endl;

	for (int i=0; i<circles.size(); i++){
		ofVec2f circlepos = circles[i].getPosition();
		//cout << circlepos.x << circlepos.y << endl;
		//pizza.draw(circlepos.x,circlepos.y,20);
		if(circlepos.x!=0 and circlepos.y!=0){
			if (circlepos.x>ofGetWidth() or circlepos.x < 0 or circlepos.y>ofGetHeight() or circlepos.y<0){
				//circles[i].destroy();
			//	physics.world->DestroyBody(circles[i].body);
				box2d.getWorld()->DestroyBody(circles[i].body);
				circles.erase(circles.begin() + i);
				//cout << "destroyed" << endl;
			}
		}
	}
	for (int i=0; i<bullets.size(); i++){
		ofVec2f bulletpos = bullets[i].getPosition();
		//cout << circlepos.x << circlepos.y << endl;
		//pizza.draw(circlepos.x,circlepos.y,20);
		if(bulletpos.x!=0 and bulletpos.y!=0){
			if (bulletpos.x>ofGetWidth() or bulletpos.x < 0 or bulletpos.y>ofGetHeight() or bulletpos.y<0){
				//circles[i].destroy();
				//	physics.world->DestroyBody(circles[i].body);
				box2d.getWorld()->DestroyBody(bullets[i].body);
				bullets.erase(bullets.begin() + i);
				//cout << "destroyed" << endl;
			}
		}
	}
	
	
	//CHECK IF GAME IS OVER
	cout << "cs: " << circles.size() << endl;
	for(int i=0; i<circles.size(); i++){
		ofVec2f circlepos = circles[i].getPosition();
		ofVec2f paddlepos = paddle.getPosition();
		int crad = circles[i].getRadius();
		//int dist = pow( ( pow(paddlepos.x-circlepos.x,2.0) + pow(paddlepos.x-circlepos.x,2.0) ),0.5 );
		
		float dist = ofDist(circlepos.x, circlepos.y, paddlepos.x, paddlepos.y);
		

		   if(dist<=(10+crad)){
			   killit = true;
			  // ofSleepMillis(2000);   
		   }
	}
}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		DRAW
//////////////////////////////////////////////////////////////////////////
void testApp::draw()
{

	if (killit == true) {
		ofBackground(255,0,0);
		beats.stop();
	}
	else {
	

	ofSetColor(250,0,0);
	ofFill();
	paddle.draw();


	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetColor(255);
		boxes[i].draw();
	}
	
	for (int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetColor(255);
		circles[i].draw();
	}
	
	for (int i=0; i<bullets.size(); i++) {
		ofFill();
		ofSetColor(0,0,255);
		bullets[i].draw();
	}

	ofSetColor(255);
	ofFill();
	shape.draw();
	
	

 
   if(drumVisible){
        if(bd.isBeatRange(0,6,5)){
            ofSetColor(255,0,0);
			
            if(ofGetElapsedTimeMillis()-timerlow > 200){
				//float w = ofRandom(4, 20);	
				//float h = ofRandom(4, 20);	
				float r = ofRandom(4, 50);
				ofxBox2dCircle circle;
				circle.setPhysics(0.2, 0.50, 0.1);
				circle.setup(box2d.getWorld(), ofRandom(20,ofGetWidth()-20), 10, 40);
				circles.push_back(circle);
				timerlow = ofGetElapsedTimeMillis();
		
			}
		}
        
		else{
            ofSetColor(255);
            BlackoutMidnight.drawString("ROCK",10,70);
        }
    }

    if(snareVisible){
        if(bd.isBeatRange(12,18,4)){
			if(ofGetElapsedTimeMillis()-timermid > 200){
			
				ofSetColor(255);
				float r = ofRandom(4, 50);
				ofxBox2dCircle circle;
				circle.setPhysics(0.2, 0.50, 0.1);
				circle.setup(box2d.getWorld(), ofRandom(20,ofGetWidth()-20), 10, 25);
				circles.push_back(circle);
				for(int i=0;i<=10;i++){
				}
				timermid = ofGetElapsedTimeMillis();
				
			}
           
		}
        else{
            ofSetColor(255);
			BlackoutMidnight.drawString("ROCK", 200, 70);
        }
    }

    if(hihatVisible){
        if(bd.isBeatRange(27,31,3)){
            ofSetColor(255,0,0);
			if(ofGetElapsedTimeMillis()-timerhigh > 200){
            
			float r = ofRandom(4, 50);		// a random radius 4px - 20px
			ofxBox2dCircle circle;
			circle.setPhysics(0.2, 0.53, 0.1);
			circle.setup(box2d.getWorld(), ofRandom(20,ofGetWidth()-20), 10, 10);
			circles.push_back(circle);
			timerhigh = ofGetElapsedTimeMillis();
			
			}
		}	
			
		else{
            ofSetColor(255);
			BlackoutMidnight.drawString("MARKET", 700, 70);        }
		}
	}

}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		AUDIO RECEIVED
//////////////////////////////////////////////////////////////////////////
void testApp::audioReceived(float* input, int bufferSize, int nChannels) {
	
	bd.audioReceived(input, bufferSize);
	
	//Copying new value from memory InputBufferCopy ---- see definition by right clicking on memcpy
    memcpy(inputBufferCopy, input, bufferSize*nChannels* sizeof(float));
    volume      = 0;
    
    for(int i = 0; i < bufferSize * nChannels; i++){
        volume += input[i] * input[i];  // add the square value to eliminate any negative values
    }
    volume      /= bufferSize * nChannels;   // get the average of all square values
    volume      = sqrt(volume);              // finally take the square root of the value
	
}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		KEY PRESSED
//////////////////////////////////////////////////////////////////////////
void testApp::keyPressed(int key){
	
	
    if(key == 'd'){
        drumVisible = !drumVisible;
    }
    if(key == 's'){
        snareVisible = !snareVisible;
    }
    if(key == 'h'){
		hihatVisible = !hihatVisible;
    }
	if(key == 'a'){
		
		ofVec2f paddlepos = paddle.getPosition();
		ofxBox2dCircle bullet;
		bullet.setPhysics(5.0, 0.20, 0.5);
		bullet.setup(box2d.getWorld(), paddlepos.x, paddlepos.y-10, 3);
		bullets.push_back(bullet);
		
		ofVec2f fvec;
		fvec.set(0, 1.0);
		bullet.addForce(fvec,1500.0);
		
    }
//    if(key == 'n'){
//		if (paddlex <= paddlepos.x/2.0){
//		//do nothing	
//		}
//		else{
//		paddlex-=10;	
//		}
//	}
//	if(key == 'm'){
//		if (paddlex >= ofGetWidth()-15){
//			//do nothing	
//		}
//		else{
//			paddlex+=10;	
//		}	
//	}
	
}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		KEY RELEASED
//////////////////////////////////////////////////////////////////////////
void testApp::keyReleased(int key){

}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		MOUSE MOVED
//////////////////////////////////////////////////////////////////////////
void testApp::mouseMoved(int x, int y){
	ofVec2f paddlepos = paddle.getPosition();

	
	if(x <= 15 or x >= ofGetWidth()-15){
		
	}
	else {
		paddlex = x;
	}

}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		MOUSE DRAGGED
//////////////////////////////////////////////////////////////////////////
void testApp::mouseDragged(int x, int y, int button){

}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		MOUSE PRESSED
//////////////////////////////////////////////////////////////////////////
void testApp::mousePressed(int x, int y, int button){

}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		MOUSE RELEASED
//////////////////////////////////////////////////////////////////////////
void testApp::mouseReleased(int x, int y, int button){

}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------		WINDOW RESIZED
//////////////////////////////////////////////////////////////////////////
void testApp::windowResized(int w, int h){

}
