SET(COLLISION_C tbAlgorithm.c
    tbPairs.c
	#tbTreeBroadphase.c
	tbBroadphase.c)

SET(COLLISION_H tbAlgorithm.h
    tbPairs.h
	#tbTreeBroadphase.h
	tbBroadphase.h
	tbCollision.h)

ADD_LIBRARY(collision STATIC ${COLLISION_C} ${COLLISION_H})
TARGET_LINK_LIBRARIES(collision math m)

