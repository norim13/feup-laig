#include "XMLSceneMod.h"

#include "Graph.h"
using namespace std;
#include <iostream>
#include "glui.h"
#include "glut.h"
#include "GL/glui.h"
#include "GL/glut.h"
//#include "Node.h"

//como vamos guardar os Appearances? talvez uma especia de map, tabela tipo <id,CGFappearance>
//� preciso por os nos com links entre eles?!?*****

XMLSceneMod::XMLSceneMod(char *filename)
{

	// Read XML from file
	Graph g = Graph();
	
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

	globalsElement = dgxElement->FirstChildElement( "globals" );
	//camerasElement = dgxElement->FirstChildElement( "cameras" );
	graphElement = dgxElement->FirstChildElement( "graph" );
	appearancesElement=dgxElement->FirstChildElement("appearances");

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

			if (strlen(face) == 0){ //aqui tem de verificar as op��es ainda...
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

	cout<<"\nParcing appearances:\n";

	if(appearancesElement==NULL)
		printf("appearances block not found!\n");



	TiXmlElement * appearances = appearancesElement->FirstChildElement("appearance");
	CGFappearance * appearance;
	string id,textureref;
	float shininess;

	while(appearances){

		CGFappearance appearanceObject= CGFappearance();

		//read the appearance's id
		id = string(appearances->Attribute("id"));
		if(id != ""){
			cout<<"-Appearance id: "<<id<<endl;
		}
		else cout<<"Error parsing appearance: missing id\n";

		//read the appearance's shininess
		char* ss = (char*)appearances->Attribute("shininess");
			sscanf(ss,"%f",&shininess);
		if(ss != ""){
			cout<<"        Shininess: "<<shininess<<endl;
		}
		else cout<<"        Error parsing appearance: missing shininess\n";

		appearanceObject.setShininess(shininess);

		//read the appearance's textured reference
		textureref = string(appearances->Attribute("textureref"));
		if(textureref != ""){
			cout<<"        Textureref: "<<textureref<<endl;
		}
		else cout<<"        Error parsing appearance: missing textureref\n";

		//appearanceObject.setTexture(textureref);									//� preciso descomentar para por a textura

		//read the appearence's elements(ambient,diffuse,specular)
		TiXmlElement *component = appearances->FirstChildElement("component");  
		string type;
		float floatVector[4];
		bool a,d,s;
		a=d=s=false;
		while(component)
		{
			

			type = string(component->Attribute("type"));
			cout<<"        Type:"<<type;
			cout<<"-> "<<(char*)component->Attribute("value")<<endl;
			if(!readFloatArray((char*)component->Attribute("value"),floatVector))
				cout<<"        Error reading values"<<endl;
			if(type=="ambient")
			{
				a=true;
				

				appearanceObject.setAmbient(&floatVector[0]);
			}
			else if(type=="diffuse")
			{
				d=true;
				appearanceObject.setDiffuse((&floatVector)[0]);
			}
			else if(type=="specular")
			{
				s=true;
				appearanceObject.setSpecular((&floatVector)[0]);
			}

			component=component->NextSiblingElement();
		}

		//If all the elements(ambient, diffuse, specular) are present
		if(a&&d&&s)
		{
			
		}
		else
			cout<<"        One or more atributes couldn't be found\n";

		cout<<endl;
		appearances = appearances->NextSiblingElement("appearance");
}


	///////////////END OF APPEARANCES/////////





	
	/////////////////GRAPH///////////////
	//in progess

	if (graphElement == NULL)
		printf("Graph block not found!\n");
	else
	{
		//criar grafo aqui
		printf("\nProcessing Graph:\n");
		//falta ler o atributo "root" do graph

		char *prefix="  -";
		TiXmlElement *node = graphElement->FirstChildElement("node");

		while (node)
		{
			printf("-Node id: %s\n",node->Attribute("id"));
			char* charString=(char *)node->Attribute("id");

			string s=string(charString);
			//cout<<s;
			Node n= Node(s);
			//cout<<"id do no"+n.getId();

			TiXmlElement *transforms  = node->FirstChildElement();
			if (!transforms){
				printf("	Error: Transforms block not found!\n");
				break;
			}
			printf("	Transforms:\n");
			TiXmlElement *transform = transforms->FirstChildElement("transform");  
			
			//inicia a matriz com a matriz identidade
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			while (transform)
			{

				char* tipo = (char*) transform->Attribute("type");
							
				
				if (strcmp(tipo,"translate") == 0){
					char* to = (char*) transform->Attribute("to");
					printf("	-Type: %s -> %s\n", tipo, to);

					float x,y,z;
					readXYZcoord(to,x,y,z);
					glTranslatef(x,y,z);
				} 
				else if (strcmp(tipo,"rotate") == 0){					
					char* axis = (char*) transform->Attribute("axis");
					char* angle = (char*) transform->Attribute("angle");
					printf("	-Type: %s -> axis: %s, angle: %s\n", tipo, axis, angle);


					float angulo;
					sscanf(angle,"%f",&angulo);
					if(axis=="xx")
						glRotatef(angulo, 1,0,0);
					else if(axis=="yy")
						glRotatef(angulo, 0,1,0);
					else if(axis=="zz")
						glRotatef(angulo, 0,0,1);
				} 

				else if (strcmp(tipo,"scale") == 0){
					char* factor = (char*) transform->Attribute("factor");
					printf("	-Type: %s -> %s\n", tipo, factor);

					float x,y,z;
					readXYZcoord(factor,x,y,z);

					glScalef(x,y,z);

				} 
				else printf("	Missing/invalid transform\n");
				
				
				transform = transform->NextSiblingElement();
			}


			//por a matriz final em m e depois atribuir m � matriz transforma��o do n�
			float m[16];
			glGetFloatv(GL_MODELVIEW_MATRIX,m);
			n.setMatrix(&m[0]);
			


			TiXmlElement *appearanceref  = node->FirstChildElement("appearanceref");
			if (!appearanceref){
				printf("	Error: Appearances block not found!\n");
				break;
			}
			char* appearance = (char*) appearanceref->Attribute("id"); 
			printf("	Appearance: %s\n", appearance);

			
			
			TiXmlElement *primitives  = node->FirstChildElement("primitives");
			if (primitives){
				printf("	Primitives:\n");
				TiXmlElement *primitive = primitives->FirstChildElement(); 
				while (primitive)
				{

					//////////////rectangle//////////////
					if (strcmp("rectangle", primitive->Value()) == 0){
						char* xy1 = (char*) primitive->Attribute("xy1");
						char* xy2 = (char*) primitive->Attribute("xy2");
						float x1, y1, x2, y2;
						if (readXYcoord(xy1, x1, y1) && readXYcoord(xy2, x2, y2))
							printf("		rectangle: xy1-%.2f %.2f, xy2-%.2f %.2f\n", x1, y1, x2, y2);
						else printf("		rectangle: invalid values or wrong format\n");
					}


					////////////triangle///////////
					else if (strcmp("triangle", primitive->Value()) == 0){
						char* xyz1 = (char*) primitive->Attribute("xyz1");
						char* xyz2 = (char*) primitive->Attribute("xyz2");
						char* xyz3 = (char*) primitive->Attribute("xyz3");
						float x1,x2,x3,y1,y2,y3,z1,z2,z3;
						if (readXYZcoord(xyz1, x1, y1, z1) && readXYZcoord(xyz2, x2, y2, z2)
							&& readXYZcoord(xyz3, x3, y3, z3))
						{
							printf("		triangle:\n");
							printf("			xyz1-%.2f %.2f %.2f,\n", x1, y1, z1);
							printf("			xyz2-%.2f %.2f %.2f,\n", x2, y2, z2);
							printf("			xyz3-%.2f %.2f %.2f\n", x3, y3, z3);
						}
						else printf("		triangle: invalid values or wrong format\n");
					}


					////////////cylinder//////////////
					else if (strcmp("cylinder", primitive->Value()) == 0){
						char* base = (char*) primitive->Attribute("base");
						char* top = (char*) primitive->Attribute("top");
						char* height = (char*) primitive->Attribute("height");
						char* slices = (char*) primitive->Attribute("slices");
						char* stacks = (char*) primitive->Attribute("stacks");
						printf("		cylinder: base-%s, top-%s, height-%s, slices-%s, stacks-%s\n",
							base, top, height, slices, stacks);
					}


					/////////////sphere////////////////
					else if (strcmp("sphere", primitive->Value()) == 0){
						char* radius = (char*) primitive->Attribute("radius");
						char* slices = (char*) primitive->Attribute("slices");
						char* stacks = (char*) primitive->Attribute("stacks");
						printf("		sphere: radius-%s, slices-%s, stacks-%s\n", radius, slices, stacks);
					}


					///////////////torus///////////////
					else if (strcmp("torus", primitive->Value()) == 0){
						char* inner = (char*) primitive->Attribute("inner");
						char* outer = (char*) primitive->Attribute("outer");
						char* slices = (char*) primitive->Attribute("slices");
						char* loops = (char*) primitive->Attribute("loops");
						printf("		torus: inner-%s, outer-%s, slices-%s, loops-%s\n",
							inner, outer, slices, loops);
					}
					else printf("		Invalid primitive detected\n");

					

					primitive = primitive->NextSiblingElement();
				}
				


			TiXmlElement *descendants  = node->FirstChildElement("descendants");
			if (!descendants){
				printf("	Error: Descendants block not found!\n");
				break;
			}

			TiXmlElement *noderef = descendants->FirstChildElement("noderef");  

			string idDesc;
			cout<<"	Descendentes: ";
			while(noderef)
				{
					idDesc = string(noderef->Attribute("id"));
					n.addDescendente(idDesc);
					cout<<idDesc<<"  ";
					noderef=noderef->NextSiblingElement();
				}
			cout<<endl;
			}
			g.addNode(n);
			node = node->NextSiblingElement();
		}
		//ao sair do while, � preciso verificar se existe pelo menos um n�
	}

	//mostrar dois nos
/*
	cout<<g.searchForNode("root")->mostrarNo();
	cout<<g.searchForNode("root")->getDescendentes().size();

	cout<<g.searchForNode("second")->mostrarNo();
	cout<<g.searchForNode("second")->getDescendentes().size();
	*/

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

bool XMLSceneMod::readXYcoord (char* rawString, float &x, float &y){
	return (sscanf(rawString,"%f %f",&x, &y) == 2);
}

bool XMLSceneMod::readXYZcoord (char* rawString, float &x, float &y, float &z){
	return (sscanf(rawString,"%f %f %f",&x, &y, &z) == 3);
}

bool XMLSceneMod::readFloatArray(char* rawString, float (&a)[4])
{
	return(sscanf(rawString,"%f %f %f %f",&a[0],&a[1],&a[2],&a[3]));
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


