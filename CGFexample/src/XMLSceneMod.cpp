#include "XMLSceneMod.h"

XMLSceneMod::XMLSceneMod(char *filename)
{

	// Read XML from file

	doc=new TiXmlDocument( filename );
	bool loadOkay = doc->LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load file '%s'. Error='%s'. Exiting.\n", filename, doc->ErrorDesc() );
		exit( 1 );
	}

	TiXmlElement* dgxElement= doc->FirstChildElement( "anf" );

	if (dgxElement == NULL)
	{
		printf("Main anf block element not found! Exiting!\n");
		exit(1);
	}

	//initElement = dgxElement->FirstChildElement( "Init" );
	matsElement = dgxElement->FirstChildElement( "Materials" );
	textsElement =  dgxElement->FirstChildElement( "Textures" );
	leavesElement =  dgxElement->FirstChildElement( "Leaves" );
	nodesElement =  dgxElement->FirstChildElement( "Nodes" );
	graphElement =  dgxElement->FirstChildElement( "Graph" );


	globalsElement = dgxElement->FirstChildElement( "globals" );
	camerasElement = dgxElement->FirstChildElement( "cameras" );

	/////////////globals/////////////////
	if (globalsElement == NULL)
		printf("globals block not found!\n");
	else
	{
		printf("Processing globals:\n");

		//////////drawing//////////////
		TiXmlElement* drawingElement = globalsElement->FirstChildElement("drawing");
		if (drawingElement)
		{
			printf("- Drawing:\n");
			char* mode = NULL;
			char* shading = NULL;

			mode = (char *) drawingElement->Attribute("mode");
			shading = (char *) drawingElement->Attribute("shading");

			if (strcmp(mode, "fill") != 0 && strcmp(mode, "line") != 0 && strcmp(mode, "point") != 0){
				printf("Error parsing drawing: wrong/missing mode variable\n");
				mode = "error";
			}
			if (strcmp(shading, "gouraud") != 0 && strcmp(shading, "flat") != 0){
				printf("Error parsing drawing: missing shading variable\n");
				shading = "error";
			}
			
			printf("	mode: %s\n	shading: %s\n", mode, shading);

			char *background=NULL;
			float red, green, blue, alpha;

			background=(char *) drawingElement->Attribute("background");
			//if(background && sscanf(background,"%f %f %f %f",&red, &green, &blue, &alpha)== 4)
			if(background && readRGBcomponents(background,red, green, blue, alpha))
			{
					printf("	background: R-%f, G-%f, B-%f, A-%f\n", red, green, blue, alpha);
			}
			else{
				printf("Error parsing background\n");
			}
		}
		else
			printf("drawing not found\n");

		/////////////////end drawing////////////

		///////culling//////////
		TiXmlElement* cullingElement = globalsElement->FirstChildElement("culling");
		if (cullingElement)
		{
			printf("- Culling:\n");
			char* face = NULL;
			char* order = NULL;

			face = (char *) cullingElement->Attribute("face");
			order = (char *) cullingElement->Attribute("order");

			if (strlen(face) == 0){ //aqui tem de verificar as opções ainda...
				printf("Error parsing culling: missing face variable\n");
				face = "error";
			}
			if (strcmp(order,"cw") != 0 && strcmp(order,"ccw") != 0){ 
				printf("Error parsing culling: missing/wrong order variable\n");
				order = "error";
			}
			
			printf("	face: %s\n	order: %s\n", face, order);
		}
		else
			printf("culling not found\n");
		////////////////end culling////////////////


		////////////lighting/////////////
		TiXmlElement* lightingElement = globalsElement->FirstChildElement("lighting");
		if (lightingElement)
		{
			printf("- Lighting:\n");

			char* doublesided = NULL;
			char* local = NULL;
			char* enabled = NULL;

			doublesided = (char *) lightingElement->Attribute("doublesided");
			local = (char *) lightingElement->Attribute("local");
			enabled = (char *) lightingElement->Attribute("enabled");

			if (strcmp(doublesided,"true") != 0 && strcmp(doublesided,"false") != 0){
				printf("Error parsing lighting: missing or wrong type doublesided variable\n");
				doublesided = "error";
			}
			if (strcmp(local,"true") != 0 && strcmp(local,"false") != 0){
				printf("Error parsing lighting: missing or wrong type local variable\n");
				local = "error";
			}
			if (strcmp(enabled,"true") != 0 && strcmp(enabled,"false") != 0){
				printf("Error parsing lighting: missing or wrong type enabled variable\n");
				enabled = "error";
			}

			printf("	doublesided: %s\n	local: %s\n	enabled: %s\n", doublesided, local, enabled);
		

			char *ambient=NULL;
			float red, green, blue, alpha;

			ambient=(char *) lightingElement->Attribute("ambient");
			if(ambient && readRGBcomponents(ambient,red, green, blue, alpha))
			{
					printf("	ambient: R-%f, G-%f, B-%f, A-%f\n", red, green, blue, alpha);
			}
			else{
				printf("Error parsing ambient\n");
			}
		}
		else
			printf("lighting not found\n");

		/////////////////end lighting ////////////////////////
	}

	////////////////////// END GLOBALS /////////////////////



	///////////////CAMERAS/////////////////
	

	//////////////END OF CAMERAS////////////
	
	


	/////////////////LIGHTS///////////////


	///////////////END OF LIGHTS/////////





	/////////////////TEXTURES///////////////


	///////////////END OF TEXTURES/////////





	/////////////////APPEARANCES///////////////


	///////////////END OF APPEARANCES/////////





	
	/////////////////GRAPH///////////////


	///////////////END OF GRAPH/////////



	

	//////////////////////OLD EXAMPLE CODE/////////////////////
	// Init
	// An example of well-known, required nodes

	
	/*
	if (initElement == NULL)
		printf("Init block not found!\n");
	else
	{
		printf("Processing init:\n");
		// frustum: example of a node with individual attributes
		TiXmlElement* frustumElement=initElement->FirstChildElement("frustum");
		if (frustumElement)
		{
			float near,far;

			if (frustumElement->QueryFloatAttribute("near",&near)==TIXML_SUCCESS && 
				frustumElement->QueryFloatAttribute("far",&far)==TIXML_SUCCESS
				)
				printf("  frustum attributes: %f %f\n", near, far);
			else
				printf("Error parsing frustum\n");
		}
		else
			printf("frustum not found\n");


		// translate: example of a node with an attribute comprising several float values
		// It shows an example of extracting an attribute's value, and then further parsing that value 
		// to extract individual values
		TiXmlElement* translateElement=initElement->FirstChildElement("translate");
		if (translateElement)
		{
			char *valString=NULL;
			float x,y,z;

			valString=(char *) translateElement->Attribute("xyz");

			if(valString && sscanf(valString,"%f %f %f",&x, &y, &z)==3)
			{
				printf("  translate values (XYZ): %f %f %f\n", x, y, z);
			}
			else
				printf("Error parsing translate");
		}
		else
			printf("translate not found\n");		

		// repeat for each of the variables as needed
	}

	// Other blocks could be validated/processed here
	*/

	// graph section


	/*
	if (graphElement == NULL)
		printf("Graph block not found!\n");
	else
	{
		char *prefix="  -";
		TiXmlElement *node=graphElement->FirstChildElement();

		while (node)
		{
			printf("Node id '%s' - Descendants:\n",node->Attribute("id"));
			TiXmlElement *child=node->FirstChildElement();
			while (child)
			{
				if (strcmp(child->Value(),"Node")==0)
				{
					// access node data by searching for its id in the nodes section
					
					TiXmlElement *noderef=findChildByAttribute(nodesElement,"id",child->Attribute("id"));

					if (noderef)
					{
						// print id
						printf("  - Node id: '%s'\n", child->Attribute("id"));

						// prints some of the data
						printf("    - Material id: '%s' \n", noderef->FirstChildElement("material")->Attribute("id"));
						printf("    - Texture id: '%s' \n", noderef->FirstChildElement("texture")->Attribute("id"));

						// repeat for other leaf details
					}
					else
						printf("  - Node id: '%s': NOT FOUND IN THE NODES SECTION\n", child->Attribute("id"));

				}
				if (strcmp(child->Value(),"Leaf")==0)
				{
					// access leaf data by searching for its id in the leaves section
					TiXmlElement *leaf=findChildByAttribute(leavesElement,"id",child->Attribute("id"));

					if (leaf)
					{
						// it is a leaf and it is present in the leaves section
						printf("  - Leaf id: '%s' ; type: '%s'\n", child->Attribute("id"), leaf->Attribute("type"));

						// repeat for other leaf details
					}
					else
						printf("  - Leaf id: '%s' - NOT FOUND IN THE LEAVES SECTION\n",child->Attribute("id"));
				}

				child=child->NextSiblingElement();
			}
			node=node->NextSiblingElement();
		}
	}*/

}

bool XMLSceneMod::readRGBcomponents (char* rawString, float &R, float &G, float &B, float &A){
	if (sscanf(rawString,"%f %f %f %f",&R, &G, &B, &A) != 4)
		return false;
	if ( R>1 || G>1 || B>1 || A>1 )
		return false;
	if ( R<0 || G<0 || B<0 || A<0 )
		return false;
	return true;
}


XMLSceneMod::~XMLSceneMod()
{
	delete(doc);
}

//-------------------------------------------------------

TiXmlElement *XMLSceneMod::findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
// Searches within descendants of a parent for a node that has an attribute _attr_ (e.g. an id) with the value _val_
// A more elaborate version of this would rely on XPath expressions
{
	TiXmlElement *child=parent->FirstChildElement();
	int found=0;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr),val)==0)
			found=1;
		else
			child=child->NextSiblingElement();

	return child;
}


