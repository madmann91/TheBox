#include "tbCommon.h"
#include "tbConfig.h"

#include "tbMath.h"
#include "tbCollision.h"
#include "tbShapes.h"
#include "tbPhysics.h"
#include "tbSystem.h"

#include "tbMemory.h"

void tbLogo(void)
{
	tbPrint("TheBox2 by Arsene Perard - Copyright(C) 2010/2011\n"
		"An experimental physics simulation sofware\n"
		"This version was compiled in "
#ifdef TB_DOUBLE
	"double precision "
#else
	"single precision "
#endif
#ifdef TB_STATISTICS
	"with statistics\n\n"
#else
	"without statistics\n\n"
#endif
	);

#ifdef DEBUG
	tbMessage("Warning : This is a debug version, simulation may be slower than expected");
#endif
}

int main(int argc,char** argv)
{
	tbParser* parser;
	tbApplication application;
#ifdef TB_STATISTICS
	unsigned long ticks;
#endif

	tbLogo();
	if(argc<=1)
	{
		tbMessage("Error : Not enough arguments, use \"TheBox2 [simulationfile]\"");
		return 1;
	}
	if(argc>2)
	{
		tbMessage("Error : Too many arguments, use \"TheBox2 [simulationfile]\"");
		return 1;
	}

	tbMessage("Loading simulation from file \"%s\"",argv[1]);
	parser=tbCreateParser(argv[1],&application);
	if(!tbParse(parser))
	{
		tbMessage("Error : %s",parser->errorMessage);
		tbDestroyParser(parser);
		tbDestroyApplication(&application);
		return 1;
	}
	tbDestroyParser(parser);

#ifdef TB_STATISTICS
	ticks=tbGetTicks();
#endif
	while(tbUpdateInterface(application.interface))
	{
		tbUpdateSimulation(application.simulation,application.timeStep);
		tbRender(application.renderer,application.simulation);
#ifdef TB_STATISTICS
		if(tbGetTicks()-ticks>TB_STATISTICS_TICKS)
		{
			tbMessage("Statistics : %f %f %f",
				(tbScalar)(application.simulation->broadphaseTicks*CLOCKS_PER_SEC)/(tbScalar)application.simulation->frames,
				(tbScalar)(application.simulation->solverTicks*CLOCKS_PER_SEC)/(tbScalar)application.simulation->frames,
				(tbScalar)(application.renderer->ticks*CLOCKS_PER_SEC)/(tbScalar)application.renderer->frames);
			ticks=tbGetTicks();

			/* Reset counters */
			application.simulation->broadphaseTicks=0;
			application.simulation->solverTicks=0;
			application.simulation->frames=0;

			application.renderer->ticks=0;
			application.renderer->frames=0;
		}
#endif
	}
	tbDestroyApplication(&application);

	tbMessage("Now exiting...");
#ifdef DEBUG
	tbMessage("Memory : used %d, freed %d, leak %d",tbMemoryUsed,tbMemoryFreed,tbMemoryUsed-tbMemoryFreed);
#endif

	return 0;
}
