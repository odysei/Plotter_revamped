#ifndef ROOT_styles__styles_h
#define ROOT_styles__styles_h

#include "TStyle.h"

namespace styles
{

TStyle *Set_TDRM();
TStyle *Set_TDRM3();

void Set_TDRM2();

void Set_Matt(int gridFlagX, int gridFlagY, int fitStatFlag, int histStatFlag);
}

#endif