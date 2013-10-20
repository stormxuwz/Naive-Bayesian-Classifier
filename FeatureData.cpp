//
//  FeatureData.cpp
//  CS440-HW3
//
//  Created by Wenzhao Xu on 11/23/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#include <vector>
#include <math.h>
#include <fstream>
#include "FeatureData.h"

using namespace std;


void TrainingSample::Training(string SampleFileName1, string SampleFileName2)
{
    ifstream file1;         //image file
    ifstream file2;         //label file
    int numberlabel=-1;
    
    char s;
    
    //set Laplace smoothing
    
    smoothing_k=1;
    smoothing_V=3;
    
    file1.open(SampleFileName1.c_str(),ios::in);
    file2.open(SampleFileName2.c_str(),ios::in);

    
    if (!file1 || !file2) {
        cout<<"No File Found"<<endl;
        exit(0);
    }
    
    samplenumber=0;
    
    //read the data
    
    while(file2>>numberlabel)
    {
        
        samplenumber+=1;
        
    /*    // for debug
        if(samplenumber==5000)
        {
            cout<<"hi";
        }
     */    
        Data[numberlabel].total_number+=1;
        
        
        for (int i=0; i<28; i++)
        {
            for(int j=0;j<29;j++)   //using 29 to consider the '/n'
            {
                s=file1.get();
                switch (s)
                {
                    case ' ':
                        Data[numberlabel].numbercount1[i][j]+=1;        //background feature
                        break;
                    case '+':
                        Data[numberlabel].numbercount2[i][j]+=1;        //gray feature
                        break;
                    case '#':
                        Data[numberlabel].numbercount3[i][j]+=1;        //black feature
                        break;
                    default:
                   //     cout<<"end of line"<<endl;
                        break;
                }
            }
        }
        
    };
    
    
    //calculate the likelihood
    for (int k=0; k<10; k++)
    {
        for (int i=0; i<28; i++)
        {
            for(int j=0;j<28;j++)
            {
                Data[k].likelihood1[i][j]=double(Data[k].numbercount1[i][j]+smoothing_k)/double(Data[k].total_number+smoothing_k*smoothing_V);
                Data[k].likelihood2[i][j]=double(Data[k].numbercount2[i][j]+smoothing_k)/double(Data[k].total_number+smoothing_k*smoothing_V);
                Data[k].likelihood3[i][j]=double(Data[k].numbercount3[i][j]+smoothing_k)/double(Data[k].total_number+smoothing_k*smoothing_V);          
            }
        }
        
        Data[k].prior_prob=double(Data[k].total_number)/double(samplenumber);
    }
 
    file1.close();
    file2.close();
}

TrainingSample::TrainingSample()        //construct function
{
    NumberDigit a;
    
    for (int k=0; k<10; k++)
    {
        a.label=k;
        for (int i=0; i<28; i++)
        {
            for(int j=0;j<28;j++)
            {
                a.numbercount1[i][j]=0;
                a.numbercount2[i][j]=0;
                a.numbercount3[i][j]=0;
            }
        }
        Data.push_back(a);              //storing 10 digit number
    }
}



