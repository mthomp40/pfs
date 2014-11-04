//
//  Jacket.h
//  pfs
//
//  Created by Behzad Khalili on 1/11/2014.
//  Copyright (c) 2014 Behzad Khalili. All rights reserved.
//

#ifndef __pfs__Jacket__
#define __pfs__Jacket__

#include <iostream>
#include <string>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>

class Jacket
{
public:
    int M1;	//time in minutes for machine A
    int M2;	//time in minutes for machine B
    int M3;	//time in minutes for machine C

    /*Definition of constructors functions*/
    Jacket() {}
    
    /*Definition of constructors functions*/
    Jacket(int m1, int m2, int m3)
    {
        M1 = m1;
        M2 = m2;
        M3 = m3;
    }

};


#endif /* defined(__pfs__Jacket__) */
