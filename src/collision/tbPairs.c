#include "tbPairs.h"
#include "tbMemory.h"

static unsigned int tbHashPair(const tbPairTable* table,const tbPair* pair)
{
	/* A simple hash function, works better with prime table sizes */
	return ((pair->first<<2)+(pair->second<<1))%table->tableSize;
}

static void tbGrowPairs(tbPairTable* table)
{
	table->maxPairs=(unsigned int)((tbScalar)table->maxPairs*TB_PAIRS_GROW)+1;
	table->indices=tbRealloc(table->indices,sizeof(tbIndexList)*table->maxPairs);
	table->pairs=tbRealloc(table->pairs,sizeof(tbPair)*table->maxPairs);
}

tbPairTable* tbCreatePairs(unsigned int numEntries)
{
	tbPairTable* table=tbAlloc(sizeof(tbPairTable));

	tbMemorySet(table,0,sizeof(tbPairTable));
	table->tableSize=numEntries;
	table->indexTable=tbAlloc(sizeof(int)*numEntries);
	tbResetPairs(table);

	return table;
}

void tbAddPair(tbPairTable* table,const tbPair* pair)
{
	const unsigned int hash=tbHashPair(table,pair);
	const int position=table->indexTable[hash];
	tbIndexList* index;

	/* Is there already a pair in this position ? */
	if(position>=0)
	{
		int next=position,current;
		tbIndexList* list;
		
		next=position;
		do
		{
			current=next;
			list=&table->indices[current];

			/* Test if the pair has already been added */
			if(table->pairs[list->index].first==pair->first&&
				table->pairs[list->index].second==pair->second)
				return;

			next=list->next;
		}
		while(next>=0);

		/* Here we can safely add this pair */
		if(table->numPairs>=table->maxPairs)
			tbGrowPairs(table);

		/* Prepare the new index */
		index=&table->indices[table->numPairs];
		index->index=table->numPairs;
		index->next=-1;

		/* Add this pair */
		table->pairs[table->numPairs]=*pair;
		table->indices[current].next=table->numPairs;
		table->numPairs++;
	}
	else
	{
		/* There's no pair here, we can add it without any check */
		if(table->numPairs>=table->maxPairs)
			tbGrowPairs(table);

		/* Prepare the new index */
		index=&table->indices[table->numPairs];
		index->index=table->numPairs;
		index->next=-1;

		/* Add this pair */
		table->pairs[table->numPairs]=*pair;
		table->indexTable[hash]=table->numPairs;
		table->numPairs++;
	}
}

int tbGetPair(const tbPairTable* table,const tbPair* pair)
{
	const unsigned int hash=tbHashPair(table,pair);
	const int position=table->indexTable[hash];

	/* Is there a pair here ? */
	if(position>=0)
	{
		tbIndexList* list=&table->indices[position];

		while(1)
		{
			/* Is this the right pair ? */
			if(table->pairs[list->index].first==pair->first&&
				table->pairs[list->index].second==pair->second)
				return list->index;

			if(list->next>0)
				list=&table->indices[list->next];
			else
				break;
		}
	}

	return -1;
}

void tbResetPairs(tbPairTable* table)
{
	unsigned int i;
	for(i=0;i<table->tableSize;i++)
		table->indexTable[i]=-1;
	table->numPairs=0;
}

void tbDestroyPairs(tbPairTable* table)
{
	if(table->indices)
		tbFree(table->indices);
	if(table->pairs)
		tbFree(table->pairs);
	tbFree(table->indexTable);
	tbFree(table);
}
