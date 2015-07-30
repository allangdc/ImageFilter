/*
 * TrainningNN.c
 *
 *  Created on: 30/07/2015
 *      Author: allan
 */

#include "TrainningNN.h"

#include <stdlib.h>
#include <time.h>

TrainningNN_data *TrainningDataCreate(int num_inputs)
{
	TrainningNN_data *tdata = (TrainningNN_data *) malloc(sizeof(TrainningNN_data));
	tdata->num_inputs = num_inputs;
	return tdata;
}
void TrainningDataSetInputValue(TrainningNN_data *tdata, SamplingRegion *region, IplImage *in)
{
	int l = region->square_size;
	IplImage *small_image = SampleRegionGetImageRegion(in, region);
	uchar *vector = SampleRegionGenerateVector(small_image);

}
void TrainningDataSetOutput(TrainningNN_data *tdata, SamplingRegion *region, IplImage *out);
void TrainningDataDestroy(TrainningNN_data **tdata);




TrainningNN *CreateTrainningBase(IplImage *image_in, IplImage *image_out, int sidesize, int num_sampling)
{
	int i;
	TrainningNN *base;
	base->image_in = image_in;
	base->image_out = image_out;
	base->num_sampling = num_sampling;
	base->image_vector_sample_in = (IplImage **) malloc(sizeof(IplImage *) * num_sampling);
	base->image_vector_sample_out = (IplImage **) malloc(sizeof(IplImage *) * num_sampling);

	for(i=0; i<num_sampling; i++)
	{
		srand(time(NULL));
		int pos_x = (rand() % (image_in->width - 2*sidesize)) + sidesize;
		int pos_y = (rand() % (image_in->height - 2*sidesize)) + sidesize;
		IplImage *img_in = cvCreateImage(cvSize(sidesize, sidesize), IPL_DEPTH_8U, image_in->nChannels);
		IplImage *img_out = cvCreateImage(cvSize(sidesize, sidesize), IPL_DEPTH_8U, image_in->nChannels);
		*(base->image_vector_sample_in + i) = img_in;
		*(base->image_vector_sample_out + i) = img_out;
		SamplingRegion sample;
		SamplingRegionCreateRegion(&sample);
		SampleRegionSetSquareSize(&sample, sidesize);
		SampleRegionSetPoint(&sample, pos_x, pos_y);
		SampleRegionGetImageRegion(image_in, &sample);
		SampleRegionGetImageRegion(image_out, &sample);
	}
	return base;
}
