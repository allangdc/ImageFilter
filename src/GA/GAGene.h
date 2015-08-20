/*
 * GAGene.h
 *
 *  Created on: 10 de ago de 2015
 *      Author: allan
 */

#ifndef GA_GAGENE_H_
#define GA_GAGENE_H_

typedef struct gene
{
	char *value;
	int sizetype;
} GAGene;

GAGene *GAGeneCreate();
void GAGeneDestroy(GAGene **gene);
void GAGeneSetValue(GAGene *gene, char *value, int sizetype);
void *GAGeneGetValue(GAGene *gene);
GAGene *GAGeneClone(GAGene *gene);

#endif /* GA_GAGENE_H_ */
