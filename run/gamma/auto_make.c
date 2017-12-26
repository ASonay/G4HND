#include <fstream>
#include <iomanip>
using namespace std;

void auto_make()
{ 

  int FN;
  char  filename[200];
  
  for(unsigned int i=20; i<=20000; i=i+20)
    {
      FN = i;
      sprintf(filename, "gamma_beam_%ikeV.in", FN);
  
      ofstream outfile(filename,ios::out);
      if(!outfile)
	{ 
	  cerr<<"open error!"<<endl;
	  exit(1);
	}
  
      outfile<<"/ND/RecMsg/FileName root_files/bin20keV_gamma/gamma_"<<FN<<"keV.root"<<endl;
      outfile<<"/run/initialize"<<endl;
      outfile<<"/gps/source/intensity 100.00"<<endl;
      outfile<<"/gps/pos/type Plane"<<endl;
      outfile<<"/gps/pos/shape Circle"<<endl;
      outfile<<"/gps/pos/radius 2.5 cm"<<endl;
      outfile<<"/gps/pos/centre 0 0 10.8 cm"<<endl;
      outfile<<"/gps/particle gamma"<<endl;
      outfile<<"/gps/energy "<< i<<" keV"<<endl;
      outfile<<"/gps/direction 0 0 -1"<<endl;
      outfile<<"/run/beamOn 1000000"<<endl;
   
      outfile.close();
    }
}
