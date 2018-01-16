#include "tbConfig.h"
#include "tbParser.h"
#include "tbMemory.h"

enum TB_PARSER_ERRORS
{
	TB_PARSER_INVALID_SIMULATION,
	TB_PARSER_ONLYONE_SIMULATION,

	TB_PARSER_INVALID_WINDOW,
	TB_PARSER_ONLYONE_WINDOW,
	TB_PARSER_ORDER_WINDOW,
	TB_PARSER_WIDTH_WINDOW,
	TB_PARSER_HEIGHT_WINDOW,
	TB_PARSER_BPP_WINDOW,
	TB_PARSER_QUALITY_WINDOW,

	TB_PARSER_ORDER_SHAPE,
	TB_PARSER_INVALID_SHAPE,
	TB_PARSER_INVALIDBOX_SHAPE,
	TB_PARSER_INVALIDCIRCLE_SHAPE,
	TB_PARSER_TYPE_SHAPE,

	TB_PARSER_INVALID_OBJECT,
	TB_PARSER_TOOMANY_OBJECT,
	TB_PARSER_SHAPEINDEX_OBJECT,

	TB_PARSER_EXTRACHARACTERS,
	TB_PARSER_ERROR_MAX
};

/* The error string table */
static const char* tbErrors[TB_PARSER_ERROR_MAX]=
{"Invalid \"simulation\" command, use \"simulation [objects count] [step] [gravity]\" (line %d)",
"Only one simulation declaration is allowed (line %d)",
"Incomplete \"window\" command, use \"window [width] [height] ([bits per pixel] [scale] [quality])\" (line %d)",
"Only one window declaration is allowed (line %d)",
"The \"window\" declaration must come after the \"simulation\" command (line %d)",
"Maximum window width is "TB_WINDOW_MAXWIDTH" (line %d)",
"Maximum window height is "TB_WINDOW_MAXHEIGHT" (line %d)",
"Invalid screen buffer depth, choose one of {16, 24, 32} (line %d)",
"Invalid rendering quality, must be between 5 and 100 (line %d)",
"No shape declaration allowed after an \"object\" command (line %d)",
"Incomplete shape command, use \"shape [restitution] [friction] [type] [options]\" (line %d)",
"Incomplete box shape command, use \"shape [restitution] [friction] box [half width] [half height]\" (line %d)",
"Incomplete circle shape command, use \"shape [restitution] [friction] circle [radius]\" (line %d)",
"Invalid shape type, use one of {box, circle} (line %d)",
"Incomplete object command, use \"object [shape index] [flags] [mass] [x] [y] [angle] ([x speed] [y speed] [angular velocity])\" (line %d)",
"Too many object declarations, see the simulation you declared before (line %d)",
"Shape index out of bounds, see the shapes you declared before (line %d)",
"Extra characters on current line (line %d)"};

static void tbParserError(tbParser*,unsigned int);
static int tbSkipSpaces(char*,char**);
static int tbReadVector(char*,tbVector,char**);
static int tbReadScalar(char*,tbScalar*,char**);
static int tbReadInteger(char*,int*,char**);
static int tbReadString(char*,char*,unsigned int,char**);
static int tbCheckEndOfLine(char*);

static int tbSimulationCommand(tbParser*,char*);
static int tbWindowCommand(tbParser*,char*);
static int tbShapeCommand(tbParser*,char*);
static int tbObjectCommand(tbParser*,char*);

static char buffer[TB_PARSER_BUFFER];
static char command[TB_PARSER_COMMAND];

tbParser* tbCreateParser(const char* fileName,tbApplication* application)
{
	tbParser* parser;

	parser=tbAlloc(sizeof(tbParser));
	tbMemorySet(parser,0,sizeof(tbParser));
	parser->fileName=fileName;
	parser->application=application;

	return parser;
}

static void tbParserError(tbParser* parser,unsigned int error)
{
	sprintf(parser->errorMessage,tbErrors[error],parser->line);
}

