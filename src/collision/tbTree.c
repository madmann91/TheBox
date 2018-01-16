#include "tbConfig.h"
#include "tbMemory.h"
#include "tbTree.h"

unsigned int tbBuildNode(tbNode* node,const tbTree* tree,const tbHeuristic* heuristic)
{
	unsigned int depth[2];
	tbNode* left,*right;
	unsigned int i;
	tbScalar ratio;

	/* Exit if the tree is too deep */
	if(node->depth>=heuristic->maxDepth)
	{
		node->children=0;
		return node->depth;
	}

	/* Find the variance and middle point for this node */
	tbZero(node->middle);
	tbZero(node->variance);
	for(i=0;i<node->numObjects;i++)
	{
		const unsigned int j=node->objects[i];
		node->middle[0]+=tree->boundingBoxes[j].min[0]+tree->boundingBoxes[j].max[0];
		node->middle[1]+=tree->boundingBoxes[j].min[1]+tree->boundingBoxes[j].max[1];

		node->variance[0]+=tbAbs(tree->boundingBoxes[j].min[0])+tbAbs(tree->boundingBoxes[j].max[0]);
		node->variance[1]+=tbAbs(tree->boundingBoxes[j].min[1])+tbAbs(tree->boundingBoxes[j].max[1]);
	}

	/* Find the splitting axis accordingly */
	node->axis=(node->variance[0]<node->variance[1]);
	node->split=node->middle[node->axis]/(2*node->numObjects);

	/* Allocate memory for the children */
	node->children=tbAlloc(sizeof(tbNode)*2);
	left=&node->children[0];
	right=&node->children[1];

	left->numObjects=right->numObjects=0;
	left->depth=right->depth=node->depth+1;
	left->objects=tbAlloc(sizeof(unsigned int)*node->numObjects);
	right->objects=tbAlloc(sizeof(unsigned int)*node->numObjects);

	/* Split this node in two parts */
	for(i=0;i<node->numObjects;i++)
	{
		const unsigned int j=node->objects[i];

		/* Determine on which side is this bounding box */
		if(tree->boundingBoxes[j].max[node->axis]>=node->split)
		{
			right->objects[right->numObjects]=
			right->numObjects++;
		}

		if(tree->boundingBoxes[j].min[node->axis]<=node->split)
		{
			left->objects[left->numObjects]=
			left->numObjects++;
		}
	}

	/* Test if the split was useful */
	ratio=(tbScalar)left->numObjects/(tbScalar)right->numObjects;
	if(ratio>heuristic->maxRatio||ratio<heuristic->minRatio
		||left->numObjects<heuristic->minObjects
		||right->numObjects<heuristic->minObjects)
	{
		tbFree(left->objects);
		tbFree(right->objects);
		tbFree(node->children);
		node->children=0;
		return node->depth;
	}

	/* Resize both arrays */
	left->objects=tbRealloc(left->objects,sizeof(unsigned int)*left->numObjects);
	left->maxObjects=left->numObjects;

	right->objects=tbRealloc(right->objects,sizeof(unsigned int)*right->numObjects);
	right->maxObjects=right->numObjects;

	/* Destroy our array */
	tbFree(node->objects);
	node->maxObjects=0;
	node->objects=0;

	/* Continue recursion */
	depth[0]=tbBuildNode(left,tree,heuristic);
	depth[1]=tbBuildNode(right,tree,heuristic);
	if(depth[0]>depth[1])
		return depth[0];
	return depth[1];
}

void tbPlaceObject(tbNode* node,unsigned int object,const tbTree* tree)
{
	/* Update variance and middle point */
	node->middle[0]+=tree->boundingBoxes[object].min[0]+tree->boundingBoxes[object].max[0];
	node->middle[1]+=tree->boundingBoxes[object].min[1]+tree->boundingBoxes[object].max[1];
	node->variance[0]+=tbAbs(tree->boundingBoxes[object].min[0])+tbAbs(tree->boundingBoxes[object].max[0]);
	node->variance[1]+=tbAbs(tree->boundingBoxes[object].min[1])+tbAbs(tree->boundingBoxes[object].max[1]);

	if(node->children)
	{
		tbNode* left=&node->children[0];
		tbNode* right=&node->children[1];

		node->numObjects++;

		/* Place the object in the good leaf */
		if(tree->boundingBoxes[object].min[node->axis]<=node->split)
			tbPlaceObject(left,object,tree);
		if(tree->boundingBoxes[object].max[node->axis]>=node->split)
			tbPlaceObject(right,object,tree);
	}
	else
	{
		/* Resize the object array if needed (it normally doesn't happen very often, if you use preallocation) */
		if(node->numObjects+1>=node->maxObjects)
		{
			node->maxObjects=node->numObjects+4;
			node->objects=tbRealloc(node->objects,sizeof(unsigned int)*node->maxObjects);
		}
		node->objects[node->numObjects]=object;
		node->numObjects++;
	}
}

void tbUpdateNode(tbNode* node,const tbTree* tree,const tbHeuristic* heuristic)
{
	if(node->children)
	{
		const tbScalar ratio=(tbScalar)node->children[0].numObjects/(tbScalar)node->children[1].numObjects;

		/* Destroy useless nodes */
		if((node->numObjects>heuristic->minObjects||node->depth>heuristic->minDepth)&&
			(ratio>heuristic->maxRatio||ratio<heuristic->minRatio))
		{
			tbDestroyNode(node);
			node->maxObjects=0;
			node->objects=0;
			return;
		}

		/* Change splitting axis according to variance and middle point */
		node->axis=(node->variance[0]<node->variance[1]);
		node->split=node->middle[node->axis]/(2*node->numObjects);

		/* Continue and rearrange the children if we didn't change the tree from this node */
		tbUpdateNode(&node->children[0],tree,heuristic);
		tbUpdateNode(&node->children[1],tree,heuristic);
	}
	else
	{
		/* Build new nodes if needed */
		if(node->numObjects>heuristic->maxObjects)
			tbBuildNode(node,tree,heuristic);
	}
}

void tbResetNode(tbNode* node)
{
	tbZero(node->variance);
	tbZero(node->middle);
	node->numObjects=0;

	if(node->children)
	{
		tbResetNode(&node->children[0]);
		tbResetNode(&node->children[1]);
	}
}

void tbDestroyNode(tbNode* node)
{
	if(node->children)
	{
		tbNode* left=&node->children[0];
		tbNode* right=&node->children[1];

		tbDestroyNode(left);
		tbDestroyNode(right);
		tbFree(node->children);
		node->children=0;
	}
	else
	{
		/* We simply release the objects array, but be careful, this node is now invalid */
		if(node->objects)
		{
			tbFree(node->objects);
			node->objects=0;
		}
		node->maxObjects=0;
	}
}
