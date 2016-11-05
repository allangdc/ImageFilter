/*
 * TrainningNN.c
 *
 *  Created on: 30/07/2015
 *      Author: allan
 */

#include "TrainingNN.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

TrainingNNData *TrainingNNDataCreate(int num_inputs, SamplingRegion *region)
{
	TrainingNNData *tdata = (TrainingNNData *) malloc(sizeof(TrainingNNData));
	tdata->num_inputs = num_inputs;
	tdata->inputs = NULL;
	tdata->region = region;
	return tdata;
}

void TrainingNNDataSetData(TrainingNNData *tdata, ImageIO *imageio)
{
	int l = tdata->region->square_size;
	IplImage *small_image_in = SampleRegionGetImageRegion(tdata->region, imageio->in);
	IplImage *small_image_out = SampleRegionGetImageRegion(tdata->region, imageio->out);
	if(tdata->inputs)
	{
		free(tdata->inputs);
		tdata->inputs = NULL;
	}
	tdata->inputs = SampleRegionGenerateVector(small_image_in);
	uchar *vector = SampleRegionGenerateVector(small_image_out);
	tdata->outputs = *(vector + (l-1)/2);
	free(vector);
	cvReleaseImage(&small_image_in);
	cvReleaseImage(&small_image_out);
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

void TrainingNNDataPrint(TrainingNNData *tdata)
{
	int i;
	int square = sqrt(tdata->num_inputs);
	printf("| ");
	for(i=0; i<tdata->num_inputs; i++)
	{
		printf("%3d ", *(tdata->inputs));
		if(i%square==0)
		{
			if(i+1 < tdata->num_inputs)
				printf("|\n| ");
			else
				printf("|\n");
		}
	}
}

TrainingNN *TrainingNNCreate(int num_images, int sample_per_images)
{
	int i;
	TrainingNN *t = (TrainingNN *) malloc(sizeof(TrainingNN));
	t->num_images = num_images;
	t->num_sample_per_images = sample_per_images;
	t->img_in = (IplImage **) malloc(sizeof(IplImage *) * num_images);
	t->img_out = (IplImage **) malloc(sizeof(IplImage *) * num_images);
	t->trainings = (TrainingNNData **) malloc(sizeof(TrainingNNData *) * num_images * sample_per_images);
	for(i=0; i<num_images; i++)
	{
		int j;
		*(t->img_in + i) = NULL;
		*(t->img_out + i) = NULL;
		for(j=0; j<sample_per_images; j++)
		{
			*(t->trainings + j) = NULL;
		}
	}
	return t;
}

void TrainingNNDestroy(TrainingNN **trainning)
{
	int i, count;
	count=0;
	for(i=0; i<(*trainning)->num_images; i++)
	{
		int j;
		IplImage *img_in = *((*trainning)->img_in + i);
		IplImage *img_out = *((*trainning)->img_out + i);
		if(img_in)
		{
			free(img_in);
			*((*trainning)->img_in + i) = NULL;
		}
		if(img_out)
		{
			free(img_out);
			*((*trainning)->img_out + i) = NULL;
		}
		for(j=0; j<(*trainning)->num_sample_per_images; j++)
		{
			free( *((*trainning)->trainings + count) );
			*((*trainning)->trainings + count) = NULL;
			count++;
		}
	}
	free((*trainning)->trainings);
	(*trainning)->trainings = NULL;
	free((*trainning)->img_in);
	(*trainning)->img_in = NULL;
	free((*trainning)->img_out);
	(*trainning)->img_out = NULL;

	free(*trainning);
	*trainning = NULL;
}

