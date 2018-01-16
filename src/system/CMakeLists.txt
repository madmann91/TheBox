SET(SYSTEM_C tbWindow.c
	tbInterface.c
	tbRenderer.c
	tbParser.c
	tbApplication.c
	tbConsole.c
	tbResources.c)

SET(SYSTEM_H tbSystem.h
	tbWindow.h
	tbInterface.h
	tbRenderer.h
	tbParser.h
	tbApplication.h
	tbConsole.h
	tbResources.h)

ADD_LIBRARY(system STATIC ${SYSTEM_C} ${SYSTEM_H})
TARGET_LINK_LIBRARIES(system physics collision math m)