void TestSample::predicting(vector<NumberDigit> & TrainedSet,string SampleFileName1, string SampleFileName2)
{
    ifstream file1;     //image file
    ifstream file2;     //label file
    char s;
    int numberlabel=-1;
    double prob;    
    int max_num;            //the lable of the highest post_prob
    double post_prob[10];   //the post_prob to a certain image. 
    double PE;               //P(E)
    
    double temp;            //for debug
    
    // open the file
    file1.open(SampleFileName1.c_str(),ios::in);
    file2.open(SampleFileName2.c_str(),ios::in);
    
    //read the data
    while (file2>>numberlabel)
    {
        for (int k=0; k<10; k++) {
            post_prob[k]=0;         //reinitialize the post_prob;
        }

        GroundResult.push_back(numberlabel);
        
        for (int i=0; i<28; i++)
        {
            for(int j=0;j<29;j++)       //using 29 to consider the '/n' at the end of each line
            {
                s=file1.get();
                
                for (int k=0; k<10;k++)
                {
                    //find the post prob for each possible digit
                    switch (s)
                    {
                        case ' ':
                            prob=TrainedSet[k].likelihood1[i][j];
                            break;
                        case '+':
                            prob=TrainedSet[k].likelihood2[i][j];
                            break;
                        case '#':
                            prob=TrainedSet[k].likelihood3[i][j];
                            break;
                        default:
                            prob=1;         //for end of line, set prob=1 so that it doesn't impact the post_prob
                            break;
                    }
                    
                    post_prob[k]+=log10(prob);
                }
            }
        }
        
        //using ML to predict
        max_num=0;
        for (int k=0;k<10;k++)
        {
            if (post_prob[k]>post_prob[max_num])
            {
                max_num=k;
            }
        }
        PredictedResult_ML.push_back(max_num);
        
        
        // add the prior probability
        for (int k=0; k<10; k++)
            post_prob[k]+=log10(TrainedSet[k].prior_prob);
        
        
        // using MAP to predict
        
        
        max_num=0;
        for (int k=0;k<10;k++)
        {
            if (post_prob[k]>post_prob[max_num])
            {
                max_num=k;
            }
        }
        
        PredictedResult.push_back(max_num);
        
        PE=0.0;
        
        //calculate the post probability consider the P(E)
        for (int k=0; k<10;k++) {
            PE+=post_prob[k];
        }
        
        temp=pow(10.0,post_prob[max_num]);
        PredictedResult_prob.push_back(post_prob[max_num]/PE);
        
    };
    
        
    file1.close();
    file2.close();
}


void TestSample::compare(vector<NumberDigit> & Data)
{
//    double odds_c[10][10];

    double totaldigitnumber[10];        //the total number of a digit in a the ground set
    int temp1,temp2;
    double temp_1,temp_2;
    
    //calculation of the total number of each prediction for a certain number
    for (int i=0; i<GroundResult.size(); i++)
    {
        
        digitcount[GroundResult[i]][PredictedResult[i]]+=1;
        totaldigitnumber[GroundResult[i]]+=1;
        
        //find the prototypical digit image
        if (GroundResult[i]==PredictedResult[i])    // if the classification is right
        {
            
            temp_1=PredictedResult_prob[i];          // the post prob of this digit
            temp_2=Prob_protyp[GroundResult[i]];
            
            if (temp_1<temp_2) {
                Prob_protyp[GroundResult[i]]=temp_1;
                NumProtyp[GroundResult[i]]=i+1;
            }
        }
        
        else                                        // then it is a wrong classification
            wrongclassification.push_back(i+1);

    }
    
    //calculation of the confusion matrix. the diagonal is the classfication rate.

    for (int i=0; i<10; i++)
    {
        for (int j=0;j<10; j++)
        {
            confusionmatrix[i][j]=double(digitcount[i][j])/double(totaldigitnumber[i]);
        }
    }
    
    for (int i=0; i<10; i++) {
        classficationratio[i]=confusionmatrix[i][i];
    }
    
    
    //calculation of the odds ratios
    
    //From the confusion matrix, the pair should be 4--9, 5--3,  7--9,  8--3 kk[*][0] and kk[*][1] are the pair digit
    
    for (int i=1; i<28;i++) {
        for (int j=1; j<28; j++) {
            for (int k=0; k<4; k++) {
                temp1=kk[k][0];
                temp2=kk[k][1];
                ratio[k][i][j]=log10(Data[temp1].likelihood2[i][j]+Data[temp1].likelihood3[i][j])-log10(Data[temp2].likelihood2[i][j]+Data[temp2].likelihood3[i][j]);
            }
        }
    }
    
    cout<<"good result!"<<endl;
    
}


TestSample::TestSample()
{
    for(int i=0;i<10;i++)
    {
        for (int j=0; j<10; j++) {
            digitcount[i][j]=0;
            confusionmatrix[i][j]=0;
        }
        
        NumProtyp[i]=-1;
        Prob_protyp[i]=1;
    }
}


