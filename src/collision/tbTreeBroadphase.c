#include "tbConfig.h"
#include "tbBroadphase.h"
#include "tbMemory.h"

static unsigned int tbDepthNode(tbNode*);
static void tbParseNode(tbNode*,const tbTree*,tbBroadphaseCallback,void*);

tbBroadphase* tbCreateBroadphase(void)
{
	tbBroadphase* broadphase;

	broadphase=tbAlloc(sizeof(tbBroadphase));
	tbMemorySet(broadphase,0,sizeof(tbBroadphase));

	broadphase->tree=tbAlloc(sizeof(tbTree));
	tbMemorySet(broadphase->tree,0,sizeof(tbTree));

	return broadphase;
}

void tbSetObjects(tbBroadphase* broadphase,tbScalar timeStep,tbObject* objects,unsigned int numObjects)
{
	tbTree* tree=broadphase->tree;
	unsigned int i;

	/* Here are the heuristics */
	broadphase->heuristic.maxObjects=30;
	broadphase->heuristic.minObjects=10;
	broadphase->heuristic.minRatio=0.3;
	broadphase->heuristic.maxRatio=3.0;
	broadphase->heuristic.maxDepth=12;
	broadphase->heuristic.minDepth=3;

	tree->objects=objects;
	if(tree->numObjects<numObjects)
	{
		tree->boundingBoxes=tbRealloc(tree->boundingBoxes,sizeof(tbObjectBox)*numObjects);
		tree->numObjects=numObjects;
	}

	/* Compute the bounding boxes of all the objects */
	for(i=0;i<numObjects;i++)
	{
		tbObjectBox* box=&tree->boundingBoxes[i];
		tbObject* object=&objects[i];
		tbState state=object->state;

		/* We must integrate the objects so that we have their right bounding boxes at time t0+timeStep */
		tbIntegrate(object,&state,timeStep);
		tbBoundingBox(object->shape,state.rotation,box->min,box->max);

		/* Note that we need to translate the transformed bounding box */
		box->min[0]+=state.position[0];
		box->min[1]+=state.position[1];
		box->max[0]+=state.position[0];
		box->max[1]+=state.position[1];
	}

	if(broadphase->tree->root)
	{
		for(i=0;i<numObjects;i++)
			tbPlaceObject(tree->root,i,tree);
	}
	else
	{
		/* The tree needs to be built */
		unsigned int i;

		tree->root=tbAlloc(sizeof(tbNode));
		tree->root->numObjects=numObjects;
		tree->root->objects=tbAlloc(sizeof(unsigned int)*numObjects);
		/* That's kind of stupid... But it makes the thing go faster */
		for(i=0;i<numObjects;i++)
			tree->root->objects[i]=i;

		tree->root->depth=0;
		tbBuildNode(tree->root,tree,&broadphase->heuristic);
	}
}

static unsigned int tbDepthNode(tbNode* node)
{
	unsigned int depth[2];

	if(node->children)
	{
		depth[0]=tbDepthNode(&node->children[0]);
		depth[1]=tbDepthNode(&node->children[1]);
		return ((depth[0]>depth[1])?depth[0]:depth[1]);
	}
	else
		return node->depth;
}

static void tbParseNode(tbNode* node,const tbTree* tree,tbBroadphaseCallback callback,void* user)
{
	if(node->children)
	{
		tbAssert((node->children[0].numObjects+node->children[1].numObjects)>=node->numObjects);

		/* Continue recursion if needed */
		if(node->numObjects>0)
		{
			tbParseNode(&node->children[0],tree,callback,user);
			tbParseNode(&node->children[1],tree,callback,user);
		}
	}
	else
	{
		unsigned int i,j;

		/* Test all the objects in this node */
		for(i=0;i<node->numObjects;i++)
		{
			const tbObjectBox* box1=&tree->boundingBoxes[node->objects[i]];
			tbObject* first=&tree->objects[node->objects[i]];
			int filter=first->flags&TB_OBJECT_STATIC;

			for(j=i+1;j<node->numObjects;j++)
			{
				const tbObjectBox* box2=&tree->boundingBoxes[node->objects[j]];
				tbObject* second=&tree->objects[node->objects[j]];

				/* Normally never happens */
				tbAssert(node->objects[j]!=node->objects[i]);

				/* Do not test static vs static objects */
				if(filter&&second->flags&TB_OBJECT_STATIC)
					continue;

				if(box1->min[0]<box2->max[0]&&box1->max[0]>box2->min[0]&&
					box1->min[1]<box2->max[1]&&box1->max[1]>box2->min[1])
					callback(user,first,second);
			}
		}
	}
}

void tbGetPairs(tbBroadphase* broadphase,tbBroadphaseCallback callback,void* user)
{
	tbTree* tree=broadphase->tree;
	broadphase->updatesCount++;

	/* Get pairs recursively */
	tbParseNode(tree->root,tree,callback,user);

	/* Modify the tree hierarchy if needed */
	if(broadphase->updatesCount>10)
	{
		tbUpdateNode(tree->root,tree,&broadphase->heuristic);
		broadphase->updatesCount=0;
	}

	/* Reset the object counters */
	tbResetNode(tree->root);
}

void tbDestroyBroadphase(tbBroadphase* broadphase)
{
	tbTree* tree=broadphase->tree;
	if(tree->boundingBoxes)
		tbFree(tree->boundingBoxes);
	if(tree->root)
	{
		tbDestroyNode(tree->root);
		tbFree(tree->root);
	}
	tbFree(tree);
	tbFree(broadphase);
}
