#ifndef __PROJECTSCENE_H_
#define __PROJECTSCENE_H_

#include "CGFscene.h"
#include "CGFappearance.h"
#include "XMLSceneMod.h"
#include "Lights.h"
#include <vector>


class ProjectScene : public CGFscene
{
public:
	void init();
	void display();

	CGFlight* lightTest;
	Light* lights[8];
	unsigned int numberOfLights;

	Graph sceneGraph;

	CGFappearance* materialA;
	CGFappearance* materialB;
	
	~ProjectScene();
	


	///////////alternar entre wireframe e texturas////////////
	int visualizationMode;
	void setVisualizationMode(int mode){
		visualizationMode = mode;
		printf("mode = %d", mode);
	}

	
	void setWireFrameMode()
	{
		glPolygonMode(GL_FRONT,GL_LINE);
		glPolygonMode(GL_BACK,GL_LINE);

	}

	void setTextureMode()
	{
		glPolygonMode(GL_FRONT,GL_FILL);
		glPolygonMode(GL_BACK,GL_FILL);

	}
	///////////////////////////////////////////////////////////


	void drawAux(Node* node);
	};




#endif