#ifndef _XMLSCENEMOD_H_
#define _XMLSCENEMOD_H_

#include "tinyxml.h"

class XMLSceneMod
{
public:
	XMLSceneMod(char *filename);
	~XMLSceneMod();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
	
	bool readRGBcomponents (char* rawString, float &R, float &G, float &B, float &A);
protected:

	TiXmlDocument* doc;

	/////////////Projecto LAIG////////////
	TiXmlElement* globalsElement;
	TiXmlElement* camerasElement;

	//////////////////////////////////////

	///////////Porjecto Exemplo///////////
	TiXmlElement* initElement; 
	TiXmlElement* matsElement;
	TiXmlElement* textsElement;
	TiXmlElement* leavesElement;
	TiXmlElement* nodesElement;
	TiXmlElement* graphElement;
	//////////////////////////////////////
};

#endif