SET(MATH_C tbVector.c
	tbMatrix.c)

SET(MATH_H tbMath.h
	tbMatrix.h
	tbVector.h)

ADD_LIBRARY(math STATIC ${MATH_C} ${MATH_H})
TARGET_LINK_LIBRARIES(math m)

