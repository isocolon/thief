#include "thief.h"

void SMARTMOVE_Adjust (void)
    {
    if (User.bSmartMove)
        {
        if ((! User.bSmartMove1) && (! User.bSmartMove2))
            {
            User.bSmartMove1 = 1 ;
            User.bSmartMove2 = 1 ;
            }
        }
    }
