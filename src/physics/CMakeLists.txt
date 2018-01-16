SET(PHYSICS_C tbObject.c
	tbSolver.c
	tbSimulation.c
	tbShapes.c)

SET(PHYSICS_H tbPhysics.h
	tbObject.h
	tbSolver.h
	tbSimulation.h
	tbShapes.h)

ADD_LIBRARY(physics STATIC ${PHYSICS_C} ${PHYSICS_H})
TARGET_LINK_LIBRARIES(physics collision math m)

