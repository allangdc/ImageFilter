/*
 * TrainningNN.h
 *
 *  Created on: 29/07/2015
 *      Author: allan
 */

#ifndef TRAINNINGNN_H_
#define TRAINNINGNN_H_

#include <cv.h>

#include "sampling.h"

typedef struct trainningnn_data
{
	int num_inputs;
	uchar *inputs;
	uchar outputs;
} TrainningNN_data;

TrainningNN_data *TrainningDataCreate(int num_inputs);
void TrainningDataSetInputValue(TrainningNN_data *tdata, SamplingRegion *region, IplImage *in);
void TrainningDataSetOutput(TrainningNN_data *tdata, SamplingRegion *region, IplImage *out);
void TrainningDataDestroy(TrainningNN_data **tdata);

typedef struct trainningnn
{
	IplImage *image_in, *image_out;
	int sidesize;
	int num_sampling;
	IplImage **image_vector_sample_in, **image_vector_sample_out;
} TrainningNN;

TrainningNN *CreateTrainningBase(IplImage *image_in, IplImage *image_out, int sidesize, int num_sampling);

#endif /* TRAINNINGNN_H_ */
