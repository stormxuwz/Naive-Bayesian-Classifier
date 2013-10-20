//
//  Feature.cpp
//  CS440-HW3
//
//  Created by Wenzhao Xu on 11/23/12.
//  Copyright (c) 2012 Xu Wenzhao. All rights reserved.
//

#include "Feature.h"


NumberDigit::NumberDigit()
{
    for (int i=0; i<28; i++)
    {
        for (int j=0; j<28; j++)
        {
            numbercount1[i][j]=0;
            numbercount2[i][j]=0;
            numbercount3[i][j]=0;
            
            likelihood1[i][j]=0;
            likelihood2[i][j]=0;
            likelihood3[i][j]=0;
            
        }
    }
    
    prior_prob=0;
    total_number=0;

}