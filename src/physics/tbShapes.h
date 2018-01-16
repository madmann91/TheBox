/************************************************************************
*                                                                       *
*  File : tbShapes.h                                                    *
*  Date : 26/10/10                                                      *
*  Comments : Object shapes                                             *
*  Author : "MadMann" A. PERARD (arsene<dot>perard<at>laposte<dot>net)  *
*                                                                       *
************************************************************************/

#ifndef __TB_SHAPES_H__
#define __TB_SHAPES_H__

#include "tbCommon.h"
#include "tbVector.h"

/*! \addtogroup physics */
/*! @{ */

/*! \name Shapes */
/*! @{ */

/*! Shape types */
enum TB_SHAPE_TYPES
{
	TB_SHAPE_BOX,		/*!< A box shape */
	TB_SHAPE_CIRCLE,	/*!< A circle shape */
	TB_SHAPE_MAX
};

/*! Box shape object */
typedef struct tbBox_s
{
	tbVector halfExtents;		/*< The box half-extents */
}tbBox;

/*! Circle shape object */
typedef struct tbCircle_s
{
	tbScalar radius;			/*< The circle radius */
}tbCircle;

/*! Shape object */
typedef struct tbShape_s
{
	unsigned int type;		/*!< Shape type */
	tbScalar inertia;		/*!< Inertia value */
	tbScalar friction;		/*!< Friction value */
	tbScalar restitution;	/*!< Restitution value */
}tbShape;

/*!
  \brief Creates a box shape
  \param[in] halfExtents The box half extents
  \return A shape object representing the box
*/
tbShape* tbCreateBox(tbVector halfExtents);
/*!
  \brief Creates a circle shape
  \param[in] radius The circle radius
  \return A shape object representing the circle
*/
tbShape* tbCreateCircle(tbScalar radius);
/*!
  \brief Returns the radius of a circle shape
  \param[in] shape The circle shape
  \return The radius of the circle shape, or an invalid scalar if the shape is not a circle.
*/
tbScalar tbGetRadius(const tbShape* shape);
/*!
  \brief Returns the half extents of a box shape
  \param[in] shape The box shape
  \param[in,out] halfExtents The vector where to store the half extents
*/
void tbGetHalfExtents(const tbShape* shape,tbVector halfExtents);
/*!
  \brief Computes the bounding box of a shape
  \param[in] shape The shape to use
  \param[in] rotation The rotation vector (ie the vector (cos(angle),sin(angle)))
  \param[out] min The minimum coordinates of the bounding box
  \param[out] max The maximum coordinates of the bounding box
*/
void tbBoundingBox(const tbShape* shape,const tbVector rotation,tbVector min,tbVector max);
/*!
  \brief Destroys the given shape
  \param[in] shape The shape to destroy
*/
void tbDestroyShape(tbShape* shape);

/*! @} */

/*! @} */

#endif
