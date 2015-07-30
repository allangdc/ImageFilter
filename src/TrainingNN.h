/*
 * TrainningNN.h
 *
 *  Created on: 29/07/2015
 *      Author: allan
 */

#ifndef TRAININGNN_H_
#define TRAININGNN_H_

#include <cv.h>

#include "sampling.h"

typedef struct trainingnn_data
{
	int num_inputs;
	uchar *inputs;
	uchar outputs;
} TrainingNNData;

TrainingNNData *TrainingNNDataCreate(int num_inputs);
void TrainingNNDataSetInputValue(TrainingNNData *tdata, SamplingRegion *region, IplImage *in);
void TrainingNNDataSetOutput(TrainingNNData *tdata, SamplingRegion *region, IplImage *out);
void TrainingNNDataDestroy(TrainingNNData **tdata);

typedef struct trainingnn
{
	TrainingNNData **trainings;
} TrainningNN;

#endif /* TRAININGNN_H_ */
