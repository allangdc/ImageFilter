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
	SamplingRegion *region;
} TrainingNNData;

TrainingNNData *TrainingNNDataCreate(int num_inputs, SamplingRegion *region);
void TrainingNNDataSetInputValue(TrainingNNData *tdata, IplImage *in);
void TrainingNNDataSetOutput(TrainingNNData *tdata, IplImage *out);
void TrainingNNDataDestroy(TrainingNNData **tdata);

typedef struct trainingnn
{
	int num_images;
	IplImage **img_in, **img_out;
	int num_image_by_sample;
	TrainingNNData **trainings;
} TrainingNN;

TrainingNN *TrainingNNCreate();

#endif /* TRAININGNN_H_ */
