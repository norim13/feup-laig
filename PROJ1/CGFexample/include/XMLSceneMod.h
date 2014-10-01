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
	bool readXYcoord (char* rawString, float &x, float &y);
	bool readXYZcoord (char* rawString, float &x, float &y, float &z);
protected:

	TiXmlDocument* doc;

	/////////////Projecto LAIG////////////
	TiXmlElement* globalsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* graphElement;

	//////////////////////////////////////


};

#endif