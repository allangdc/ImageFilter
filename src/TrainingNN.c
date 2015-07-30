/*
 * TrainningNN.c
 *
 *  Created on: 30/07/2015
 *      Author: allan
 */

#include "TrainingNN.h"

#include <stdlib.h>
#include <time.h>

TrainingNNData *TrainingNNDataCreate(int num_inputs)
{
	TrainingNNData *tdata = (TrainingNNData *) malloc(sizeof(TrainingNNData));
	tdata->num_inputs = num_inputs;
	tdata->inputs = NULL;
	return tdata;
}

void TrainingNNDataSetInputValue(TrainingNNData *tdata, SamplingRegion *region, IplImage *in)
{
	int l = region->square_size;
	IplImage *small_image = SampleRegionGetImageRegion(in, region);
	uchar *vector = SampleRegionGenerateVector(small_image);
	tdata->inputs = vector;
	cvReleaseImage(&small_image);
}

void TrainingNNDataSetOutput(TrainingNNData *tdata, SamplingRegion *region, IplImage *out)
{
	int l = region->square_size;
	IplImage *small_image = SampleRegionGetImageRegion(out, region);
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
	free(tdata);
	tdata = NULL;
}
