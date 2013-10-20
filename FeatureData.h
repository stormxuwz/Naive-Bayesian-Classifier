//
//  FeatureData.h
//  CS440-HW3
//
//  Created by Wenzhao Xu on 11/23/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#ifndef __CS440_HW3__FeatureData__
#define __CS440_HW3__FeatureData__

#include <iostream>
#include <vector>
#include <string>
#include "Feature.h"

using namespace std;


class TrainingSample
{
public:
    int array[28][28];

    vector <NumberDigit> Data;            //the vector to store the data
    void Training(string,string);

    int samplenumber;
    float smoothing_k,smoothing_V;
    
    TrainingSample();
    
};


class TestSample
{
public:
    int array[28][28];
    vector<int> GroundResult;           //the vector to store ground result
    vector<int> PredictedResult;        //the vector to store predicted result using MAP
    vector<int> PredictedResult_ML;     //the vector to store predicted result using ML
    vector<double> PredictedResult_prob; //the vector to store post_prob about the result
    vector<int> wrongclassification;
    vector<double> RealPost;            //the real post prob about the prediction. 

    double digitcount[10][10];          //
    double confusionmatrix[10][10];     //
    double classficationratio[10];
    
    int NumProtyp[10];               //the array to store the line number of the prototypical digit
    float Prob_protyp[10];
    

    void predicting(vector<NumberDigit> &, string, string);
    void compare(vector<NumberDigit> &);
    
    //kk represents the highest four values determined from confusion matrix.
    int kk[4][2]={{4,9},{5,3},{7,9},{8,3}};
    double ratio[3][28][28];
    
    /*
    int k1[2]={4,9};
    int k2[2]={5,3};
    int k3[3]={7,9};
    int k4[4]={8,3};
    */
    
    double ratios1[28][28];
    double ratios2[28][28];
    double ratios3[28][28];
    double ratios4[28][28];
    
    TestSample();
};

#endif /* defined(__CS440_HW3__FeatureData__) */