/*
 * TrainningNN.c
 *
 *  Created on: 30/07/2015
 *      Author: allan
 */

#include "TrainingNN.h"

#include <stdlib.h>
#include <time.h>

TrainingNNData *TrainingNNDataCreate(int num_inputs, SamplingRegion *region)
{
	TrainingNNData *tdata = (TrainingNNData *) malloc(sizeof(TrainingNNData));
	tdata->num_inputs = num_inputs;
	tdata->inputs = NULL;
	tdata->region = region;
	return tdata;
}

void TrainingNNDataSetInputValue(TrainingNNData *tdata, IplImage *in)
{
	int l = tdata->region->square_size;
	IplImage *small_image = SampleRegionGetImageRegion(in, tdata->region);
	uchar *vector = SampleRegionGenerateVector(small_image);
	tdata->inputs = vector;
	cvReleaseImage(&small_image);
}

void TrainingNNDataSetOutput(TrainingNNData *tdata, IplImage *out)
{
	int l = tdata->region->square_size;
	IplImage *small_image = SampleRegionGetImageRegion(out, tdata->region);
	uchar *vector = SampleRegionGenerateVector(small_image);
	tdata->outputs = *(vector + (l-1)/2);
	cvReleaseImage(&small_image);
}

void TrainingNNDataDestroy(TrainingNNData **tdata)
{
	if((*tdata)->inputs)
	{
		free((*tdata)->inputs);
		(*tdata)->inputs = NULL;
	}
	free((*tdata)->region);
	free(tdata);
	tdata = NULL;
}
