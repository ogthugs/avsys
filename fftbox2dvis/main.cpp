#include "ofMain.h"
#include "testApp.h"

static int pts[] = {257,219,257,258,259,274,263,325,266,345,266,352,269,369,276,387,286,415,291,425,302,451,308,462,316,472,321,480,328,488,333,495,339,501,345,505,350,507,365,515,370,519,377,522,382,525,388,527,405,534,426,538,439,539,452,539,468,540,485,540,496,541,607,541,618,539,625,537,641,530,666,513,682,500,710,476,723,463,727,457,729,453,732,450,734,447,738,440,746,423,756,404,772,363,779,343,781,339,784,327,789,301,792,278,794,267,794,257,795,250,795,232,796,222,796,197,797,195,797,188,796,188};
static int nPts  = 61*2;

//========================================================================
int main( ){

	ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context
	
	// this is my "app" :
	testApp APP;
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	ofRunApp(&APP);
	
	
}