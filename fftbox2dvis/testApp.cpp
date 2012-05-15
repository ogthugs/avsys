#include "testApp.h"

static int pts[] = {257,219,257,258,259,274,263,325,266,345,266,352,269,369,276,387,286,415,291,425,302,451,308,462,316,472,321,480,328,488,333,495,339,501,345,505,350,507,365,515,370,519,377,522,382,525,388,527,405,534,426,538,439,539,452,539,468,540,485,540,496,541,607,541,618,539,625,537,641,530,666,513,682,500,710,476,723,463,727,457,729,453,732,450,734,447,738,440,746,423,756,404,772,363,779,343,781,339,784,327,789,301,792,278,794,267,794,257,795,250,795,232,796,222,796,197,797,195,797,188,796,188};
static int nPts  = 61*2;
		
//--------------------------------------------------------------
void testApp::setup(){	 
	
	// 0 output channels, 
	// 2 input channels
	// 44100 samples per second
	// BUFFER_SIZE samples per buffer
	// 4 num buffers (latency)
	
	ofSoundStreamSetup(0,2,this, 44100, BUFFER_SIZE, 4);	
	
	left = new float[BUFFER_SIZE];
	right = new float[BUFFER_SIZE];	
	
	FFTanalyzer.setup(44100, BUFFER_SIZE/2, 2);
	
	FFTanalyzer.peakHoldTime = 15; // hold longer
	FFTanalyzer.peakDecayRate = 0.95f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies
	
	ofSetFrameRate(30);
	ofEnableSmoothing();
	
	//box2d physics objects
	box2d.init();
	box2d.setGravity(0.0,8.0);
	//box2d.createGround();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	counter = 0;
	
	firstLoop = true;

}


//--------------------------------------------------------------
void testApp::update(){
	ofBackground(0);
	box2d.update();
	
	
	//~~~~~~~~~~
	float avg_power = 0.0f;
	myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left, BUFFER_SIZE, &magnitude[0],&phase[0], &power[0],&avg_power);
	for (int i = 0; i < (int)(BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
	}
	FFTanalyzer.calculate(freq);
	//~~~~~~~~~~~
	
	if(firstLoop){ //ONLY called ONCE!
		float total_fft = 0.0;
		float average_fft;
		for(int i=0; i<FFTanalyzer.nAverages; i++) {
			
			ofxBox2dCircle circle;
			circle.setPhysics(300.0, 0.10, 0.1);
			circle.setup(box2d.getWorld(), 100+i*50, ofGetHeight()/2, 10.0);
			circles.push_back(circle);	
			
			total_fft += FFTanalyzer.peaks[i];
			if(i==FFTanalyzer.nAverages-1){
				average_fft = 5.0*(total_fft/float(i));
			}
		}
		firstLoop = false;	
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(255);

	/*
	for (int i = 0; i < FFTanalyzer.nAverages; i++){
		ofRect(200+(i*40),600,20,-FFTanalyzer.averages[i] * 6);
		}
	
	
	//we loop through the array of rectangles and draw them, simple as that
	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetColor(255);
		circles[i].draw();
	}
	*/
	
	for(int i=0; i<FFTanalyzer.nAverages; i++){
		
		//ofVec2f amt_huh = circles[i].getPosition();
		//forceFftVec.set(0, -0.5*FFTanalyzer.peaks[i]);
		//ofVec2f huh(0,0);
		//circles[i].setFixedRotation(true);
		//circles[i].addImpulseForce(forceFftVec,amt_huh);
		//circles[i].setDamping(1.1);
		//circles[i].addForce(forceFftVec,1.0);
		ofVec2f current_pos = circles[i].getPosition();
		circles[i].setPosition(current_pos.x,ofGetHeight()-FFTanalyzer.peaks[i]*5);
		ofFill();
		ofSetColor(255);
		circles[i].draw();
	}
	
	
	//draw loop for physics boxes
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xBF2545);
		boxes[i].draw();
	}
	
	// draw the ground
//	box2d.drawGround();
	
	
	

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if(key == 'c') {
		float r = ofRandom(4, 20);		// a random radius 4px - 20px
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(box2d.getWorld(), mouseX, mouseY, r);
		circles.push_back(circle);
	}
	
	if(key == 'b') {
		float w = ofRandom(4, 20);	
		float h = ofRandom(4, 20);	
		ofxBox2dRect rect;
		rect.setPhysics(0.30, 0.53, 0.1);
		rect.setup(box2d.getWorld(), mouseX, mouseY, w, h);
		boxes.push_back(rect);
	}
	
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	
	

}

//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
	}
}

