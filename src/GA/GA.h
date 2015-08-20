/*
 * GA.h
 *
 *  Created on: 10 de ago de 2015
 *      Author: allan
 */

#ifndef GA_H_
#define GA_H_

typedef struct cromossome
{
	GAGene **genes;
	int num_genes;
} GACromossome;

GACromossome *GACromossomeCreate();
void GACromossomeDestroy(GACromossome **cromossome);
void GACromossomeSetNumGenes(GACromossome *cromossome, int num_genes);
void GACromossomeAddGene(GACromossome *cromossome, int id, GAGene *gene);

#endif /* GA_H_ */
