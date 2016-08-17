#ifndef helpers__ROOT_IO_cpp
#define helpers__ROOT_IO_cpp

#include "helpers/ROOT_IO.h"
#include "helpers/type_conversion.h"

void SaveC(TCanvas *c1, const string &name)
{
    c1->Update();
    char *nam = str2char(name);
    c1->SaveAs(nam);
    delete[] nam;
}

#endif
