#ifndef AGraph__extra_cpp
#define AGraph__extra_cpp

#include "AGraph/extra.h"

// Additional programs required for understanding options
string ExtractSentence(const string &input)
{
    string word;
    unsigned long counter = 0;

    while (input[counter] != '\0' && input[counter] != '\n') {
        if (counter != 0) {
            if (input[counter - 1] == ' ' && input[counter] == ' ')
                return word;
            word += input[counter - 1];
        }
        counter++;
    }
    if (counter != 0 && input[counter - 1] != ' ')
        word += input[counter - 1];

    return word;
}

void ExtractOptionAndValue(const string &input, string &option, string &value)
{
    int fuse = 0;
    unsigned long counter = 0;

    while (input[counter] != '\0' && input[counter] != '\n' &&
           input[counter] != '=') {
        option += input[counter];
        counter++;
    }
    if (input[counter] == '=') {
        option += input[counter];
        counter += 2;
        fuse = 1;
    }

    while (input[counter - 1] != '\0' && input[counter - 1] != '\n' &&
           input[counter] != '\0' && input[counter] != '\n') {
        if (fuse != 0) {
            if (input[counter - 1] == ' ' && input[counter] == ' ')
                fuse = 0;
            else
                value += input[counter - 1];
        }
        counter++;
    }
    if (fuse != 0 && input[counter - 1] != ' ' && input[counter - 1] != '=' &&
        input[counter - 1] != '\0' && input[counter - 1] != '\n')
        value += input[counter - 1];
}

/*Work in progress
void AutoLegend(vector<double> vDataX, vector<double> vDataZ, double *RangeX,
double *RangeZ, double *point1, double *point2)
{
    double LengthX=RangeX[1]-RangeX[0], LengthZ=RangeZ[1]-RangeZ[0];
    double Area=LengthX*LengthZ, Ratio=LengthX/LengthZ;

    if( vDataX.size()!= vDataZ.size() )
    {
        cout << "\nError. Vector sizes mismatch. Exiting.\n"; return;
    }
    else unsigned long Size=vDataX.size();

    double Analysis[8][Size]; //Area UL, ratio UL, area UR, ratio UR, area DL
ratio DL, area DR, ratio DR

    unsigned int counterL, counterR;
    double value, valueL, areaL, ratioL, valueR, areaR, ratioR;

    for( unsigned int counter=0; counter<Size; counter++ )
    {
        value=vDataZ[counter];


        //// Up area ////
        //Left block
        counterL=counter;
        valueL=vDataZ[counterL];
        while( valueL < value && counterL!=0)
        {
            counterL--;
            valueL=vDataZ[counterL];
        }
        ratioL=( vDataX[counter]-vDataX[counterL] )/(
vDataZ[counter]-vDataZ[counterL] )/Ratio;
        areaL=( vDataX[counter]-vDataX[counterL] )*(
vDataZ[counter]-vDataZ[counterL] )/Area;

        //Right block
        counterR=counter;
        valueR=vDataZ[counterR];
        while( valueR < value && counterR!=Size-1)
        {
            counterR++;
            valueR=vDataZ[counterR];
        }
        ratioR=( vDataX[counterR]-vDataX[counter] )/(
vDataZ[counterR]-vDataZ[counter] )/Ratio;
        areaR=( vDataX[counterR]-vDataX[counter] )*(
vDataZ[counterR]-vDataZ[counter] )/Area;

        Analysis[0][counter]=areaL;
        Analysis[1][counter]=areaR;
        Analysis[2][counter]=ratioL;
        Analysis[3][counter]=ratioR;


        //// Down area ////
        //Will come
    }

    for( unsigned int counter=0; counter<Size; counter++ )
    {


    }


}
*/

#endif
