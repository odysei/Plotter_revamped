#ifndef ROOT_styles__mattstyle_cpp
#define ROOT_styles__mattstyle_cpp

#include "ROOT_styles/styles.h"

// ROOT macro remnants

namespace styles
{

void Set_Matt(int gridFlagX, int gridFlagY, int fitStatFlag, int histStatFlag) 
{

  TStyle *myStyle = new TStyle("Matt Style","Matt Style");

  // For the canvas:
  myStyle->SetTitleFillColor(10);
  myStyle->SetCanvasBorderMode(0);
  myStyle->SetCanvasColor(10);
  myStyle->SetCanvasDefH(600);   //Height of canvas
  myStyle->SetCanvasDefW(800);    //Width of canvas
  //myStyle->SetCanvasDefX(0);      //Position on screen
  //myStyle->SetCanvasDefY(0);

  // For the Pad:
  //myStyle->SetPadBorderMode(0);
  //myStyle->SetPadBorderSize(Width_t size = 1);
  myStyle->SetPadColor(kWhite);
  myStyle->SetPadGridX(gridFlagX);
  myStyle->SetPadGridY(gridFlagY);
  //myStyle->SetGridColor(0);
  //myStyle->SetGridStyle(3);
  //myStyle->SetGridWidth(1);
  //myStyle->SetFrameLineColor(1);

  // For the frame:
  //myStyle->SetFrameBorderMode(0);
  //myStyle->SetFrameBorderSize(1);
  //myStyle->SetFrameFillColor(0);
  //myStyle->SetFrameFillStyle(0);
  //myStyle->SetFrameLineColor(0);
  //myStyle->SetFrameLineStyle(1);
  //myStyle->SetFrameLineWidth(1);

  // For the histo:
  myStyle->SetMarkerStyle(20);
  myStyle->SetHistFillColor(kWhite);
  //myStyle->SetHistFillStyle(0);
  //myStyle->SetHistLineColor(1);
  //myStyle->SetHistLineStyle(0);
  //myStyle->SetHistLineWidth(1);
  //myStyle->SetLegoInnerR(Float_t rad = 0.5);
  //myStyle->SetNumberContours(Int_t number = 20);

  //myStyle->SetEndErrorSize(2);
  //myStyle->SetErrorMarker(20);
  //myStyle->SetErrorX(0);

 
  // For the fit/function:
  //myStyle->SetFitFormat("5.4g");
  myStyle->SetFuncColor(2);
  myStyle->SetFuncStyle(1);
  //myStyle->SetFuncWidth(1);
  myStyle->SetOptFit(fitStatFlag);

	    /*
		The parameter mode can be = pcev  (default = 0111)
		p = 1;  print Probability
		c = 1;  print Chisquare/Number of degress of freedom
		e = 1;  print errors (if e=1, v must be 1)
		v = 1;  print name/values of parameters
		Example: gStyle->SetOptFit(1011);
		print fit probability, parameter names/values and errors.
		When "v"=1 is specified, only the non-fixed parameters are shown.
		When "v"=2 all parameters are shown.
		Note: gStyle->SetOptFit(1) means "default value", so it is equivalent to
		gStyle->SetOptFit(111)
	    */
		 
  
  // For the date:
  //myStyle->SetOptDate(0);
  //myStyle->SetDateX(Float_t x = 0.01);
  //myStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  //myStyle->SetOptFile(0);  
  myStyle->SetStatColor(kWhite);
  myStyle->SetStatFont(42);
  //myStyle->SetStatFontSize(0.1);//0.045
  //myStyle->SetStatTextColor(1);
  //myStyle->SetStatFormat("6.4g");
  //myStyle->SetStatBorderSize(1);
  myStyle->SetStatH(0.19);
  myStyle->SetStatW(0.13);
  // myStyle->SetStatStyle(Style_t style = 1001);
  // myStyle->SetStatX(Float_t x = 0);
  // myStyle->SetStatY(Float_t y = 0);
  myStyle->SetOptStat(histStatFlag); // To display the mean and RMS: SetOptStat("mr");
	
		 /*
		 The parameter mode can be = ksiourmen  (default = 000001111)
		 k = 1;  kurtosis printed
		 k = 2;  kurtosis and kurtosis error printed
		 s = 1;  skewness printed
		 s = 2;  skewness and skewness error printed
		 i = 1;  integral of bins printed
		 o = 1;  number of overflows printed
		 u = 1;  number of underflows printed
		 r = 1;  rms printed
		 r = 2;  rms and rms error printed
		 m = 1;  mean value printed
		 m = 2;  mean and mean error values printed
		 e = 1;  number of entries printed
		 n = 1;  name of histogram is printed
		 Example: gStyle->SetOptStat(11);
		 print only name of histogram and number of entries.
		 gStyle->SetOptStat(1101);  displays the name of histogram, mean value and RMS.
		 WARNING: never call SetOptStat(000111); but SetOptStat(1111), 0001111 will be taken as an octal number !!
		 WARNING: SetOptStat(1) is taken as SetOptStat(1111) (for back compatibility
		 with older versions. If you want to print only the name of the histogram call SetOptStat(1000000001).
		 NOTE that in case of 2-D histograms, when selecting just underflow (10000) or overflow (100000), the stats 
		 box will show all combinations of underflow/overflows and not just one single number!
		 */
	 
	 
  // Margins:
  myStyle->SetPadTopMargin(0.125);//(0.15); //0.05); // DA: changed 
  //myStyle->SetPadBottomMargin(0.13);
  myStyle->SetPadLeftMargin(0.18);
  //myStyle->SetPadRightMargin(0.05);//(0.15);//0.02); // DA: changed

  // For the Global title:
  //myStyle->SetOptTitle(1);
  myStyle->SetTitleFont(42);
  myStyle->SetTitleFillColor(kWhite);
  //myStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  //myStyle->SetTitleFontSize(0.05);
  //myStyle->SetTitleH(0); // Set the height of the title box
  //myStyle->SetTitleW(0); // Set the width of the title box
  //myStyle->SetTitleX(0); // Set the position of the title box
  //myStyle->SetTitleY(0.985); // Set the position of the title box
  //myStyle->SetTitleStyle(Style_t style = 1001);
  //myStyle->SetTitleBorderSize(2);

  // For the axis titles:
  //myStyle->SetTitleColor(1, "XYZ");
  myStyle->SetTitleFont(42, "XYZ");
  myStyle->SetTitleSize(0.05, "XYZ");
  //myStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  //myStyle->SetTitleYSize(Float_t size = 0.02);
  myStyle->SetTitleXOffset(0.9);
  myStyle->SetTitleYOffset(1.2);
  //myStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:
  myStyle->SetLabelColor(1, "XYZ");
  myStyle->SetLabelFont(42, "XYZ");
  myStyle->SetLabelOffset(0.007, "XYZ");
  myStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:
  //myStyle->SetAxisColor(1, "XYZ");
  //myStyle->SetStripDecimals(kTRUE);
  //myStyle->SetTickLength(0.03, "XYZ");
  //myStyle->SetNdivisions(510, "XYZ");
  //myStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  //myStyle->SetPadTickY(1);

  // Change for log plots:
  //myStyle->SetOptLogx(logFlagX);
  //myStyle->SetOptLogy(logFlagY);
  //myStyle->SetOptLogz(0);

  // Postscript options:
  //myStyle->SetPaperSize(20.,20.);
  //myStyle->SetLineScalePS(Float_t scale = 3);
  //myStyle->SetLineStyleString(Int_t i, const char* text);
  //myStyle->SetHeaderPS(const char* header);
  //myStyle->SetTitlePS(const char* pstitle);

  //myStyle->SetBarOffset(Float_t baroff = 0.5);
  //myStyle->SetBarWidth(Float_t barwidth = 0.5);
  //myStyle->SetPaintTextFormat(const char* format = "g");
  //myStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  //myStyle->SetTimeOffset(Double_t toffset);
  //myStyle->SetHistMinimumZero(kTRUE);



  myStyle->cd();

}
}

#endif
