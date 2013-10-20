//
//  main.cpp
//  CS440-HW3
//
//  Created by Wenzhao Xu on 11/23/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include "Feature.h"
#include "FeatureData.h"


using namespace std;

void outputResult(TestSample & Sample, TrainingSample & Sample2)
{
    
    ifstream fin;
    ofstream fout;
    ofstream fout2;
    ofstream fout3;
    ofstream fout4;
    ofstream fout5;

    
    string line;
    int line_no=0;
    
    fin.open("/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW3/CS440-HW3/digitdata/testimages",ios::in); 
    
    fout.open("/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW3/CS440-HW3/result.txt",ios::out);              //confusion matrix
    fout2.open("/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW3/CS440-HW3/result_heatmap.txt",ios::out);     //heatmap for each digit
    fout3.open("/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW3/CS440-HW3/result_protypical.txt",ios::out);  //prototypical image
    fout4.open("/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW3/CS440-HW3/result_ratio.txt",ios::out);       //odds ratio
    
    fout5.open("/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW3/CS440-HW3/result_wrong.txt",ios::out);
    
    //output the confusion matrix
    
    for (int i=0;i<10; i++) {
        for (int j=0; j<10; j++) {
            fout.width(8);
            fout<<setprecision(3)<<Sample.confusionmatrix[i][j];
        }
        fout<<endl;
    }
    
    //output the classficationratio and prototypical
    for (int k=0; k<10;k++) {
        fout<<k<<" "<<setw(8)<<setprecision(3)<<Sample.classficationratio[k]<<setw(8)
        <<Sample.NumProtyp[k]<<setw(8)<<(Sample.NumProtyp[k]-1)*28<<setw(8)<<Sample.Prob_protyp[k]<<endl;
    }
    
    //output the prototypical digit
    
    cout<<"output the digit";
    
    while (getline(fin, line))
    {
        ++line_no;
        
        for (int k=0; k<10; k++)
        {
            if (line_no==(Sample.NumProtyp[k]-1)*28+1)    //current line is the starting line of a protypical digit image.
            {
                fout3<<k<<line<<endl;
                cout<<k<<line<<endl;
                for (int j=0; j<27; j++)                //using 27 since we have already read one line of a digit
                {
                    getline(fin,line);
                    fout3<<k<<line<<endl;
                    cout<<k<<line<<endl;
                    ++line_no;
                }   
                cout<<endl;
                fout3<<endl;
            }
        }
    }
    
    //output the log likelihood
    
 //   fout2<<"log likelihood"<<endl;

    for (int k=0; k<10; k++)
    {
        for (int i=0; i<28; i++)
        {
            for (int j=0; j<28; j++)
            {
                fout2.width(10);
                fout2<<setprecision(4)<<log10(Sample2.Data[k].likelihood2[i][j]+Sample2.Data[k].likelihood3[i][j]);
            }
            fout2<<endl;
        }
        fout2<<endl;
    }
    
    //output the odds ratio
    
    for (int k=0; k<4; k++) {
        fout4<<Sample.kk[k][0]<<"  "<<Sample.kk[k][1]<<endl;
        
        for (int i=0; i<28; i++) {
            for (int j=0; j<28; j++) {
                fout4.width(10);
                fout4<<setprecision(3)<<Sample.ratio[k][i][j];
            }
            fout4<<endl;
        }
        
        fout4<<endl;
    }
    
    //output the wrong classification
    
    int tempp;
    
    fout5<<"#"<<"  "<<"Predict"<<"  "<<"Ground"<<endl;
    for (int i=0; i<Sample.wrongclassification.size(); i++) {
        tempp=Sample.wrongclassification[i];
        fout5<<(tempp-1)*28+1<<"   "<<Sample.PredictedResult[tempp-1]<<"  "<<Sample.GroundResult[tempp-1]<<endl;
    }

    fout.close();
    fout2.close();
    fout3.close();
    fout4.close();
    fout5.close();
    fin.close();
}


int main(int argc, const char * argv[])
{

    // insert code here...
    std::cout << "Hello, World>>>>!\n";
    
    TrainingSample Sample_Training;
    TestSample  Sample_Test;
    
    string file1;
    string file2;
    
    string file3;
    string file4;
    
    file1="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW3/CS440-HW3/digitdata/trainingimages";  //Training image file
    file2="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW3/CS440-HW3/digitdata/traininglabels";  //Training number file
    file3="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW3/CS440-HW3/digitdata/testimages";  //testing image file
    file4="/Users/Xuwenzhao/Documents/Developer/C++/CS440-HW3/CS440-HW3/digitdata/testlabels";  //testing number file
    
    //Trainging Data
    Sample_Training.Training(file1, file2);
    
    //Predicting
    Sample_Test.predicting(Sample_Training.Data, file3, file4);
    
    //Compare and evaluate result
    Sample_Test.compare(Sample_Training.Data);
    
    //output the result
    
    outputResult(Sample_Test, Sample_Training);
    
 
    return 0;
}

