// BoxBlurAlgorithm.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "BoxBlurAlgorithm.h"


// This is an example of an exported variable
BOXBLURALGORITHM_API int nBoxBlurAlgorithm=0;

// This is an example of an exported function.
BOXBLURALGORITHM_API int fnBoxBlurAlgorithm(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see BoxBlurAlgorithm.h for the class definition
CBoxBlurAlgorithm::CBoxBlurAlgorithm()
{
    return;
}
