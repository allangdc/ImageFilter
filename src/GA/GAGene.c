/*
 * GAGene.c
 *
 *  Created on: 10 de ago de 2015
 *      Author: allan
 */

#include "GAGene.h"
#include <stdlib.h>

GAGene *GAGeneCreate()
{
	GAGene *gene = (GAGene *) malloc(sizeof(GAGene));
	gene->value = NULL;
	gene->sizetype = 0;
	return gene;
}

void GAGeneDestroy(GAGene **gene)
{
	if(*gene)
	{
		GAGene *g = *gene;
		if(g->value)
		{
			free(g->value);
			g->value = NULL;
			g->sizetype = 0;
		}
		free(*gene);
		*gene = NULL;
	}
}

void GAGeneSetValue(GAGene *gene, char *value, int sizetype) {
	gene->value = value;
	gene->sizetype = sizetype;
}

void *GAGeneGetValue(GAGene *gene)
{
	return gene->value;
}

GAGene *GAGeneClone(GAGene *gene)
{
	int i;
	GAGene *g = (GAGene *) malloc(sizeof(GAGene));
	g->value = (char *) malloc(sizeof(char) * gene->sizetype);
	g->sizetype = gene->sizetype;
	for(i=0; i<gene->sizetype; i++)
		*(g->value + i) = *((char *) (gene->value + i));
	return g;
}

