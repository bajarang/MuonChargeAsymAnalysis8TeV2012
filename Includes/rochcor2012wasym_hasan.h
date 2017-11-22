#include <iostream>
#include <TChain.h>
#include <TClonesArray.h>
#include <TString.h>
#include <map>

#include <TSystem.h>
#include <TROOT.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TRandom3.h>


class rochcor2012 {
 public:
  rochcor2012();
  rochcor2012(int seed);
  ~rochcor2012();
  
  void momcor_mc(TLorentzVector&, float, int, float&);
  void momcor_data(TLorentzVector&, float, int, float&);
  
  int etabin(float);
  int phibin(float);
  
 private:
  
  TRandom3 eran;
  TRandom3 sran;
  
  
  //  static float netabin[9] = {-2.4,-2.1,-1.4,-0.7,0.0,0.7,1.4,2.1,2.4};
  static const double pi = 3.14159265358979323846;
  static const float netabin[25];
  
  static const float genm_smr = 9.10204e+01; //gen mass peak with eta dependent gaussian smearing => better match in Z mass profile vs. eta/phi
  static const float genm = 91.06; //gen mass peak without smearing => Z mass profile vs. eta/phi in CMS note
  
  static const float mrecm = 9.09633e+01; //rec mass peak in MC 
  static const float drecm = 9.06097e+01; //rec mass peak in data 
  static const float mgscl_stat = 0.0001; //stat. error of global factor for mass peak in MC 
  static const float mgscl_syst = 0.0006; //syst. error of global factor for mass peak in MC  
  static const float dgscl_stat = 0.0001; //stat. error of global factor for mass peak in data 
  static const float dgscl_syst = 0.0008; //syst. error of global factor for mass peak in data 
        
  static const float sf[3];
  static const float sfer[3];

  //---------------------------------------------------------------------------------------------
  
  static const float dcor_bf[16][24];  
  static const float dcor_ma[16][24];
  static const float mcor_bf[16][24];
  static const float mcor_ma[16][24];
  static const float dcor_bfer[16][24];  
  static const float dcor_maer[16][24];
  static const float mcor_bfer[16][24];
  static const float mcor_maer[16][24];
  
  //=======================================================================================================
  
  static const float dmavg[16][24];  
  static const float dpavg[16][24];  
  static const float mmavg[16][24];  
  static const float mpavg[16][24];
  
  //===============================================================================================

  float mptsys_mc_dm[16][24];
  float mptsys_mc_da[16][24];
  float mptsys_da_dm[16][24];
  float mptsys_da_da[16][24];

  float gscler_mc_dev;
  float gscler_da_dev;


};
  
