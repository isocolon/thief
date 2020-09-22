#include "thief.h"

void TRUEPREMOVE_Adjust(void)
{
	switch(User.nTruePremove)
	{
		case PREMOVE_NONE :
			User.bTruePremove = 0;
			break;

		case PREMOVE_THIEF :
			User.bTruePremove = 1;
			break;

		case PREMOVE_LR :
			User.bTruePremove = 1;
			break;

		default :
			User.bTruePremove = 0;
			User.nTruePremove = PREMOVE_NONE;
			break;
	}
}