static int tbSkipSpaces(char* text,char** ptr)
{
	unsigned int i=0;
	while(text[i]!='\0'&&isspace(text[i])) i++;
	*ptr=text+i;
	return i;
}

static int tbReadVector(char* text,tbVector vector,char** ptr)
{
	if(!tbReadScalar(text,&vector[0],ptr))
		return 0;
	return tbReadScalar(*ptr,&vector[1],ptr);
}

static int tbReadScalar(char* text,tbScalar* scalar,char** ptr)
{
	unsigned int i=0;
	char tmp;
	if(text[0]=='#') return 0;
	while(text[i]!='\0'&&!isspace(text[i])) i++;
	tmp=text[i];
	text[i]='\0';
	*scalar=(tbScalar)atof(text);
	text[i]=tmp;
	*ptr=text+i;
	if(i)
		tbSkipSpaces(*ptr,ptr);
	return i;
}

static int tbReadInteger(char* text,int* integer,char** ptr)
{
	unsigned int i=0;
	char tmp;
	if(text[0]=='#') return 0;
	while(text[i]!='\0'&&!isspace(text[i])) i++;
	tmp=text[i];
	text[i]='\0';
	*integer=atoi(text);
	text[i]=tmp;
	*ptr=text+i;
	if(i)
		tbSkipSpaces(*ptr,ptr);
	return i;
}

static int tbReadString(char* text,char* string,unsigned int n,char** ptr)
{
	unsigned int i=0;

	while(i<n&&text[i]!='\0'&&!isspace(text[i]))
	{
		string[i]=text[i];
		i++;
	}

	if(i>=n)
	{
		string[n-1]='\0';
		return 0;
	}

	string[i]='\0';
	*ptr=text+i;
	if(i)
		tbSkipSpaces(*ptr,ptr);
	return i;
}

static int tbCheckEndOfLine(char* line)
{
	unsigned int i=0;
	while(line[i]!='\0'&&isspace(line[i])) i++;
	return (line[i]=='#')||(line[i]=='\0');
}

static int tbSimulationCommand(tbParser* parser,char* text)
{
	tbApplication* application=parser->application;
	unsigned int numObjects;
	tbScalar gravity;
	char* ptr;
	
	if(application->simulation)
	{
		tbParserError(parser,TB_PARSER_ONLYONE_SIMULATION);
		return 0;
	}

	if(!tbReadInteger(text,&numObjects,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_SIMULATION);
		return 0;
	}

	if(!tbReadScalar(ptr,&application->timeStep,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_SIMULATION);
		return 0;
	}

	if(!tbReadScalar(ptr,&gravity,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_SIMULATION);
		return 0;
	}

	if(!tbCheckEndOfLine(ptr))
	{
		tbParserError(parser,TB_PARSER_EXTRACHARACTERS);
		return 0;
	}

	application->broadphase=tbCreateBroadphase(0);
	application->simulation=tbCreateSimulation(numObjects,application->broadphase);
	application->simulation->gravity=gravity;

	return 1;
}

