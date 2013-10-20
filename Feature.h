//
//  Feature.h
//  CS440-HW3
//
//  Created by Wenzhao Xu on 11/23/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#ifndef __CS440_HW3__Feature__
#define __CS440_HW3__Feature__

#include <iostream>


class NumberDigit
{
public:
    int label;          //what the number is
    
    NumberDigit();
    
    long numbercount1[28][28];           // the number of each value at each pix
    double likelihood1[28][28];          // the likelihood of each value, 0 for, 1 for ,2 for
    
    long numbercount2[28][28];
    double likelihood2[28][28];
    
    long numbercount3[28][28];
    double likelihood3[28][28];
    
    float prior_prob;   //the prior probality
    long total_number;  //the number of this number in testing sample
    
    
};


#endif /* defined(__CS440_HW3__Feature__) */