static int tbWindowCommand(tbParser* parser,char* text)
{
	const tbVector center={0,0};
	tbApplication* application=parser->application;
	unsigned int width,height,bpp,quality;
	tbScalar scale;
	char* ptr;
	
	if(application->window)
	{
		tbParserError(parser,TB_PARSER_ONLYONE_WINDOW);
		return 0;
	}

	if(!application->simulation)
	{
		tbParserError(parser,TB_PARSER_ORDER_WINDOW);
		return 0;
	}

	if(!tbReadInteger(text,&width,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_WINDOW);
		return 0;
	}

	if(width>(unsigned int)atoi(TB_WINDOW_MAXWIDTH))
	{
		tbParserError(parser,TB_PARSER_WIDTH_WINDOW);
		return 0;
	}

	if(!tbReadInteger(ptr,&height,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_WINDOW);
		return 0;
	}

	if(height>(unsigned int)atoi(TB_WINDOW_MAXHEIGHT))
	{
		tbParserError(parser,TB_PARSER_HEIGHT_WINDOW);
		return 0;
	}

	if(!tbReadInteger(ptr,&bpp,&ptr))
	{
		bpp=16;
		scale=(tbScalar)20.0/(tbScalar)width;
		quality=25;
	}
	else
	{
		if((bpp!=16)&&(bpp!=24)&&(bpp!=32))
		{
			tbParserError(parser,TB_PARSER_BPP_WINDOW);
			return 0;
		}

		if(!tbReadScalar(ptr,&scale,&ptr))
		{
			tbParserError(parser,TB_PARSER_INVALID_WINDOW);
			return 0;
		}
		if(!tbReadInteger(ptr,&quality,&ptr))
		{
			tbParserError(parser,TB_PARSER_INVALID_WINDOW);
			return 0;
		}

		if(quality<5||quality>100)
		{
			tbParserError(parser,TB_PARSER_QUALITY_WINDOW);
			return 0;
		}
	}

	if(!tbCheckEndOfLine(ptr))
	{
		tbParserError(parser,TB_PARSER_EXTRACHARACTERS);
		return 0;
	}

	application->window=tbCreateWindow(width,height,bpp);
	application->renderer=tbCreateRenderer(application->window,quality);
	application->interface=tbCreateInterface(application->simulation,application->window);
	
	tbSetView(application->window,center,scale);
	tbSetCursor(application->window,TB_CURSOR_ARROW);

	return 1;
}

static int tbShapeCommand(tbParser* parser,char* text)
{
	tbApplication* application=parser->application;
	tbShape* shape=0;
	tbScalar restitution,friction;
	char* ptr;
	
	if(parser->currentObject>0)
	{
		tbParserError(parser,TB_PARSER_ORDER_SHAPE);
		return 0;
	}

	if(!tbReadScalar(text,&restitution,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_SHAPE);
		return 0;
	}

	if(!tbReadScalar(ptr,&friction,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_SHAPE);
		return 0;
	}	

	if(!tbReadString(ptr,command,TB_PARSER_COMMAND,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_SHAPE);
		return 0;
	}

	if(!strcmp(command,"box"))
	{
		tbVector halfExtents;

		if(!tbReadVector(ptr,halfExtents,&ptr))
		{
			tbParserError(parser,TB_PARSER_INVALIDBOX_SHAPE);
			return 0;
		}

		shape=tbCreateBox(halfExtents);
	}

	if(!strcmp(command,"circle"))
	{
		tbScalar radius;

		if(!tbReadScalar(ptr,&radius,&ptr))
		{
			tbParserError(parser,TB_PARSER_INVALIDCIRCLE_SHAPE);
			return 0;
		}

		shape=tbCreateCircle(radius);
	}

	if(!tbCheckEndOfLine(ptr))
	{
		tbParserError(parser,TB_PARSER_EXTRACHARACTERS);
		return 0;
	}

	if(shape)
	{
		shape->friction=friction;
		shape->restitution=restitution;

		application->numShapes++;
		if(application->shapes)
			application->shapes=tbRealloc(application->shapes,sizeof(tbShape*)*application->numShapes);
		else
			application->shapes=tbAlloc(sizeof(tbShape*)*application->numShapes);
		application->shapes[application->numShapes-1]=shape;
		return 1;
	}
	
	tbParserError(parser,TB_PARSER_TYPE_SHAPE);
	return 0;
}

static int tbObjectCommand(tbParser* parser,char* text)
{
	tbApplication* application=parser->application;
	tbObject* object;
	unsigned int shape;
	tbScalar mass;
	char* ptr;

	if(parser->currentObject>=application->simulation->numObjects)
	{
		tbParserError(parser,TB_PARSER_TOOMANY_OBJECT);
		return 0;
	}

	object=&application->simulation->objects[parser->currentObject];
	if(!tbReadInteger(text,&shape,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_OBJECT);
		return 0;
	}

	if(shape>=application->numShapes)
	{
		tbParserError(parser,TB_PARSER_SHAPEINDEX_OBJECT);
		return 0;
	}

	object->shape=application->shapes[shape];

	tbSkipSpaces(ptr,&ptr);
	if(!tbReadInteger(ptr,&object->flags,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_OBJECT);
		return 0;
	}

	tbSkipSpaces(ptr,&ptr);
	if(!tbReadScalar(ptr,&mass,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_OBJECT);
		return 0;
	}
	/* TODO : Do something better */
	object->force[1]=-1;
	if(object->flags&TB_OBJECT_STATIC)
	{
		object->inverseMass=0;
		object->inverseInertia=0;
	}
	else
		tbSetMass(object,mass);

	tbSkipSpaces(ptr,&ptr);
	if(!tbReadVector(ptr,object->state.position,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_OBJECT);
		return 0;
	}

	tbSkipSpaces(ptr,&ptr);
	if(!tbReadScalar(ptr,&object->state.angle,&ptr))
	{
		tbParserError(parser,TB_PARSER_INVALID_OBJECT);
		return 0;
	}
	tbSetAngle(&object->state,object->state.angle);

	tbSkipSpaces(ptr,&ptr);
	if(tbReadScalar(ptr,&object->state.velocity[0],&ptr))
	{
		tbSkipSpaces(ptr,&ptr);
		if(!tbReadScalar(ptr,&object->state.velocity[1],&ptr))
		{
			tbParserError(parser,TB_PARSER_INVALID_OBJECT);
			return 0;
		}

		tbSkipSpaces(ptr,&ptr);
		if(!tbReadScalar(ptr,&object->state.angularVelocity,&ptr))
		{
			tbParserError(parser,TB_PARSER_INVALID_OBJECT);
			return 0;
		}
	}

	if(!tbCheckEndOfLine(ptr))
	{
		tbParserError(parser,TB_PARSER_EXTRACHARACTERS);
		return 0;
	}

	parser->currentObject++;

	return 1;
}

int tbParse(tbParser* parser)
{
	tbApplication* application=parser->application;
	char* ptr;
	FILE* fp;

	tbMemorySet(application,0,sizeof(tbApplication));

	fp=fopen(parser->fileName,"r");
	if(!fp)
	{
		sprintf(parser->errorMessage,"Cannot open file \"%s\"",parser->fileName);
		return 0;
	}

	parser->line=0;
	parser->currentObject=0;

	while(!feof(fp))
	{
		parser->line++;

		if(!fgets(buffer,TB_PARSER_BUFFER,fp))
			break;

		tbSkipSpaces(buffer,&ptr);
		if(!tbReadString(ptr,command,TB_PARSER_COMMAND,&ptr))
			continue;

		if(command[0]=='#')
			continue;

		if(!strcmp(command,"object"))
		{
			if(!tbObjectCommand(parser,ptr))
			{
				fclose(fp);
				return 0;
			}
			continue;
		}

		if(!strcmp(command,"simulation"))
		{
			if(!tbSimulationCommand(parser,ptr))
			{
				fclose(fp);
				return 0;
			}
			continue;
		}

		if(!strcmp(command,"window"))
		{
			if(!tbWindowCommand(parser,ptr))
			{
				fclose(fp);
				return 0;
			}
			continue;
		}

		if(!strcmp(command,"shape"))
		{
			if(!tbShapeCommand(parser,ptr))
			{
				fclose(fp);
				return 0;
			}
			continue;
		}	

		sprintf(parser->errorMessage,"Unknown command \"%s\" (line %d)",command,parser->line);
		fclose(fp);
		return 0;
	}

	fclose(fp);

	if(!application->simulation||!application->window)
	{
		sprintf(parser->errorMessage,"You must set up rendering and physics, use \"simulation\" or \"window\"");
		return 0;
	}

	if(parser->currentObject!=application->simulation->numObjects)
	{
		sprintf(parser->errorMessage,"You must initialize every simulation object, use the \"object\" command");
		return 0;
	}

	return 1;
}

void tbDestroyParser(tbParser* parser)
{
	tbFree(parser);
}
