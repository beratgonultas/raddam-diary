#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>
#include "TRandom3.h"
#include "TH1F.h"
#include "TH1.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TColor.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TClonesArray.h"

#include <sys/types.h>
#include <dirent.h>

#include <TMath.h>
#include <TGraph2D.h>
#include <TRandom2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF2.h>
#include <TVirtualFitter.h>
#include <TPolyLine3D.h>
#include <Math/Vector3D.h>
#include <TVector3.h>
#include <TMultiGraph.h>
#include <TH2Poly.h>
#include <TLine.h>

using namespace std;using namespace ROOT::Math;

struct runProperties {
    int runno;
    int runname;
    int datetime;
    float ratioValue;
    float ratioValue1;
    float ratioValue3;
    float ratioError;
    float pedestalValue1;
    float pedestalValue2;
};
runProperties singleRun;
vector <runProperties> allRuns;

struct channel {
    int index;
    vector<runProperties> runForOneChannel;
    TGraph* ratioPlot;
    TGraph* ratioPlot_1;
    TGraph* ratioPlot_2;
    TGraph* ratioPlot_3;
    TGraph* pedestalGraph1;
    TGraph* pedestalGraph2;
    TGraph* pedestalNormalized1;
    TGraph* pedestalNormalized2;
    
    TGraph2D* ratio2D;
};

struct edata {
	vector <Int_t> *ieta;
	vector <Int_t> *iphi;
	vector <Int_t> *depth;
	vector <vector <Int_t>> *pulse;
	vector <vector <Int_t>> *tdc;
	vector <vector <Int_t>> *capid;
};
edata ed;

struct semapin {
	int crate;
	int slot;
	int fiber;
	int channel;
	string PMTname;
	string Channelname;
	string winchester;
	int qiecard;
	string qiecrate;
	int qieslot;
	int PMTid;
	int BBid;
	int ieta;
	int iphi;
	int depth;
	int box;
	string boxname;
	string boxbarcode;
	int VA;
	int VB;
	int VC;
	
	float SX5gain;
	float SX5gainerr;
	float B904pedestal;
	float B904pedestalsigma;
};
semapin semin;

struct semap {
	int crate;
	int slot;
	int fiber;
	int channel;
	string PMTname;
	string Channelname;
	string winchester;
	int qiecard;
	int ieta;
	int iphi;
	int depth;
	int box;
	string boxname;
	
	float SX5gain;
	float SX5gainerr;
	float B904pedestal;
};
semap sem;

//array of semap structures
vector <semap> SEM;

struct ledlist {
	int mapind;
	float Qmean;
	float Qsigma;
	float npemean;
	float npeerr;
	float gain;
	float gainerr;
	float nbadcapid;
	float qf[32];//??
	float ng[32];//??
	
    /*TH1F == 1D histogram with floats*/
    
    TH1F* p;//pulse
	TH1F* t;//tdc
    TH1F* Q1;//Q1 histogram
    TH1F* Q2;//Q2 histogram
    TH1F* Qtot;//Qtot
    TH1F* Qpedest; //Qpedestal (average of charges in TS0 and TS1) histogram
    TH1F* TDC_TS2;//TDC 1 histogram
    TH1F* TDC_TS3;//TDC 2 histogram
    TH2F* Q1vsQ2; //Charges of ts2 and ts3
    TH2F* Q1vsTDC; //Charge - TDC histogram for Q1
    TH2F* Q2vsTDC; //Charge - TDC histogram for Q2
    TH2F* Q2_Q1vsTDC1; // Q2/Q1 vs TDC TS2 2D histogram
    TH2F* Q2_Q1vsTDC2; // Q2/Q1 vs TDC TS3 2D histogram
    TH2F* QtotvsTDC1; // Qtot vs TDC TS2 2D histogram
    TH2F* QtotvsTDC2; // Qtot vs TDC TS3 2D histogram
    TH2F* TDC1vsTDC2; // TDC1 vs TDC2 2D histogram
    TH2F* Q2_Q1vsQ1; // Q2/Q1 vs Q1 2D histogram
    TH2F* Q2_Q1vsQ2; // Q2/Q1 vs Q2 2D histogram
    TH2F* Q2_Q1vsQtot; // Q2/Q1 vs Qtotal 2D histogram
    TH2F* Q2_Q1vsQsum; // Q2/Q1 vs Qtotal 2D histogram
    
    TH2F* tdc1_1vstdc1_2_first;
    TH2F* tdc2_1vstdc2_2_first;
    TH2F* tdc1_1vstdc1_2;
    TH2F* tdc2_1vstdc2_2;
    TH2F* tdc1_1vstdc1_2_third;
    TH2F* tdc2_1vstdc2_2_third;
    //TH2F* q1_1vsq1_2; //Comparison of 2 PMT Channels charge values for TS2
    //TH2F* q2_1vsq2_2; //Comparison of 2 PMT Channels charge values for TS3
    
    TH1F* Q2overQ1_first;//Q2/Q1 for exact delay time values in selectedTSs array (DELAY TIME ~3)
    TH1F* Q2overQ1;//Q2/Q1 for latter delay time values in selectedTSs array (DELAY TIME ~4)
    TH1F* Q2overQ1_third;//delay time 5 for HFP and majority of HFM, delay time 3 for some HFM (DELAY TIME ~5)
    TGraph* Q1_Q2overQtotal;//(Q1+Q2)/Qtot Graph

    Double_t q2overq1;
    Double_t q2overq1_fi;
    Double_t q2overq1_th;
    Double_t q2overq1_error;
    Double_t qped1 = 0;
    Double_t qped2 = 0;
    int runnumber;
    
    TH1F* pn;//pulse norm
	TH1F* Q[6][32];//Q
	TGraph* QP[6];//Q profile
    double PT[10][3];//pulse, TDC, norm

};

//from JM 070516
//Converts analogue signal to digital values
float adc2fC_QIE10[256]={

  // =========== RANGE 0 ===========

  // --------- subrange 1 ---------
  -14.45,-11.35,-8.25,-5.15,-2.05,1.05,4.15,7.25,10.35,13.45,16.55,19.65,22.75,25.85,28.95,32.05,
  // --------- subrange 2 ---------
  36.7,42.9,49.1,55.3,61.5,67.7,73.9,80.1,86.3,92.5,98.7,104.9,111.1,117.3,123.5,129.7,135.9,142.1,148.3,154.5,
  // --------- subrange 3 ---------
  163.8,176.2,188.6,201.0,213.4,225.8,238.2,250.6,263.0,275.4,287.8,300.2,312.6,325.0,337.4,349.8,362.2,374.6,387.0,399.4,411.8,
  // --------- subrange 4 ---------
  430.4,455.2,480.0,504.8,529.6,554.4,579.2,
  // =========== RANGE 1 ===========

  // --------- subrange 1 ---------
  529.4,554.2,579.0,603.8,628.6,653.4,678.2,703.0,727.8,752.6,777.4,802.2,827.0,851.8,876.6,901.4,
  // --------- subrange 2 ---------
  938.6,988.2,1037.8,1087.4,1137.0,1186.6,1236.2,1285.8,1335.4,1385.0,1434.6,1484.2,1533.8,1583.4,1633.0,1682.6,1732.2,1781.8,1831.4,1881.0,
  // --------- subrange 3 ---------
  1955.4,2054.6,2153.8,2253.0,2352.2,2451.4,2550.6,2649.8,2749.0,2848.2,2947.4,3046.6,3145.8,3245.0,3344.2,3443.4,3542.6,3641.8,3741.0,3840.2,3939.4,
  // --------- subrange 4 ---------
  4088.2,4286.6,4485.0,4683.4,4881.8,5080.2,5278.6,
  // =========== RANGE 2 ===========

  // --------- subrange 1 ---------
  4879.2,5077.6,5276.0,5474.4,5672.8,5871.2,6069.6,6268.0,6466.4,6664.8,6863.2,7061.6,7260.0,7458.4,7656.8,7855.2,
  // --------- subrange 2 ---------
  8152.8,8549.6,8946.4,9343.2,9740.0,10136.8,10533.6,10930.4,11327.2,11724.0,12120.8,12517.6,12914.4,13311.2,13708.0,14104.8,14501.6,14898.4,15295.2,15692.0,
  // --------- subrange 3 ---------
  16287.2,17080.8,17874.4,18668.0,19461.6,20255.2,21048.8,21842.4,22636.0,23429.6,24223.2,25016.8,25810.4,26604.0,27397.6,28191.2,28984.8,29778.4,30572.0,31365.6,32159.2,
  // --------- subrange 4 ---------
  33349.6,34936.8,36524.0,38111.2,39698.4,41285.6,42872.8,
  // =========== RANGE 3 ===========

  // --------- subrange 1 ---------
  39693.5,41280.5,42867.5,44454.5,46041.5,47628.5,49215.5,50802.5,52389.5,53976.5,55563.5,57150.5,58737.5,60324.5,61911.5,63498.5,
  // --------- subrange 2 ---------
  65879.0,69053.0,72227.0,75401.0,78575.0,81749.0,84923.0,88097.0,91271.0,94445.0,97619.0,100793.0,103967.0,107141.0,110315.0,113489.0,116663.0,119837.0,123011.0,126185.0,
  // --------- subrange 3 ---------
  130946.0,137294.0,143642.0,149990.0,156338.0,162686.0,169034.0,175382.0,181730.0,188078.0,194426.0,200774.0,207122.0,213470.0,219818.0,226166.0,232514.0,238862.0,245210.0,251558.0,257906.0,
  // --------- subrange 4 ---------
  267428.0,280124.0,292820.0,305516.0,318212.0,330908.0,343604.0

};

int HFMBoxMap[37]={0,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19};

//Initialization of run number, will be defined in main
int RunNo=0;int RunType=0;

int plindMap[2][50][75]={{{0}}};


//Structure of a raddam channel
typedef struct {
    int ieta;
    int iphi;
    int depth;
} Raddam_ch;

//Array that contains 56 raddam channels ieta, iphi and idepth
static const Raddam_ch RADDAM_CH[56]= {{30,1,2},{30,21,1},{30,37,2},{30,57,1},{32,1,1},{32,21,2},{32,37,1},{32,57,2},{34,1,2},{34,21,1},{34,37,2},{34,57,1},{36,1,1},{36,21,2 },{36,37,1},{36,57,2},{38,1,2},{38,21,1},{38,37,2},{38,57,1},{40,19,2},{40,35,1},{40,55,2},{40,71,1},{41,19,1},{41,35,2},{41,55,1},{41,71,2},{-30,15,2},{-30,35,1},{-30,51,2},{-30,71,1},{-32,15,1},{-32,35,2},{-32,51,1},{-32,71,2},{-34,15,2},{-34,35,1},{-34,51,2},{-34,71,1},{-36,15,1},{-36,35,2},{-36,51,1},{-36,71,2},{-38,15,2},{-38,35,1},{-38,51,2},{-38,71,1}, {-40,15,1},{-40,35,2},{-40,51,1},{-40,71,2},{-41,15,2},{-41,35,1},{-41,51,2},{-41,71,1}};

//TDC2 cuts for delay time 5 for HFP and majority of HFM, delay time 3 for some HFM  (DELAY TIME ~5)
static const int tdcCuts3_TS3[56][2]= {{22,25},{60,64},{16,20},{60,64},     {18,22},{60,64},{20,26},{13,18},    {19,22},{60,64},{60,64},{12,14},    {22,28},{14,17},{60,64},{60,64},    {20,27},{60,64},{60,64},{60,64},     {60,64},{20,25},{60,64},{60,64},    {60,64},{20,30},{60,64},{60,64},    {60,64},{60,64},{60,64},{60,64},    {60,64},{60,64},{60,64},{60,64},{60,64},{60,64},{60,64},{60,64},    {15,19},{60,64},{60,64},{60,64},    {60,64},{60,64},{60,64},{60,64},    {60,64},{60,64},{60,64},{60,64},    {60,64},{60,64},{60,64},{60,64}};

//TDC2 cuts for latter of delay time values in selectedTSs array  (DELAY TIME ~4)
static const int tdcCuts2_TS3[56][2]= {{12,16},{60,64},{6,10},{0,8},    {10,12},{60,64},{12,16},{5,7},  {10,12},{60,64},{13,15},{2,5},    {13,17},{5,7},{13,15},{60,64},  {13,16},{60,64},{60,64},{10,12},  {12,14},{12,14},{11,13},{16,18},    {12,13},{15,17},{60,64},{16,22},    {11,13},{60,64},{60,64},{60,64},    {60,64},{60,64},{60,64},{60,64},    {60,64},{60,64},{60,64},{60,64},    {6,9},{0,64},{60,64},{60,64},    {13,15},{60,64},{60,64},{60,64},    {14,16},{60,64},{60,64},{60,64},    {13,15},{60,64},{60,64},{60,64}};

//TDC1 cuts for exact latter of time values in selectedTSs array  (DELAY TIME ~4)
static const int tdcCuts2_TS2[56][2]= {{10,16},{0,64},{0,64},{0,64},    {0,64},{0,64},{0,64},{0,64},  {0,64},{0,64},{0,64},{0,64},   {0,64},{0,64},{0,64},{0,64},  {0,64},{0,64},{0,64},{0,64},  {0,64},{0,64},{0,64},{0,64},    {0,64},{14,18},{0,64},{0,64},  {0,64},{0,64},{0,64},{0,64},   {0,64},{0,64},{0,64},{0,64},  {0,64},{0,64},{0,64},{0,64},    {0,64},{0,64},{0,64},{0,64},    {0,64},{0,64},{0,64},{0,64},  {0,64},{0,64},{0,64},{0,64},   {0,64},{0,64},{0,64},{0,64}};

//TDC2 cuts for exact delay time values in selectedTSs array  (DELAY TIME ~3)
static const int tdcCuts1_TS3[56][2]= {{4,6},{60,64},{60,64},{60,64},    {0,3},{60,64},{4,6},{60,64},  {0,3},{60,64},{4,7},{60,64},    {5,7},{60,64},{2,6},{60,64},  {4,6},{60,64},{60,64},{0,2},  {1,5},{3,5},{0,3},{6,10},    {0,5},{4,8},{60,64},{7,14},    {0,6},{0,64},{60,64},{60,64},    {60,64},{60,64},{60,64},{60,64},    {60,64},{60,64},{60,64},{60,64},    {60,64},{60,64},{60,64},{60,64},    {4,6},{60,64},{60,64},{60,64},    {5,6},{60,64},{60,64},{60,64},    {4,7},{60,64},{60,64},{60,64}};

//TDC1 cuts for exact delay time values in selectedTSs array  (DELAY TIME ~3)
static const int tdcCuts1_TS2[56][2]= {{2,6},{0,4},{60,64},{0,3},    {0,2},{60,64},{4,7},{60,64},  {0,3},{6,8},{4,7},{60,64},    {4,6},{60,64},{3,5},{2,4},  {3,6},{4,6},{60,64},{0,4},  {2,6},{4,6},{0,4},{0,64},    {2,6},{4,8},{0,4},{6,10},    {0,7},{60,64},{0,2},{4,8},    {0,10},{0,10},{0,10},{0,10},    {0,10},{60,64},{0,10},{0,10},    {60,64},{0,10},{60,64},{0,10},    {0,10},{0,10},{0,10},{0,10},    {0,10},{0,10},{0,10},{0,64},    {0,10},{0,10},{0,10},{0,10}};

static const int selectedTSs[56]= {3,3,3,3, 3,3,4,3, 3,4,4,3, 4,3,4,4, 4,4,4,4, 4,4,4,4, 4,4,4,2, 3,3,3,2, 3,4,4,2, 4,4,4,2, 3,4,3,2, 4,4,4,2, 4,4,4,2, 4,4,4,4};

int getmap() {
    
    //reads the file semapex_v3.txt
	ifstream semapinf("semapex_v3.txt");
	while(!semapinf.eof()) {
        //Reads formatted input from the file, and initializes an object semin
				semapinf>>semin.crate>>semin.slot>>semin.fiber>>semin.channel>>semin.PMTname>>semin.Channelname>>semin.winchester>>semin.qiecard>>semin.qiecrate>>semin.qieslot>>semin.PMTid>>semin.BBid>>semin.ieta>>semin.iphi>>semin.depth>>semin.box>>semin.boxname>>semin.boxbarcode>>semin.VA>>semin.VB>>semin.VC>>semin.SX5gain>>semin.SX5gainerr>>semin.B904pedestal>>semin.B904pedestalsigma;
		
		sem.crate=semin.crate;
		sem.slot=semin.slot;
		sem.fiber=semin.fiber;
		sem.channel=semin.channel;
		sem.PMTname=semin.PMTname;
		sem.Channelname=semin.Channelname;
		sem.winchester=semin.winchester;
		sem.qiecard=semin.qiecard;
		sem.ieta=semin.ieta;
		sem.iphi=semin.iphi;
		sem.depth=semin.depth;
		sem.box=semin.box;
		sem.boxname=semin.boxname;
		sem.SX5gain=semin.SX5gain;
		sem.SX5gainerr=semin.SX5gainerr;
		sem.B904pedestal=semin.B904pedestal;
		SEM.push_back(sem);
	}
	semapinf.close();
}

/*
 * Reads the NTuple file, get events and channel info, initializes histograms,
 * select events and fills histograms, writes the results to a file
 */
int plotleds() {
    
	char hname[500];char hname2[500];
	sprintf(hname,"../NTuples/N_%d.root",RunNo); //Input file that is created by ntupler, i.e. config file
	TFile* inroot=new TFile(hname); //Root binary file that includes events
	TTree *tree = (TTree*)inroot->Get("Events"); //Reads file and gets events
	tree->SetBranchAddress("ieta",&ed.ieta);
	tree->SetBranchAddress("iphi",&ed.iphi);
	tree->SetBranchAddress("depth",&ed.depth);
	tree->SetBranchAddress("pulse",&ed.pulse);
	tree->SetBranchAddress("tdc",&ed.tdc);
	tree->SetBranchAddress("capid",&ed.capid);
	
	//int ev1=e1;int ev2=e2;
	//if(ev1==-1) {
    int ev1=0;
    int ev2=tree->GetEntries();
    //}
	
	ledlist pl; //Contains all histograms in ledlist structure for a single channel
    vector <ledlist> PL; // list of pl structure of all channels
    int plind=-1;
    
	tree->GetEntry(0);
	int nTS=ed.pulse->at(0).size(); // number of time slices

	int qpcols[6]={9,1,2,3,4,6};//???
    
    //HISTO INITIALIZATIONS
	for(int i1=0;i1<ed.ieta->size();i1++) {
		plind=-1;
        
		for(int i2=0;i2<PL.size();i2++) {
            
			if(SEM[PL[i2].mapind].ieta==ed.ieta->at(i1) && SEM[PL[i2].mapind].iphi==ed.iphi->at(i1)){
				plind=i2;
                break;
			}
		}
		if(plind==-1) {
            
			pl.mapind=-1;
			for(int i2=0;i2<SEM.size();i2++) {
                
				if(SEM[i2].ieta==ed.ieta->at(i1) && SEM[i2].iphi==ed.iphi->at(i1)) {
					pl.mapind=i2;
                    break;
				}
			}
			pl.Qmean=0.;
			pl.Qsigma=0.;
			pl.npemean=0.;
			pl.npeerr=0.;
			pl.gain=0.;
			pl.gainerr=0.;
			pl.nbadcapid=0.;

            pl.runnumber = RunNo; //Run number
            
			for(int ik1=0;ik1<32;ik1++) {
				pl.qf[ik1]=0.;
				pl.ng[ik1]=0.;
			}
            
			/* 2D Histogram of Pulse Shapes of Q1 and Q2 */
            sprintf(hname, "Q1 vs Q2 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q1vsQ2 = new TH2F(hname,hname,200,0.,2000.,200,0.,2000.);
            pl.Q1vsQ2->SetCanExtend(TH2::kAllAxes);
            pl.Q1vsQ2->GetXaxis()->SetTitle("Pulse Q1");pl.Q1vsQ2->GetXaxis()->CenterTitle();
            pl.Q1vsQ2->GetYaxis()->SetTitle("Pulse Q2");pl.Q1vsQ2->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of ADC vs TDC for Q1 */
            sprintf(hname, "Q1 vs TDC1 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q1vsTDC = new TH2F(hname,hname,200,0,200,130,0,129);
            pl.Q1vsTDC->SetCanExtend(TH2::kAllAxes);
            pl.Q1vsTDC->GetXaxis()->SetTitle("Q1 (fC)");pl.Q1vsTDC->GetXaxis()->CenterTitle();
            pl.Q1vsTDC->GetYaxis()->SetTitle("TDC1");pl.Q1vsTDC->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of ADC vs TDC for Q2 */
            sprintf(hname, "Q2 vs TDC2 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q2vsTDC = new TH2F(hname,hname,200,0,200,130,0,129);
            pl.Q2vsTDC->SetCanExtend(TH2::kAllAxes);
            pl.Q2vsTDC->GetXaxis()->SetTitle("Q2 (fC)");pl.Q2vsTDC->GetXaxis()->CenterTitle();
            pl.Q2vsTDC->GetYaxis()->SetTitle("TDC2");pl.Q2vsTDC->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of Q2/Q1 vs TDC TS2 */
            sprintf(hname, "Q2/Q1 vs TDC1 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q2_Q1vsTDC1 = new TH2F(hname,hname,30,-3,3.,130,0,129);
            pl.Q2_Q1vsTDC1->SetCanExtend(TH2::kAllAxes);
            pl.Q2_Q1vsTDC1->GetXaxis()->SetTitle("Q2/Q1");pl.Q2_Q1vsTDC1->GetXaxis()->CenterTitle();
            pl.Q2_Q1vsTDC1->GetYaxis()->SetTitle("TDC1");pl.Q2_Q1vsTDC1->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of Q2/Q1 vs TDC TS3 */
            sprintf(hname, "Q2/Q1 vs TDC2 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q2_Q1vsTDC2 = new TH2F(hname,hname,30,-3.,3.,130,0,129);
            pl.Q2_Q1vsTDC2->SetCanExtend(TH2::kAllAxes);
            pl.Q2_Q1vsTDC2->GetXaxis()->SetTitle("Q2/Q1");pl.Q2_Q1vsTDC2->GetXaxis()->CenterTitle();
            pl.Q2_Q1vsTDC2->GetYaxis()->SetTitle("TDC2");pl.Q2_Q1vsTDC2->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of Q2/Q1 vs Q1 */
            sprintf(hname, "Q2/Q1 vs Q1 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q2_Q1vsQ1 = new TH2F(hname,hname,30,-3,3.,10,0,9);
            pl.Q2_Q1vsQ1->SetCanExtend(TH2::kAllAxes);
            pl.Q2_Q1vsQ1->GetXaxis()->SetTitle("Q2/Q1");pl.Q2_Q1vsQ1->GetXaxis()->CenterTitle();
            pl.Q2_Q1vsQ1->GetYaxis()->SetTitle("Q1");pl.Q2_Q1vsQ1->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of Q2/Q1 vs Q2 */
            sprintf(hname, "Q2/Q1 vs Q2 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q2_Q1vsQ2 = new TH2F(hname,hname,30,-3.,3.,10,0,9);
            pl.Q2_Q1vsQ2->SetCanExtend(TH2::kAllAxes);
            pl.Q2_Q1vsQ2->GetXaxis()->SetTitle("Q2/Q1");pl.Q2_Q1vsQ2->GetXaxis()->CenterTitle();
            pl.Q2_Q1vsQ2->GetYaxis()->SetTitle("Q2");pl.Q2_Q1vsQ2->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of Q2/Q1 vs Qtotal */
            sprintf(hname, "Q2/Q1 vs Qtotal (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q2_Q1vsQtot = new TH2F(hname,hname,30,-3.,3.,10,0,9);
            pl.Q2_Q1vsQtot->SetCanExtend(TH2::kAllAxes);
            pl.Q2_Q1vsQtot->GetXaxis()->SetTitle("Q2/Q1");pl.Q2_Q1vsQtot->GetXaxis()->CenterTitle();
            pl.Q2_Q1vsQtot->GetYaxis()->SetTitle("Qtot");pl.Q2_Q1vsQtot->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of Q2/Q1 vs Qtotal */
            sprintf(hname, "Q2/Q1 vs (Q1+Q2) (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q2_Q1vsQsum = new TH2F(hname,hname,30,-3.,3.,10,0,9);
            pl.Q2_Q1vsQsum->SetCanExtend(TH2::kAllAxes);
            pl.Q2_Q1vsQsum->GetXaxis()->SetTitle("Q2/Q1");pl.Q2_Q1vsQsum->GetXaxis()->CenterTitle();
            pl.Q2_Q1vsQsum->GetYaxis()->SetTitle("Q1+Q2");pl.Q2_Q1vsQsum->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of Qtot vs TDC TS2 */
            sprintf(hname, "Qtot vs TDC1 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.QtotvsTDC1 = new TH2F(hname,hname,21,-10.,10.,130,0,129);
            pl.QtotvsTDC1->SetCanExtend(TH2::kAllAxes);
            pl.QtotvsTDC1->GetXaxis()->SetTitle("Qtot");pl.QtotvsTDC1->GetXaxis()->CenterTitle();
            pl.QtotvsTDC1->GetYaxis()->SetTitle("TDC1");pl.QtotvsTDC1->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of Qtot vs TDC TS3 */
            sprintf(hname, "Qtot vs TDC2 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.QtotvsTDC2 = new TH2F(hname,hname,21,-10.,10.,130,0,129);
            pl.QtotvsTDC2->SetCanExtend(TH2::kAllAxes);
            pl.QtotvsTDC2->GetXaxis()->SetTitle("Qtot");pl.QtotvsTDC2->GetXaxis()->CenterTitle();
            pl.QtotvsTDC2->GetYaxis()->SetTitle("TDC2");pl.QtotvsTDC2->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of TDC1 vs TDC2 */
            sprintf(hname, "TDC1 vs TDC2 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.TDC1vsTDC2 = new TH2F(hname,hname,130,0,129,130,0,129);
            pl.TDC1vsTDC2->GetXaxis()->SetTitle("TDC1");pl.TDC1vsTDC2->GetXaxis()->CenterTitle();
            pl.TDC1vsTDC2->GetYaxis()->SetTitle("TDC2");pl.TDC1vsTDC2->GetYaxis()->CenterTitle();
            
            /* Q1 1D histogram */
            sprintf(hname,"Q1 %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q1=new TH1F(hname,hname,100,0.,1000.);
            pl.Q1->SetCanExtend(TH1::kAllAxes);
            pl.Q1->GetXaxis()->SetTitle("Q_{1}(fC)");pl.Q1->GetXaxis()->CenterTitle();
            pl.Q1->GetYaxis()->SetTitle("Events");pl.Q1->GetYaxis()->CenterTitle();
            if(SEM[pl.mapind].depth<=2) pl.Q1->SetLineColor(3);
            else pl.Q1->SetLineColor(4);
            
            /* Q2 1D histogram */
            sprintf(hname,"Q2 %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q2=new TH1F(hname,hname,100,0.,1000.);
            pl.Q2->SetCanExtend(TH1::kAllAxes);
            pl.Q2->GetXaxis()->SetTitle("Q_{2}(fC)");pl.Q2->GetXaxis()->CenterTitle();
            pl.Q2->GetYaxis()->SetTitle("Events");pl.Q2->GetYaxis()->CenterTitle();
            pl.Q2->SetLineColor(7);
            
            /* Qpedest 1D histogram */
            sprintf(hname,"Qpedestal %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Qpedest=new TH1F(hname,hname,100,-5.,5.);
            pl.Qpedest->SetCanExtend(TH1::kAllAxes);
            pl.Qpedest->GetXaxis()->SetTitle("Q_{pedestal}(fC)");pl.Qpedest->GetXaxis()->CenterTitle();
            pl.Qpedest->GetYaxis()->SetTitle("Events");pl.Qpedest->GetYaxis()->CenterTitle();
            pl.Qpedest->SetLineColor(51);
            
            /* TDC 1 1D histogram */
            sprintf(hname,"TDC Q1 %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.TDC_TS2=new TH1F(hname,hname,67,0,66);
            pl.TDC_TS2->GetXaxis()->SetTitle("TDC_{1}");pl.TDC_TS2->GetXaxis()->CenterTitle();
            pl.TDC_TS2->GetYaxis()->SetTitle("Events");pl.TDC_TS2->GetYaxis()->CenterTitle();
            pl.TDC_TS2->SetLineColor(6);
            
            /* TDC 2 1D histogram */
            sprintf(hname,"TDC Q2 %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.TDC_TS3=new TH1F(hname,hname,130,0,129);
            pl.TDC_TS3->GetXaxis()->SetTitle("TDC_{2}");pl.TDC_TS3->GetXaxis()->CenterTitle();
            pl.TDC_TS3->GetYaxis()->SetTitle("Events");pl.TDC_TS3->GetYaxis()->CenterTitle();
            pl.TDC_TS3->SetLineColor(8);
            
            /* s1/s2 1d histogram */
			sprintf(hname2,"Q2/Q1 %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
			pl.Q2overQ1=new TH1F(hname2,hname2,100,0.,5.);
            pl.Q2overQ1->SetCanExtend(TH1::kAllAxes);
			pl.Q2overQ1->GetXaxis()->SetTitle("Q_{2}/Q_{1}");pl.Q2overQ1->GetXaxis()->CenterTitle();
			pl.Q2overQ1->GetYaxis()->SetTitle("Events");pl.Q2overQ1->GetYaxis()->CenterTitle();
			pl.Q2overQ1->SetLineColor(30);
            
            /* s1/s2 1d histogram */
            sprintf(hname2,"Q2/Q1 Former %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q2overQ1_first=new TH1F(hname2,hname2,100,0.,10.);
            pl.Q2overQ1_first->SetCanExtend(TH1::kAllAxes);
            pl.Q2overQ1_first->GetXaxis()->SetTitle("Q_{2}/Q_{1} 1st");pl.Q2overQ1_first->GetXaxis()->CenterTitle();
            pl.Q2overQ1_first->GetYaxis()->SetTitle("Events");pl.Q2overQ1_first->GetYaxis()->CenterTitle();
            pl.Q2overQ1_first->SetLineColor(25);

            /* s1/s2 1d histogram */
            sprintf(hname2,"Q2/Q1 Latter %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q2overQ1_third=new TH1F(hname2,hname2,100,0.,10.);
            pl.Q2overQ1_third->SetCanExtend(TH1::kAllAxes);
            pl.Q2overQ1_third->GetXaxis()->SetTitle("Q_{2}/Q_{1} 3rd");pl.Q2overQ1_third->GetXaxis()->CenterTitle();
            pl.Q2overQ1_third->GetYaxis()->SetTitle("Events");pl.Q2overQ1_third->GetYaxis()->CenterTitle();
            pl.Q2overQ1_third->SetLineColor(35);
            
            /* Qtot 1d histogram */
			sprintf(hname2,"Qtot %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
			pl.Qtot=new TH1F(hname2,hname2,1000,0.,1000.);
            pl.Qtot->SetCanExtend(TH1::kAllAxes);
			pl.Qtot->GetXaxis()->SetTitle("Qtot (fC)"); pl.Qtot->GetXaxis()->CenterTitle();
			pl.Qtot->GetYaxis()->SetTitle("Events"); pl.Qtot->GetYaxis()->CenterTitle();
			pl.Qtot->SetLineColor(8);
			
            /* 2D Histogram of TDC1-1 vs TDC1-2 */
            sprintf(hname, "TDC1-1 vs TDC1-2 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.tdc1_1vstdc1_2 = new TH2F(hname,hname,67,0,66,67,0,66);
            pl.tdc1_1vstdc1_2->GetXaxis()->SetTitle("TDC1-1");pl.tdc1_1vstdc1_2->GetXaxis()->CenterTitle();
            pl.tdc1_1vstdc1_2->GetYaxis()->SetTitle("TDC1-2");pl.tdc1_1vstdc1_2->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of TDC2-1 vs TDC2-2 */
            sprintf(hname, "TDC2-1 vs TDC2-2 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.tdc2_1vstdc2_2 = new TH2F(hname,hname,67,0,66,67,0,66);
            pl.tdc2_1vstdc2_2->GetXaxis()->SetTitle("TDC2-1");pl.tdc2_1vstdc2_2->GetXaxis()->CenterTitle();
            pl.tdc2_1vstdc2_2->GetYaxis()->SetTitle("TDC2-2");pl.tdc2_1vstdc2_2->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of TDC1-1 vs TDC1-2 FORMER*/
            sprintf(hname, "TDC1-1 vs TDC1-2 Former (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.tdc1_1vstdc1_2_first = new TH2F(hname,hname,67,0,66,67,0,66);
            pl.tdc1_1vstdc1_2_first->GetXaxis()->SetTitle("TDC1-1");pl.tdc1_1vstdc1_2_first->GetXaxis()->CenterTitle();
            pl.tdc1_1vstdc1_2_first->GetYaxis()->SetTitle("TDC1-2");pl.tdc1_1vstdc1_2_first->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of TDC2-1 vs TDC2-2 FORMER*/
            sprintf(hname, "TDC2-1 vs TDC2-2 Former (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.tdc2_1vstdc2_2_first = new TH2F(hname,hname,67,0,66,67,0,66);
            pl.tdc2_1vstdc2_2_first->GetXaxis()->SetTitle("TDC2-1");pl.tdc2_1vstdc2_2_first->GetXaxis()->CenterTitle();
            pl.tdc2_1vstdc2_2_first->GetYaxis()->SetTitle("TDC2-2");pl.tdc2_1vstdc2_2_first->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of TDC1-1 vs TDC1-2 LATTER */
            sprintf(hname, "TDC1-1 vs TDC1-2 Latter (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.tdc1_1vstdc1_2_third = new TH2F(hname,hname,67,0,66,67,0,66);
            pl.tdc1_1vstdc1_2_third->GetXaxis()->SetTitle("TDC1-1");pl.tdc1_1vstdc1_2_third->GetXaxis()->CenterTitle();
            pl.tdc1_1vstdc1_2_third->GetYaxis()->SetTitle("TDC1-2");pl.tdc1_1vstdc1_2_third->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of TDC2-1 vs TDC2-2 */
            sprintf(hname, "TDC2-1 vs TDC2-2 Latter (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.tdc2_1vstdc2_2_third = new TH2F(hname,hname,67,0,66,67,0,66);
            pl.tdc2_1vstdc2_2_third->GetXaxis()->SetTitle("TDC2-1");pl.tdc2_1vstdc2_2_third->GetXaxis()->CenterTitle();
            pl.tdc2_1vstdc2_2_third->GetYaxis()->SetTitle("TDC2-2");pl.tdc2_1vstdc2_2_third->GetYaxis()->CenterTitle();
            
            /* 2D Histogram of Q1-1 vs Q1-2
            sprintf(hname, "Q1-1 vs Q1-2 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.q1_1vsq1_2 = new TH2F(hname,hname,100,0,100,100,0,100);
            pl.q1_1vsq1_2->SetCanExtend(TH2::kAllAxes);
            pl.q1_1vsq1_2->GetXaxis()->SetTitle("Q1-1");pl.q1_1vsq1_2->GetXaxis()->CenterTitle();
            pl.q1_1vsq1_2->GetYaxis()->SetTitle("Q1-2");pl.q1_1vsq1_2->GetYaxis()->CenterTitle();*/
            
            /* 2D Histogram of Q2-1 vs Q2-2
            sprintf(hname, "Q2-1 vs Q2-2 (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.q2_1vsq2_2 = new TH2F(hname,hname,100,0,100,100,0,100);
            pl.q2_1vsq2_2->SetCanExtend(TH2::kAllAxes);
            pl.q2_1vsq2_2->GetXaxis()->SetTitle("Q2-1");pl.q2_1vsq2_2->GetXaxis()->CenterTitle();
            pl.q2_1vsq2_2->GetYaxis()->SetTitle("Q2-2");pl.q2_1vsq2_2->GetYaxis()->CenterTitle();*/
            
            
			for(int is1=0;is1<10;is1++) {
				for(int is2=0;is2<3;is2++) {
					pl.PT[is1][is2]=0.;
				}
			}
            
            /* Pulse Shapes 1D Histogram */
			sprintf(hname,"#splitline{Pulse %d %s %s}{      (%d, %d)}", SEM[pl.mapind].box,SEM[pl.mapind].PMTname.c_str(),SEM[pl.mapind].Channelname.c_str(),SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            //Name of the histogram contains box number, PMT name and Channel name sequentially
			sprintf(hname2,"Pulse %d %s %s",SEM[pl.mapind].box,SEM[pl.mapind].PMTname.c_str(),SEM[pl.mapind].Channelname.c_str());
			pl.p=new TH1F(hname2,hname,nTS,-0.5,((float)nTS)-0.5); //Divided the axis to the TS number
			pl.p->GetXaxis()->SetTitle("TS (x25 ns)");pl.p->GetXaxis()->CenterTitle();
			pl.p->GetYaxis()->SetTitle("Mean Charge per TS (fC)");pl.p->GetYaxis()->CenterTitle();
			pl.p->SetFillColor(4);pl.p->SetLineColor(4);
			
            /* TDC Shapes 1D Histogram */
			sprintf(hname,"#splitline{TDC %d %s %s}{      (%d, %d)}", SEM[pl.mapind].box,SEM[pl.mapind].PMTname.c_str(),SEM[pl.mapind].Channelname.c_str(),SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
			sprintf(hname2,"TDC %d %s %s",SEM[pl.mapind].box,SEM[pl.mapind].PMTname.c_str(),SEM[pl.mapind].Channelname.c_str());
			pl.t=new TH1F(hname2,hname,nTS,-0.5,((float)nTS)-0.5);
			pl.t->GetXaxis()->SetTitle("TS (x25 ns)");pl.t->GetXaxis()->CenterTitle();
			pl.t->GetYaxis()->SetTitle("Mean TDC per TS");pl.t->GetYaxis()->CenterTitle();
			pl.t->SetLineColor(46);
            
            //(Q1+Q2)/Qtot graph
            pl.Q1_Q2overQtotal = new TGraph();
            sprintf(hname, "(Q1+Q2)/Qtotal (%d %d)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
            pl.Q1_Q2overQtotal->SetName(hname);pl.Q1_Q2overQtotal->SetTitle(hname);
            pl.Q1_Q2overQtotal->SetMarkerStyle(1);pl.Q1_Q2overQtotal->SetMarkerColor(9);
            pl.Q1_Q2overQtotal->GetYaxis()->SetRangeUser(0,1.2);  pl.Q1_Q2overQtotal->GetXaxis()->SetRangeUser(0,35000);
            pl.Q1_Q2overQtotal->GetYaxis()->SetTitle("(Q1+Q2)/Qtotal"); pl.Q1_Q2overQtotal->GetXaxis()->SetTitle("Events");
			
			PL.push_back(pl);
		}
	}
    //END OF HISTO INITIALIZATIONS
	
	int MI=-1;
	for(int i2=0;i2<PL.size();i2++) {
        
		MI=PL[i2].mapind;
        //HFM plindMap[0] and HFP plindMap[1] = i2
        //plindMap maps the PL vector content to channels (ieta,iphi,idepth)
		plindMap[(SEM[MI].ieta<0)?0:1][abs(SEM[MI].ieta)][SEM[MI].iphi]=i2;
	}
	
	double ped=0.;double sig=0.;bool capidOK=true;int cap0=0;
    
    //EVENT SELECTION AND FILLING THE HISTOGRAMS
    for(int i=ev1;i<ev2;i++) {
        
        tree->GetEntry(i);// gets i'th event

        float tdc1values[PL.size()], tdc2values[PL.size()], tdc1values_1[PL.size()], tdc2values_1[PL.size()], tdc1values_3[PL.size()], tdc2values_3[PL.size()], q1s_1[PL.size()], q2s_1[PL.size()], q1s_3[PL.size()], q2s_3[PL.size()],q1s[PL.size()], q2s[PL.size()],qtots[PL.size()], qpeds[PL.size()] ;
        float NQ1, NQ2, NQp;
        
        for(int i1=0;i1<ed.ieta->size();i1++){
            
            plind=plindMap[(ed.ieta->at(i1)<0)?0:1][abs(ed.ieta->at(i1))][ed.iphi->at(i1)];
            ped=0.;sig=0.;capidOK=true;
            float QQ[10]={0.};float TDC[10]={0.};float Qtot=0.;float qf=0.;
            
            /*float charge1 = adc2fC_QIE10[ed.pulse->at(i1)[2]] ;
            float charge2 = adc2fC_QIE10[ed.pulse->at(i1)[3]];
            float chargetotal=0.0;
            
            for(int i3 = 0 ; i3 < nTS; i3++) {
                chargetotal += adc2fC_QIE10[ed.pulse->at(i1)[i3]];
            }
            
            float chargeratios = (charge1 + charge2)/chargetotal;
            
            PL[plind].Q1_Q2overQtotal->SetPoint(PL[plind].Q1_Q2overQtotal->GetN(),i, chargeratios);
            
            bool a = i>=20000 && i<25000 ;
            bool b = (ed.ieta->at(i1)>0) || (ed.iphi->at(i1) != 71) || (ed.ieta->at(i1)) == -41;
            bool c = i>=10000 && i<15000;
            bool d = (ed.ieta->at(i1)<0) && (ed.iphi->at(i1) == 71) && (abs(ed.ieta->at(i1))<= 40 );*/
            
            bool e1, e2, e3;
            
            for( int chan = 0 ; chan < 56 ; chan++) {
                if ((ed.ieta->at(i1)==RADDAM_CH[chan].ieta) && (ed.iphi->at(i1)==RADDAM_CH[chan].iphi)){
                    if (RADDAM_CH[chan].ieta < 0 && RADDAM_CH[chan].ieta != -41 && RADDAM_CH[chan].iphi == 35 && RunNo > 322851){
                        e1 = (i >= 5000) && i < 10000;
                        e2 = (i >= 10000) && i < 15000;
                        e3 = (i >= 15000) && i < 20000;
                        break;
                    }
                    e1 = (i >= (selectedTSs[chan]-1)*5000 && i < selectedTSs[chan]*5000);
                    e2 = (i >= selectedTSs[chan]*5000 && i < (selectedTSs[chan]+1)*5000);
                    e3 = (i >= (selectedTSs[chan]+1)*5000 && i < (selectedTSs[chan]+2)*5000);
                    break;
                }
            }

            if(e1){ //THE EARLIER ONE
                
                for(int i2=0;i2<nTS;i2++) {
                    QQ[i2]=adc2fC_QIE10[ed.pulse->at(i1)[i2]];
                }
                
                //Cuts for invalid TDC values:
                bool f1, f2, f ;
                for( int chan = 0 ; chan < 56 ; chan++) {
                    if ((ed.ieta->at(i1)==RADDAM_CH[chan].ieta) && (ed.iphi->at(i1)==RADDAM_CH[chan].iphi)){
                        f1 = ((ed.tdc->at(i1)[3] > tdcCuts1_TS3[chan][1]) || (ed.tdc->at(i1)[3] < tdcCuts1_TS3[chan][0]));
                        f2 = ((ed.tdc->at(i1)[2] > tdcCuts1_TS2[chan][1]) || (ed.tdc->at(i1)[2] < tdcCuts1_TS2[chan][0]));
                        f = f1 || f2;
                        break;
                    }
                }
                if(!f) {
                    if(i1 < ed.ieta->size()/2){

                        tdc1values_1[i1] = ed.tdc->at(i1)[2];
                        tdc2values_1[i1] = ed.tdc->at(i1)[3];
                        q1s_1[i1] = QQ[2] - (QQ[0]+QQ[1])/2 ;
                        q2s_1[i1] = QQ[3] - (QQ[0]+QQ[1])/2 ;
                        
                    } else if (q1s_1[i1-ed.ieta->size()/2] != -1) {
                        
                        //Normalized Q1 and Q2 values for 2 channels
                        NQ1 = QQ[2] - (QQ[0]+QQ[1])/2 + q1s_1[i1-ed.ieta->size()/2];
                        NQ2 = QQ[3] - (QQ[0]+QQ[1])/2 + q2s_1[i1-ed.ieta->size()/2];
                        
                        PL[plind].Q2overQ1_first->Fill(NQ2/NQ1);
                        PL[plind].tdc1_1vstdc1_2_first->Fill(tdc1values_1[i1-ed.ieta->size()/2], ed.tdc->at(i1)[2]);
                        PL[plind].tdc2_1vstdc2_2_first->Fill(tdc2values_1[i1-ed.ieta->size()/2],ed.tdc->at(i1)[3]);
                    }
                } else {
                    q1s_1[i1] = -1;
                }
                
            }
            
            if(e2){ //THE MAIN ONE
                
                for(int i2=0;i2<nTS;i2++) {
                    //PL->PT is a [10][3] array whic corresponds to values at 10 TS of pulse tcd and norm
                    PL[plind].PT[i2][0]+=adc2fC_QIE10[ed.pulse->at(i1)[i2]]; //Pulse
                    PL[plind].PT[i2][1]+=ed.tdc->at(i1)[i2]; // Tdc
                    PL[plind].PT[i2][2]+=1.; //Norm
                    QQ[i2]=adc2fC_QIE10[ed.pulse->at(i1)[i2]];
                    TDC[i2]=ed.tdc->at(i1)[i2];
                    Qtot+=QQ[i2];//Total charge of this channel this event
                    
                    if(i2 > 0) {
                        if(!((ed.capid->at(i1)[i2]-ed.capid->at(i1)[i2-1])==1 || (ed.capid->at(i1)[i2]-ed.capid->at(i1)[i2-1])==-3)) {
                            capidOK=false;
                        }
                    }
                }
                
                if(i1 < ed.ieta->size()/2){
                    PL[plind].qped1 += (QQ[0]+QQ[1])/2; //cout << plind << " birinci " << ed.ieta->at(i1) << " " << ed.iphi->at(i1) << " " << ed.depth->at(i1) << endl;
                } else {
                    PL[plind].qped2 += (QQ[0]+QQ[1])/2; //cout << plind << " ikinci " << ed.ieta->at(i1) << " " << ed.iphi->at(i1) << " " << ed.depth->at(i1) << endl;
                }
                
                //Cuts for invalid TDC values:
                bool f1, f2, f ;
                for( int chan = 0 ; chan < 56 ; chan++) {
                    if ((ed.ieta->at(i1)==RADDAM_CH[chan].ieta) && (ed.iphi->at(i1)==RADDAM_CH[chan].iphi)){
                        f1 = ((ed.tdc->at(i1)[3] > tdcCuts2_TS3[chan][1]) || (ed.tdc->at(i1)[3] < tdcCuts2_TS3[chan][0]));
                        f2 = ((ed.tdc->at(i1)[2] > tdcCuts2_TS2[chan][1]) || (ed.tdc->at(i1)[2] < tdcCuts2_TS2[chan][0]));
                        f = f1 || f2;
                        break;
                    }
                }
                
                if(!capidOK) PL[plind].nbadcapid+=1.;
                
                if(!f) {

                    if(i1 < ed.ieta->size()/2){
                        
                        //q1values[i1] = QQ[2];
                        //q2values[i1] = QQ[3];
                        tdc1values[i1] = ed.tdc->at(i1)[2];
                        tdc2values[i1] = ed.tdc->at(i1)[3];
                        q1s[i1] = QQ[2] - (QQ[0]+QQ[1])/2 ;
                        q2s[i1] = QQ[3] - (QQ[0]+QQ[1])/2 ;
                        qtots[i1] = Qtot ;
                        qpeds[i1] = (QQ[0]+QQ[1])/2 ;
                        
                    } else if (q1s[i1-ed.ieta->size()/2] != -1) {
                        
                        //Normalized Q1 and Q2 values for 2 channels
                        NQ1 = QQ[2] - (QQ[0]+QQ[1])/2 + q1s[i1-ed.ieta->size()/2];
                        NQ2 = QQ[3] - (QQ[0]+QQ[1])/2 + q2s[i1-ed.ieta->size()/2];
                        NQp = (QQ[0]+QQ[1])/2 + qpeds[i1-ed.ieta->size()/2];
                        
                        //Charge histograms
                        PL[plind].Q2overQ1->Fill(NQ2/NQ1);
                        PL[plind].Q1vsQ2->Fill(NQ1, NQ2); //Fills Q1 vs Q2 histogram
                        PL[plind].Q1->Fill(NQ1);
                        PL[plind].Q2->Fill(NQ2);
                        PL[plind].Qtot->Fill(Qtot+ qtots[i1-ed.ieta->size()/2]);
                        PL[plind].Qpedest->Fill(NQp);
                        PL[plind].Q2_Q1vsQ1->Fill(NQ2/NQ1,NQ1); //Fills Q2/Q1 vs Q1
                        PL[plind].Q2_Q1vsQ2->Fill(NQ2/NQ1,NQ2); //Fills Q2/Q1 vs Q2
                        PL[plind].Q2_Q1vsQtot->Fill(NQ2/NQ1,Qtot+ qtots[i1-ed.ieta->size()/2]); //Fills Q2/Q1 vs Qtotal
                        PL[plind].Q2_Q1vsQsum->Fill(NQ2/NQ1,NQ1+NQ2); //Fills Q2/Q1 vs Q1+Q2
                        
                        //TDC Histograms
                        PL[plind].TDC_TS2->Fill(ed.tdc->at(i1)[2] + tdc1values[i1-ed.ieta->size()/2] );
                        PL[plind].TDC_TS3->Fill(ed.tdc->at(i1)[3] + tdc2values[i1-ed.ieta->size()/2]);
                        PL[plind].QtotvsTDC1->Fill(Qtot+ qtots[i1-ed.ieta->size()/2],ed.tdc->at(i1)[2] + tdc1values[i1-ed.ieta->size()/2]); //Fills Qtot vs TDC TS2
                        PL[plind].QtotvsTDC2->Fill(Qtot+ qtots[i1-ed.ieta->size()/2],ed.tdc->at(i1)[3] + tdc2values[i1-ed.ieta->size()/2]); //Fills Qtot vs TDC TS3
                        PL[plind].TDC1vsTDC2->Fill(ed.tdc->at(i1)[2] + tdc1values[i1-ed.ieta->size()/2] ,ed.tdc->at(i1)[3] + tdc2values[i1-ed.ieta->size()/2] ); //Fills TDC1 vs TDC2
                        PL[plind].Q1vsTDC->Fill(NQ1,ed.tdc->at(i1)[2] + tdc1values[i1-ed.ieta->size()/2]); //Fills ADC vs TDC histogram for Q1
                        PL[plind].Q2vsTDC->Fill(NQ2,ed.tdc->at(i1)[3] + tdc2values[i1-ed.ieta->size()/2]); //Fills ADC vs TDC histogram for Q2
                        PL[plind].Q2_Q1vsTDC1->Fill(NQ2/NQ1,ed.tdc->at(i1)[2]+ tdc1values[i1-ed.ieta->size()/2]); //Fills Q2/Q1 vs TDC TS2
                        PL[plind].Q2_Q1vsTDC2->Fill(NQ2/NQ1,ed.tdc->at(i1)[3]+ tdc2values[i1-ed.ieta->size()/2]); //Fills Q2/Q1 vs TDC TS3
                        
                        //PMT Channels comparison histograms
                        //PL[plind].q1_1vsq1_2->Fill(q1values[i1-ed.ieta->size()/2], QQ[2]);
                        //PL[plind].q2_1vsq2_2->Fill(q2values[i1-ed.ieta->size()/2], QQ[3]);
                        PL[plind].tdc1_1vstdc1_2->Fill(tdc1values[i1-ed.ieta->size()/2], ed.tdc->at(i1)[2]);
                        PL[plind].tdc2_1vstdc2_2->Fill(tdc2values[i1-ed.ieta->size()/2],ed.tdc->at(i1)[3]);
                        
                        
                    }
                } else {
                    q1s[i1] = -1;
                }
            }
            
            if(e3){ //THE LATTER ONE
                
                for(int i2=0;i2<nTS;i2++) {
                    QQ[i2]=adc2fC_QIE10[ed.pulse->at(i1)[i2]];
                }
                
                //Cuts for invalid TDC values:
                bool f1, f2, f ;
                for( int chan = 0 ; chan < 56 ; chan++) {
                    if ((ed.ieta->at(i1)==RADDAM_CH[chan].ieta) && (ed.iphi->at(i1)==RADDAM_CH[chan].iphi)){
                        f1 = ((ed.tdc->at(i1)[3] > tdcCuts3_TS3[chan][1]) || (ed.tdc->at(i1)[3] < tdcCuts3_TS3[chan][0]));
                        if(RADDAM_CH[chan].ieta == 41 && RADDAM_CH[chan].iphi == 35) {
                            f2 = (ed.tdc->at(i1)[2] > 28) || (ed.tdc->at(i1)[2] < 22);
                        } else if(RADDAM_CH[chan].ieta == 30 && RADDAM_CH[chan].iphi == 1){
                            f2 = (ed.tdc->at(i1)[2] > 26) || (ed.tdc->at(i1)[2] < 20);
                        } else {
                            f2 = false;
                        }
                        f = f1 || f2;
                        break;
                    }
                }
                if(!f) {
                    if(i1 < ed.ieta->size()/2){
                        
                        tdc1values_3[i1] = ed.tdc->at(i1)[2];
                        tdc2values_3[i1] = ed.tdc->at(i1)[3];
                        q1s_3[i1] = QQ[2] - (QQ[0]+QQ[1])/2 ;
                        q2s_3[i1] = QQ[3] - (QQ[0]+QQ[1])/2 ;
                        
                    } else if (q1s_3[i1-ed.ieta->size()/2] != -1) {
                        
                        //Normalized Q1 and Q2 values for 2 channels
                        NQ1 = QQ[2] - (QQ[0]+QQ[1])/2 + q1s_3[i1-ed.ieta->size()/2];
                        NQ2 = QQ[3] - (QQ[0]+QQ[1])/2 + q2s_3[i1-ed.ieta->size()/2];
                        
                        PL[plind].Q2overQ1_third->Fill(NQ2/NQ1);
                        PL[plind].tdc1_1vstdc1_2_third->Fill(tdc1values_3[i1-ed.ieta->size()/2], ed.tdc->at(i1)[2]);
                        PL[plind].tdc2_1vstdc2_2_third->Fill(tdc2values_3[i1-ed.ieta->size()/2],ed.tdc->at(i1)[3]);
                    }
                }  else {
                    q1s_3[i1] = -1;
                }
                
            }
        }
    }
    //END OF EVENT SELECTION AND FILLING THE HISTOGRAMS
    
	int nn=0;
	int iside=0;int iquad=0;
	float fnevt=((float)(ev2-ev1));
	
	TFile* outfile=new TFile("RadDam.root","recreate");
	
	for(int i2=0;i2<PL.size();i2++) {
		for(int is1=0;is1<nTS;is1++) {
			if(PL[i2].PT[is1][2]>0) {
                PL[i2].PT[is1][0]/=PL[i2].PT[is1][2]; //Pulse is divided by norm
                PL[i2].PT[is1][1]/=PL[i2].PT[is1][2]; //Tdc is divided by norm
            }
			PL[i2].p->SetBinContent(is1+1,PL[i2].PT[is1][0]); //Pulse Shapes is initiated
			PL[i2].t->SetBinContent(is1+1,PL[i2].PT[is1][1]); //TDC Shapes is initiated
		}
		PL[i2].nbadcapid/=fnevt;
	}
    
	{
        //HISTOGRAM FIT, DRAW AND WRITE
		string pmtnames[24]={"A2","A4","A6","A8","A1","A3","A5","A7","B2","B4","B6","B8","B1","B3","B5","B7","C2","C4","C6","C8","C1","C3","C5","C7"};
		string chnames[2]={"1+2","3+4"};
		string side[2]={"P","M"};
		
        int dividexaxes = 4;
        int divideyaxes = 4;
        int pagenumber = 4;
        
        string histonames[]= {"PulseShapes.pdf","TDCShapes.pdf","Q2_Q1vsTDC1.pdf","Q2_Q1vsTDC2.pdf","Q1vsQ2.pdf","Q1vsTDC1.pdf","Q2vsTDC2.pdf","QTotal.pdf","Q2overQ1.pdf","Q1.pdf","Q2.pdf","TDC1.pdf","TDC2.pdf","QtotvsTDC1.pdf","QtotvsTDC2.pdf", "Qpedestal.pdf","TDC1vsTDC2.pdf", "TDC1-1vsTDC1-2.pdf", "TDC2-1vsTDC2-2.pdf", "Q2_Q1vsQ1.pdf" , "Q2_Q1vsQ2.pdf" , "Q2_Q1vsQtot.pdf", "Q2_Q1vsQsum.pdf", "Q2overQ1first.pdf" , "Q2overQ1third.pdf", "TDC1comp_first.pdf", "TDC2comp_first.pdf", "TDC1comp_third.pdf", "TDC2comp_third.pdf" };
        
        int histonumber =  sizeof(histonames)/sizeof(histonames[0]);

        for( int n=0 ; n<histonumber ; n++){
            
            int l = histonames[n].length();
            
            char histoname1[20];
            char histoname[20] ;
            histonames[n].copy(histoname,l,0);
            
            for(int foo=l; foo<20;foo++){
                histoname[foo]= '\0';
            }
            
            TCanvas* canvas[pagenumber];
            
            for(int x=0;x<pagenumber;x++){
                char name[100];
                sprintf(name, "cc%d%d", n,x);
                canvas[x] = new TCanvas(name,name,5000,6000);
                gStyle->SetOptStat(0);
                gStyle->SetTitleFontSize(0.1);
                canvas[x]->Divide(dividexaxes,divideyaxes,0.001,0.001);
            }
            
            int currentpage = 0; int currentcanv = 1;
            
            for(int i1=0;i1<56;i1++) {
                        
                    plind=-1;
                    for(int ik1=0;ik1<PL.size();ik1++)
                    {
                        MI=PL[ik1].mapind;
                        if(SEM[MI].ieta==RADDAM_CH[i1].ieta && SEM[MI].iphi==RADDAM_CH[i1].iphi)
                        {
                            plind=ik1;
                            break;
                        }
                    }
        
                    canvas[currentpage]->cd(currentcanv);
                    currentcanv++;
                    if(currentcanv> divideyaxes*dividexaxes ) {
                        currentpage++;
                        currentcanv = 1;
                    }
                    gPad-> SetLogy(0);
                        
                    if(n==0){ //Pulse Shapes Histogram
                        
                        PL[plind].p->Draw("hist");
                        PL[plind].p->GetYaxis()->SetRangeUser(-10.,(PL[plind].p->GetBinContent(PL[plind].p->GetMaximumBin()))*1.5);
                        PL[plind].p->Write();
                            
                    } else if(n==1){ //Tdc histogram
                        
                        PL[plind].t->Draw();
                        PL[plind].t->GetYaxis()->SetRangeUser(0.,75.);
                        PL[plind].t->Write();
                            
                    } else if(n==2){ //Q2/Q1 vs TDC TS2 histogram
                        
                        PL[plind].Q2_Q1vsTDC1->Draw("COLZ");
                        PL[plind].Q2_Q1vsTDC1->Write();
                        
                    } else if(n==3){ //Q2/Q1 vs TDC TS3 histogram
                        
                        PL[plind].Q2_Q1vsTDC2->Draw("COLZ");
                        PL[plind].Q2_Q1vsTDC2->Write();
                            
                    } else if(n==4){ //Q1 vs Q2 histogram
                        
                        PL[plind].Q1vsQ2->Draw("COLZ");
                        PL[plind].Q1vsQ2->Write();
                            
                    } else if(n==5){ //Q1 vs TDC 1 histogram
                        
                        PL[plind].Q1vsTDC->Draw("COLZ");
                        PL[plind].Q1vsTDC->Write();
                            
                    } else if(n==6){ //Q2 vs TDC 2 histogram
                        
                        PL[plind].Q2vsTDC->Draw("COLZ");
                        PL[plind].Q2vsTDC->Write();
                            
                    } else if(n==7){ //Qtot histogram
                        
                        gStyle->SetOptStat(1101);
                        gPad-> SetLogy(1);
                        PL[plind].Qtot->GetYaxis()->SetRangeUser(1,(PL[plind].Qtot->GetBinContent(PL[plind].Qtot->GetMaximumBin()))*1.5);
                        PL[plind].Qtot->Draw("hist");
                        PL[plind].Qtot->Write();
                            
                    } else if(n==8){ //Q2/Q1 histogram
                        
                        gStyle->SetOptStat(1111);
                        PL[plind].Q2overQ1->GetYaxis()->SetRangeUser(-1.,(PL[plind].Q2overQ1->GetBinContent(PL[plind].Q2overQ1->GetMaximumBin()))*1.5);
                        PL[plind].Q2overQ1->Fit("gaus");
                        if ( PL[plind].Q2overQ1->GetEntries() != 0) {
                            PL[plind].q2overq1 = PL[plind].Q2overQ1->GetFunction("gaus")->GetParameter(1);
                            PL[plind].q2overq1_error = PL[plind].Q2overQ1->GetFunction("gaus")->GetParError(1);
                            //cout << "ieta: " << RADDAM_CH[plind].ieta << " iphi: "<< RADDAM_CH[plind].iphi<< " " <<PL[plind].q2overq1 << " +-" << PL[plind].q2overq1_error << endl;
                        } else {
                            PL[plind].q2overq1 = 0;
                            PL[plind].q2overq1_error = 0;
                        }
                        gStyle->SetOptFit(0111);
                        PL[plind].Q2overQ1->Draw();
                        PL[plind].Q2overQ1->Write();
                        
                    } else if(n==9){ //Q1 histogram
                        
                        gStyle->SetOptStat(1101);
                        gPad-> SetLogy(1);
                        PL[plind].Q1->GetYaxis()->SetRangeUser(1,(PL[plind].Q1->GetBinContent(PL[plind].Q1->GetMaximumBin()))*1.5);
                        PL[plind].Q1->Draw();
                        PL[plind].Q1->Write();
                
                    } else if(n==10){ //Q2 histogram
                       
                        gStyle->SetOptStat(1101);
                        gPad-> SetLogy(1);
                        PL[plind].Q2->GetYaxis()->SetRangeUser(1,(PL[plind].Q2->GetBinContent(PL[plind].Q2->GetMaximumBin()))*1.5);
                        PL[plind].Q2->Draw();
                        PL[plind].Q2->Write();
                
                    } else if(n==11){ //TDC1 histogram
                        
                        PL[plind].TDC_TS2->Draw("hist");
                        PL[plind].TDC_TS2->Write();
                        
                    } else if(n==12){ //TDC2 histogram
                        
                        PL[plind].TDC_TS3->Draw("hist");
                        PL[plind].TDC_TS3->Write();
                        
                    } else if(n==13){ //Qtot vs TDC TS2 histogram
                        
                        PL[plind].QtotvsTDC1->Draw("COLZ");
                        PL[plind].QtotvsTDC1->Write();
                        
                    } else if(n==14){ //Qtot vs TDC TS3 histogram
                        PL[plind].QtotvsTDC2->Draw("COLZ");
                        PL[plind].QtotvsTDC2->Write();
                        
                    } else if(n==15){ // Qpedestal histogram
                        
                        gStyle->SetOptStat(1101);
                        gPad-> SetLogy(1);
                        PL[plind].Qpedest->GetYaxis()->SetRangeUser(1,(PL[plind].Qpedest->GetBinContent(PL[plind].Qpedest->GetMaximumBin()))*1.5);
                        PL[plind].Qpedest->Draw();
                        PL[plind].Qpedest->Write();
                        
                    } else if(n == 16){ //TDC1 vs TDC2 Histogram
                        
                        PL[plind].TDC1vsTDC2->Draw("COLZ");
                        PL[plind].TDC1vsTDC2->Write();
                        
                    } else if(n == 17){ //TDC1-1 vs TDC1-2 Histogram
                        
                        PL[plind].tdc1_1vstdc1_2->Draw("COLZ");
                        PL[plind].tdc1_1vstdc1_2->Write();
                        
                    } else if(n == 18){ //TDC2-1 vs TDC2-2 Histogram
                        
                        PL[plind].tdc2_1vstdc2_2->Draw("COLZ");
                        PL[plind].tdc2_1vstdc2_2->Write();
                        
                    } else if(n == 19){ //Q2/Q1 vs Q1 histogram
                        
                        PL[plind].Q2_Q1vsQ1->Draw("COLZ");
                        PL[plind].Q2_Q1vsQ1->Write();
                        
                    } else if(n == 20){ //Q2/Q1 vs Q2 histogram
                        
                        PL[plind].Q2_Q1vsQ2->Draw("COLZ");
                        PL[plind].Q2_Q1vsQ2->Write();
                        
                    } else if(n == 21){ //Q2/Q1 vs Qtot histogram
                        
                        PL[plind].Q2_Q1vsQtot->Draw("COLZ");
                        PL[plind].Q2_Q1vsQtot->Write();
                        
                    } else if(n == 22){ //Q2/Q1 vs Qsum histogram
                        
                        PL[plind].Q2_Q1vsQsum->Draw("COLZ");
                        PL[plind].Q2_Q1vsQsum->Write();
                        
                    } else if(n == 23){ //Q2/Q1  former
                        
                        gStyle->SetOptStat(1111);
                        PL[plind].Q2overQ1_first->GetYaxis()->SetRangeUser(-1.,(PL[plind].Q2overQ1_first->GetBinContent(PL[plind].Q2overQ1_first->GetMaximumBin()))*1.5);
                        PL[plind].Q2overQ1_first->Fit("gaus");
                        if ( PL[plind].Q2overQ1_first->GetEntries() != 0) {
                            cout << "plind: "<<plind<< " ,number of entries of 1st Q2/Q1: " << PL[plind].Q2overQ1_first->GetEntries() << endl;
                            PL[plind].q2overq1_fi = PL[plind].Q2overQ1_first->GetFunction("gaus")->GetParameter(1);//PROBLEM HERE(?)
                        } else {
                            PL[plind].q2overq1_fi = 0;
                        }
                        gStyle->SetOptFit(0111);
                        PL[plind].Q2overQ1_first->Draw();
                        PL[plind].Q2overQ1_first->Write();
                        
                    } else if(n == 24){ //Q2/Q1 latter
                        
                        gStyle->SetOptStat(1111);
                        PL[plind].Q2overQ1_third->GetYaxis()->SetRangeUser(-1.,(PL[plind].Q2overQ1_third->GetBinContent(PL[plind].Q2overQ1_third->GetMaximumBin()))*1.5);
                        PL[plind].Q2overQ1_third->Fit("gaus");
                        if ( PL[plind].Q2overQ1_third->GetEntries() != 0) {
                            PL[plind].q2overq1_th = PL[plind].Q2overQ1_third->GetFunction("gaus")->GetParameter(1);
                        } else {
                            PL[plind].q2overq1_th = 0;
                        }
                        gStyle->SetOptFit(0111);
                        PL[plind].Q2overQ1_third->Draw();
                        PL[plind].Q2overQ1_third->Write();
                        
                    }  else if(n == 25){ //TDC1-1 vs TDC1-2 Histogram former
                        
                        PL[plind].tdc1_1vstdc1_2_first->Draw("COLZ");
                        PL[plind].tdc1_1vstdc1_2_first->Write();
                        
                    } else if(n == 26){ //TDC2-1 vs TDC2-2 Histogram former
                        
                        PL[plind].tdc2_1vstdc2_2_first->Draw("COLZ");
                        PL[plind].tdc2_1vstdc2_2_first->Write();
                        
                    }  else if(n == 27){ //TDC1-1 vs TDC1-2 Histogram latter
                        
                        PL[plind].tdc1_1vstdc1_2_third->Draw("COLZ");
                        PL[plind].tdc1_1vstdc1_2_third->Write();
                        
                    } else if(n == 28){ //TDC2-1 vs TDC2-2 Histogram latter
                        
                        PL[plind].tdc2_1vstdc2_2_third->Draw("COLZ");
                        PL[plind].tdc2_1vstdc2_2_third->Write();
                        
                    }
            }
            for(int x=0;x<pagenumber; x++){

                char dummy[10];
            
                if( x==0 ) sprintf(dummy,"("); //First page of pdf file
                else if(x==pagenumber-1) sprintf(dummy,")"); //Last page of pdf file
                else sprintf(dummy,"");
                
                sprintf(histoname1,"%s%s",histoname,dummy);
                canvas[x]->SaveAs(histoname1);
                delete canvas[x];
            }
            sprintf(hname,"mv %s ../Plots/%d",histoname,RunNo);system(hname);

        }
        //END OF HISTOGRAM FIT, DRAW AND WRITE
        
        for(int ik1=0;ik1<PL.size();ik1++) {
            int max=0;float qfmax=0.;
            for(int ik2=0;ik2<32;ik2++) {
                if(PL[ik1].qf[ik2]>qfmax){
                    qfmax=PL[ik1].qf[ik2];
                    max=ik2;
                }
            }
        }
        
        for(int ik1=0;ik1<PL.size();ik1++) {
            PL[ik1].p=0;PL[ik1].pn=0;PL[ik1].t=0;
        }
    }
    
    //Q2/Q1 FILE WRITE
    char filename[100]; char command[100];
    
    fstream results;
    results.open("results.txt",fstream::out);
    
    if(!results.is_open()) {
        cout << "error"<<endl;
        return 0;
    }
    
    for(int i1=0;i1<56;i1++) {
            int index =-1;
            for(int ik1=0;ik1<PL.size();ik1++) {
                MI=PL[ik1].mapind;
                if(SEM[MI].ieta==RADDAM_CH[i1].ieta && SEM[MI].iphi==RADDAM_CH[i1].iphi) {
                    index=ik1;
                    break;
                }
            }
            if(i1 == 55) results << SEM[MI].ieta << " " <<SEM[MI].iphi <<" "<< PL[index].q2overq1_fi <<" "<< PL[index].q2overq1_th << " "<< PL[index].q2overq1<< " "<< PL[index].q2overq1_error<< " " << PL[index].qped1<< " " << PL[index].qped2 ;
            else results << SEM[MI].ieta << " " <<SEM[MI].iphi <<" "<< PL[index].q2overq1_fi <<" "<< PL[index].q2overq1_th<< " " << PL[index].q2overq1<< " "<< PL[index].q2overq1_error<< " " << PL[index].qped1<< " " << PL[index].qped2 << endl;
    }
    sprintf(command, "mv results.txt ../Data/results_%d.txt", RunNo); system(command);
    results.close();
    //END
    
    sprintf(hname,"mv RadDam.root ../Histos/RadDam_%d.root",RunNo);system(hname);
    outfile->Close();
}

/*
 Analyzes all runs taken so far, draws Q2/Q1 plots for each channel, saves it into Ratios.pdf
*/
void plotratios(){
    
    char hname[500]; char graphname[100];
    
    vector<channel> ChannelList;
    int numberofchannels= 56;
    
    //Initializes channel list array
    for(int i=0; i<numberofchannels; i++ ){
        channel ch;
        ch.index = i; // Corresponds to the channel RADDAM_CH[i]
        ChannelList.push_back(ch);
    }
    
    //Opens run list file
    ifstream runlist("RunList.csv");
    if (!runlist.is_open()){
        cout << "Run list cannot be openned!" << endl;
        return;
    }
    
    int runno, runname; float datetime;
    string a, b, c, d, e, f; //Consecutively: Run no(1,2,3,...) , run number(321446,...), date, time(hour), date+time, date and time converted to a number
    
    //Reads the run list and initializes run data, stores in allRuns code
    while (runlist.good()) {
        
        getline(runlist,a, ','); getline(runlist, b,','); getline(runlist, c,',');
        getline(runlist, d,','); getline(runlist, e,','); getline(runlist, f,'\n');
        if (a.empty()) break;
        
        runno = stoi(a); runname =stoi(b); datetime=stoi(f);
        
        singleRun.runno = runno;
        singleRun.runname = runname;
        singleRun.datetime = datetime;
        
        allRuns.push_back(singleRun);
    }
    runlist.close();
    
    int runs = allRuns.size();
    float refPdstl[numberofchannels][2] = {{0}};//Use averages of all pedestals instead of first one
    float pdstl[runs][numberofchannels][2];
    
    
    //Pedestal values of all channels, normalized to the average of all, in a single graph
    TGraph* normPed = new TGraph();
    normPed->SetName("Normalized Pedestals of All Channels");normPed->SetTitle("Normalized Pedestals of All Channels");
    normPed->SetMarkerStyle(20);normPed->SetMarkerColor(4);
    normPed->GetYaxis()->SetTitle("Normalized Pedestals"); normPed->GetXaxis()->SetTitle("Days");
    
    //Pedestal values of all channels in a single graph
    TGraph* pedestalAll = new TGraph();
    pedestalAll->SetName("Pedestals of All Channels");pedestalAll->SetTitle("Pedestals of All Channels");
    pedestalAll->SetMarkerStyle(20);pedestalAll->SetMarkerColor(3);
    pedestalAll->GetYaxis()->SetRangeUser(-5,5);  //pedestalAll->GetXaxis()->SetRangeUser(0,150);
    pedestalAll->GetYaxis()->SetTitle("Pedestals"); pedestalAll->GetXaxis()->SetTitle("Days");
    
    //Traces all runs, opens the data file, takes channel info and ratio value for this run
    for(int i=0;i<allRuns.size();i++) {
        
        char filelocation[100];
        sprintf(filelocation, "../Data/results_%d.txt",allRuns[i].runname);
        ifstream run(filelocation);
        if(run.is_open()){
            cout<< "file name: " << filelocation << endl;
            
            int ieta, iphi; float ratio1, ratio2, ratio3, rat2error, pedes1, pedes2;
            
            while(!run.eof()){
                run >> ieta >> iphi >> ratio1 >> ratio3 >> ratio2 >> rat2error >> pedes1 >> pedes2;
        
                pedes1 /= 5000; pedes2 /=5000;
                
                //Finds the correct channel
                for(int j=0;j<numberofchannels;j++) {
                    
                    if (ieta == RADDAM_CH[j].ieta && iphi == RADDAM_CH[j].iphi) {
                        
                        runProperties r;
                        r.ratioValue = ratio2;
                        r.ratioValue1 = ratio1;
                        r.ratioValue3 = ratio3;
                        r.ratioError = rat2error;
                        r.pedestalValue1 = pedes1;
                        r.pedestalValue2 = pedes2;
                        r.datetime = allRuns[i].datetime;
                        
                        ChannelList[j].runForOneChannel.push_back(r);
                        
                        refPdstl[j][0] += pedes1;
                        refPdstl[j][1] += pedes2;
                        pdstl[i][j][0] = pedes1;
                        pdstl[i][j][1] = pedes2;
                        
                        //normPed->SetPoint(normPed->GetN(),allRuns[i].datetime, pedes1/refPdstl[j][0]);
                        //normPed->SetPoint(normPed->GetN(),allRuns[i].datetime, pedes2/refPdstl[j][1]);
                        pedestalAll->SetPoint(pedestalAll->GetN(), allRuns[i].datetime, pedes1);
                        pedestalAll->SetPoint(pedestalAll->GetN(), allRuns[i].datetime, pedes2);
                    }
                }
            }
            run.close();
        } else {
            cout << "Run number " << allRuns[i].runname << " file does not exists!" << endl;
            break;
        }
    }
    
    for(int m=0; m<runs ; m++){
        for(int n=0; n<numberofchannels; n++){
            if( m == 0 ){
                refPdstl[n][0] /= runs; refPdstl[n][1] /= runs;
            }
            float l1 = pdstl[m][n][0]/refPdstl[n][0]; float l2 = pdstl[m][n][1]/refPdstl[n][1];
            //cout << "run " << m << " channel " << n << " entry1 " << l1  << "  = " << pdstl[m][n][0] << " / " << refPdstl[n][0] << endl;
            //cout << "run " << m << " channel " << n << " entry2 " << l2  << "  = " << pdstl[m][n][1] << " / " << refPdstl[n][1] << endl;
            normPed->SetPoint(normPed->GetN(),allRuns[m].datetime, l1);
            normPed->SetPoint(normPed->GetN(),allRuns[m].datetime, l2);
        }
    }

    //Creates the graph and canvas to display it
    TFile* outfile=new TFile("Ratios.root","recreate");
    
    //ALL PEDESTALS GRAPH
    TCanvas* canv = new TCanvas("page","page",5000,6000);
    canv->Divide(1,2,0.001,0.001);
    pedestalAll->SetMarkerStyle(21);
    canv->cd(1);
    pedestalAll->Draw("AP");
    canv->cd(2);
    normPed->GetYaxis()->SetRangeUser(0,2);  normPed->GetXaxis()->SetRangeUser(0,150);
    normPed->SetMarkerStyle(19);
    normPed->Draw("AP");
    canv->SaveAs("AllPedestals.pdf");
    delete canv;
    //END OF ALL PEDESTALS GRAPH
    
    //RATIOS HISTOGRAMS DRAW
    int dividexaxes = 4;
    int divideyaxes = 4;
    int pagenumber = 4;
    string histonames[]= {"Ratios.pdf","Ratios_Superimposed.pdf","Pedestals1.pdf", "Pedestals2.pdf", "NormPedestals1.pdf", "NormPedestals2.pdf"};
    
    int histonumber =  sizeof(histonames)/sizeof(histonames[0]);
    float reference1[ChannelList.size()] = {0}; float reference2[ChannelList.size()] = {0};
    
    for( int n=0 ; n<histonumber ; n++){
        
        int l = histonames[n].length();
        
        char histoname1[20];
        char histoname[20] ;
        histonames[n].copy(histoname,l,0);
        
        for(int foo=l; foo<20;foo++){
            histoname[foo]= '\0';
        }
    
        TCanvas* canvas[pagenumber];
        
        for(int x=0;x<pagenumber;x++){
            char name[100];
            sprintf(name, "page%d%d",x,n);
            canvas[x] = new TCanvas(name,name,5000,6000);
            gStyle->SetOptStat(0);
            gStyle->SetTitleFontSize(0.1);
            canvas[x]->Divide(dividexaxes,divideyaxes,0.001,0.001);
        }
        int currentpage = 0; int currentcanv = 1;
        
        for(int k = 0; k<ChannelList.size(); k++){
            
            int numberofruns = ChannelList[k].runForOneChannel.size();

            float dates[numberofruns], values[numberofruns], values1[numberofruns], values3[numberofruns] , dateerrors[numberofruns], valueerrors[numberofruns];
        
            if(n==0) {
                
                float A = ChannelList[k].runForOneChannel[0].ratioValue;
                float DA = ChannelList[k].runForOneChannel[0].ratioError;
                
                for(int l =0; l<numberofruns; l++){
                    
                    float B = ChannelList[k].runForOneChannel[l].ratioValue;
                    float DB = ChannelList[k].runForOneChannel[l].ratioError;
                    
                    dates[l] = ChannelList[k].runForOneChannel[l].datetime;
                    dateerrors[l] = 0;
                    values[l] = B/A; //Normalized ratios
                    valueerrors[l] = (B/A)*sqrt((DA/A)*(DA/A) + (DB/B)*(DB/B)); //Uncertainity for normalized ratios
                    //cout << "Channel number: "<< k << " run number: " << l << " value: " << values[l] << " error : " << valueerrors[l] << endl;
                }
                canvas[currentpage]->cd(currentcanv);
                currentcanv++;
                if(currentcanv> divideyaxes*dividexaxes ) {
                    currentpage++;
                    currentcanv = 1;
                }

                char title[100];
                sprintf(title, "(%d %d)", RADDAM_CH[k].ieta, RADDAM_CH[k].iphi);
                
                ChannelList[k].ratioPlot = new TGraphErrors(numberofruns,dates,values,dateerrors,valueerrors);
                ChannelList[k].ratioPlot->SetTitle(title);
                ChannelList[k].ratioPlot->GetYaxis()->SetRangeUser(0.8,1.2);  ChannelList[k].ratioPlot->GetXaxis()->SetRangeUser(0,150);
                ChannelList[k].ratioPlot->GetYaxis()->SetTitle("Normalized Ratio (Q2/Q1)"); ChannelList[k].ratioPlot->GetXaxis()->SetTitle("Days");
                ChannelList[k].ratioPlot->Draw("A*");
                ChannelList[k].ratioPlot->Write();
                
            } else if(n==1) {
                
                float percentageSTD[numberofruns]; float average, std, waverage;
                
                float A1 = ChannelList[k].runForOneChannel[0].ratioValue1;
                float A2 = ChannelList[k].runForOneChannel[0].ratioValue;
                float A3 = ChannelList[k].runForOneChannel[0].ratioValue3;
                
                for(int l =0; l<numberofruns; l++){
                    
                    float B1 = ChannelList[k].runForOneChannel[l].ratioValue1;
                    float B2 = ChannelList[k].runForOneChannel[l].ratioValue;
                    float B3 = ChannelList[k].runForOneChannel[l].ratioValue3;

                    dates[l] = ChannelList[k].runForOneChannel[l].datetime;
                    values[l] = B2/A2; //Normalized ratios
                    values1[l] = B1/A1; //Normalized ratios
                    values3[l] = B3/A3; //Normalized ratios
                    average = (B1/A1 + B2/A2 + B3/A3)/3;
                    std = sqrt(((B1/A1-average)*(B1/A1-average) + (B2/A2-average)*(B2/A2-average) + (B3/A3-average)*(B3/A3-average))/2);
                    percentageSTD[l] = std/average;

                }
                canvas[currentpage]->cd(currentcanv);

                char title[100];
                sprintf(title, "(%d %d)", RADDAM_CH[k].ieta, RADDAM_CH[k].iphi);
                
                ChannelList[k].ratioPlot_1 = new TGraph(numberofruns,dates,values1); ChannelList[k].ratioPlot_1->SetTitle(title);
                ChannelList[k].ratioPlot_2 = new TGraph(numberofruns,dates,values);
                ChannelList[k].ratioPlot_3 = new TGraph(numberofruns,dates,values3);
                
                ChannelList[k].ratioPlot_1->SetMarkerColor(7);
                ChannelList[k].ratioPlot_1->GetYaxis()->SetRangeUser(0.8,1.2);  ChannelList[k].ratioPlot_1->GetXaxis()->SetRangeUser(0,150);
                ChannelList[k].ratioPlot_1->Draw("A*");
                
                ChannelList[k].ratioPlot_2->SetLineWidth(3);
                ChannelList[k].ratioPlot_2->GetYaxis()->SetRangeUser(0.8,1.2);  ChannelList[k].ratioPlot_2->GetXaxis()->SetRangeUser(0,150);
                ChannelList[k].ratioPlot_2->SetMarkerStyle(21);
                ChannelList[k].ratioPlot_2->SetMarkerColor(6);
                ChannelList[k].ratioPlot_2->Draw("P");
                
                ChannelList[k].ratioPlot_3->SetLineWidth(3);
                ChannelList[k].ratioPlot_3->GetYaxis()->SetRangeUser(0.8,1.2);  ChannelList[k].ratioPlot_3->GetXaxis()->SetRangeUser(0,150);
                ChannelList[k].ratioPlot_3->SetMarkerStyle(10);
                ChannelList[k].ratioPlot_3->SetMarkerColor(4);
                ChannelList[k].ratioPlot_3->Draw("P");

                currentcanv++;
                if(currentcanv> divideyaxes*dividexaxes ) {
                    currentpage++;
                    currentcanv = 1;
                }
                
                float wi = 0;
                for(int l =0; l<numberofruns; l++){
                    wi += percentageSTD[l];
                }
                waverage = wi/numberofruns;
                cout << "Channel " << RADDAM_CH[k].ieta << " " << RADDAM_CH[k].iphi << " systematic error " << waverage << endl;
                
            }   else if(n==2){
                
                for(int l =0; l<numberofruns; l++){
                    dates[l] = ChannelList[k].runForOneChannel[l].datetime;
                    values[l] = ChannelList[k].runForOneChannel[l].pedestalValue1;
                    reference1[k] += values[l];
                }
                canvas[currentpage]->cd(currentcanv);
                currentcanv++;
                if(currentcanv> divideyaxes*dividexaxes ) {
                    currentpage++;
                    currentcanv = 1;
                }

                char title[100];
                sprintf(title, "(%d %d %d)", RADDAM_CH[k].ieta, RADDAM_CH[k].iphi, RADDAM_CH[k].depth);
                
                ChannelList[k].pedestalGraph1 = new TGraph(numberofruns,dates,values);
                ChannelList[k].pedestalGraph1->SetTitle(title);
                gStyle->SetOptStat(1101);
                ChannelList[k].pedestalGraph1->GetYaxis()->SetRangeUser(-5,14);  ChannelList[k].pedestalGraph1->GetXaxis()->SetRangeUser(0,150);
                ChannelList[k].pedestalGraph1->GetYaxis()->SetTitle("Pedestals"); ChannelList[k].pedestalGraph1->GetXaxis()->SetTitle("Days");
                ChannelList[k].pedestalGraph1->Draw("A*");
                ChannelList[k].pedestalGraph1->Write();
                
            } else if(n==3){
                
                for(int l =0; l<numberofruns; l++){
                    dates[l] = ChannelList[k].runForOneChannel[l].datetime;
                    values[l] = ChannelList[k].runForOneChannel[l].pedestalValue2;
                    reference2[k] += values[l];
                }
                canvas[currentpage]->cd(currentcanv);
                currentcanv++;
                if(currentcanv> divideyaxes*dividexaxes ) {
                    currentpage++;
                    currentcanv = 1;
                }
                
                char title[100];
                sprintf(title, "(%d %d %d)", RADDAM_CH[k].ieta, RADDAM_CH[k].iphi, RADDAM_CH[k].depth+2 );
                
                ChannelList[k].pedestalGraph2 = new TGraph(numberofruns,dates,values);
                ChannelList[k].pedestalGraph2->SetTitle(title);
                gStyle->SetOptStat(1101);
                ChannelList[k].pedestalGraph2->GetYaxis()->SetRangeUser(-5,14);  ChannelList[k].pedestalGraph2->GetXaxis()->SetRangeUser(0,150);
                ChannelList[k].pedestalGraph2->GetYaxis()->SetTitle("Pedestals"); ChannelList[k].pedestalGraph2->GetXaxis()->SetTitle("Days");
                ChannelList[k].pedestalGraph2->Draw("A*");
                ChannelList[k].pedestalGraph2->Write();
                
            } else if (n == 4){
                reference1[k] /= numberofruns;
                //cout << k << " " << reference1[k] << endl;
                for(int l =0; l<numberofruns; l++){
                    dates[l] = ChannelList[k].runForOneChannel[l].datetime;
                    values[l] = (ChannelList[k].runForOneChannel[l].pedestalValue1)/reference1[k];
                }
                canvas[currentpage]->cd(currentcanv);
                currentcanv++;
                if(currentcanv> divideyaxes*dividexaxes ) {
                    currentpage++;
                    currentcanv = 1;
                }
                
                char title[100];
                sprintf(title, "(%d %d %d)", RADDAM_CH[k].ieta, RADDAM_CH[k].iphi, RADDAM_CH[k].depth);
                
                ChannelList[k].pedestalNormalized1 = new TGraph(numberofruns,dates,values);
                ChannelList[k].pedestalNormalized1->SetTitle(title);
                ChannelList[k].pedestalNormalized1->GetYaxis()->SetRangeUser(0,2);  ChannelList[k].pedestalNormalized1->GetXaxis()->SetRangeUser(0,150);
                ChannelList[k].pedestalNormalized1->GetYaxis()->SetTitle("Normalized Pedestals"); ChannelList[k].pedestalNormalized1->GetXaxis()->SetTitle("Days");
                ChannelList[k].pedestalNormalized1->Draw("A*");
                ChannelList[k].pedestalNormalized1->Write();
            } else if (n == 5){
                reference2[k] /= numberofruns;
                //cout << k << " " << reference1[k] << endl;
                for(int l =0; l<numberofruns; l++){
                    dates[l] = ChannelList[k].runForOneChannel[l].datetime;
                    values[l] = (ChannelList[k].runForOneChannel[l].pedestalValue2)/reference2[k];
                }
                canvas[currentpage]->cd(currentcanv);
                currentcanv++;
                if(currentcanv> divideyaxes*dividexaxes ) {
                    currentpage++;
                    currentcanv = 1;
                }
                
                char title[100];
                sprintf(title, "(%d %d %d)", RADDAM_CH[k].ieta, RADDAM_CH[k].iphi, RADDAM_CH[k].depth+2);
                
                ChannelList[k].pedestalNormalized2 = new TGraph(numberofruns,dates,values);
                ChannelList[k].pedestalNormalized2->SetTitle(title);
                ChannelList[k].pedestalNormalized2->GetYaxis()->SetRangeUser(0,2);  ChannelList[k].pedestalNormalized2->GetXaxis()->SetRangeUser(0,150);
                ChannelList[k].pedestalNormalized2->GetYaxis()->SetTitle("Normalized Pedestals"); ChannelList[k].pedestalNormalized2->GetXaxis()->SetTitle("Days");
                ChannelList[k].pedestalNormalized2->Draw("A*");
                ChannelList[k].pedestalNormalized2->Write();
            }
            
        }
        for(int x=0;x<pagenumber; x++){
            
            char dummy[10];
            
            if( x==0 ) sprintf(dummy,"("); //First page of pdf file
            else if(x==pagenumber-1) sprintf(dummy,")"); //Last page of pdf file
            else sprintf(dummy,"");
            
            sprintf(histoname1,"%s%s",histoname,dummy);
            canvas[x]->SaveAs(histoname1);
            delete canvas[x];
        }
    }
    //END OF RATIOS HISTOGRAMS DRAW
    
    //RATIOS 3D HISTOGRAM DRAW
    for(int k = 0; k<ChannelList.size(); k++){
        
        if(k%4 == 0){
            /* 3D Histogram of phi, ratio and days */
            char hname[100];
            sprintf(hname, "Eta %d; Days; Phi; Normalized Ratios",RADDAM_CH[k].ieta);
            ChannelList[k].ratio2D = new TGraph2D();
            ChannelList[k].ratio2D->SetTitle(hname);
            
            /*ChannelList[k].ratio3D = new TH3F(hname,hname,130,0,129,80,0,79,100,0.8,1.2);//Days, phi and ratios in sequence
            ChannelList[k].ratio3D->GetYaxis()->SetTitle("Phi"); ChannelList[k].ratio3D->GetYaxis()->CenterTitle();
            ChannelList[k].ratio3D->GetZaxis()->SetTitle("Normalized Ratios"); ChannelList[k].ratio3D->GetZaxis()->CenterTitle();
            ChannelList[k].ratio3D->GetXaxis()->SetTitle("Days"); ChannelList[k].ratio3D->GetXaxis()->CenterTitle();*/
        }
        float A = ChannelList[k].runForOneChannel[0].ratioValue;
        int numberofruns = ChannelList[k].runForOneChannel.size();
        
        for(int l =0; l<numberofruns; l++){
            
            float B = ChannelList[k].runForOneChannel[l].ratioValue;
            //cout << k-k%4 << " phi: "<<RADDAM_CH[k].iphi << " ratio: " << B/A << " date: " << ChannelList[k].runForOneChannel[l].datetime << endl;
            //ChannelList[k-k%4].ratio3D->Fill(ChannelList[k].runForOneChannel[l].datetime, RADDAM_CH[k].iphi,B/A);
            ChannelList[k-k%4].ratio2D->SetPoint(ChannelList[k-k%4].ratio2D->GetN(), ChannelList[k].runForOneChannel[l].datetime, RADDAM_CH[k].iphi,B/A);
        }
        
    }
    
    int dividexaxes3d = 1;
    int divideyaxes3d = 2;
    int pagenumber3d = 7;
        
    char histoname1[20];
    char histoname[20];
    sprintf(histoname, "Ratios2d.pdf");
        
    TCanvas* canvas3d[pagenumber3d];
        
    for(int x=0;x<pagenumber3d;x++){
        char name[100];
        sprintf(name, "page%d",x);
        canvas3d[x] = new TCanvas(name,name,5000,6000);
        gStyle->SetOptStat(0);
        gStyle->SetTitleFontSize(0.1);
        canvas3d[x]->Divide(dividexaxes3d,divideyaxes3d,0.001,0.001);
    }
    int currentpage = 0; int currentcanv = 1;
        
    for(int o=0 ; o<14; o++){
        canvas3d[currentpage]->cd(currentcanv);
        currentcanv++;
        if(currentcanv> divideyaxes3d*dividexaxes3d ) {
            currentpage++;
            currentcanv = 1;
        }
        //ChannelList[o*4].ratio2D->GetZaxis()->SetLimits(0.8,1.2);
        ChannelList[o*4].ratio2D->GetHistogram()->SetMaximum(1.15); ChannelList[o*4].ratio2D->GetHistogram()->SetMinimum(0.85);
        ChannelList[o*4].ratio2D->Draw("lego2");
        ChannelList[o*4].ratio2D->Write();
    }
    for(int x=0;x<pagenumber3d; x++){
            
        char dummy[10];
            
        if( x==0 ) sprintf(dummy,"("); //First page of pdf file
        else if(x==pagenumber3d-1) sprintf(dummy,")"); //Last page of pdf file
        else sprintf(dummy,"");
            
        sprintf(histoname1,"%s%s",histoname,dummy);
        canvas3d[x]->SaveAs(histoname1);
        delete canvas3d[x];
    }
    //END OF RATIOS HISTOGRAMS DRAW
    
    //ALL RATIOS 3D HISTOGRAM DRAW
    TGraph2D* ratio2DHFP = new TGraph2D();
    ratio2DHFP->SetTitle("HF+ ; Days; Eta; Weighted Averages");
    TGraph2D* ratio2DHFM = new TGraph2D();
    ratio2DHFM->SetTitle("HF- ; Days; Eta; Weighted Averages");
    
    /*TH3F* ratioHFP3D = new TH3F("HF+","HF+",130,0,129,12,30,41,100,0.8,1.2);////Days, ieta and weighted averages in sequence
    ratioHFP3D->GetYaxis()->SetTitle("Eta"); ratioHFP3D->GetYaxis()->CenterTitle();
    ratioHFP3D->GetZaxis()->SetTitle("Weighted Averages"); ratioHFP3D->GetZaxis()->CenterTitle();
    ratioHFP3D->GetXaxis()->SetTitle("Days"); ratioHFP3D->GetXaxis()->CenterTitle();
    
    TH3F* ratioHFM3D = new TH3F("HF-","HF-",130,0,129,12,-30,-41,100,0.8,1.2);////Days, ieta and weighted averages in sequence
    ratioHFM3D->GetYaxis()->SetTitle("Eta"); ratioHFM3D->GetYaxis()->CenterTitle();
    ratioHFM3D->GetZaxis()->SetTitle("Weighted Averages"); ratioHFM3D->GetZaxis()->CenterTitle();
    ratioHFM3D->GetXaxis()->SetTitle("Days"); ratioHFM3D->GetXaxis()->CenterTitle();*/
    
    int numberofruns = ChannelList[0].runForOneChannel.size();
    float weightedaverage[numberofruns][14] = {{0}};
    float WAnormalization[numberofruns][14] = {{0}};
    float A, DA, B, DB, er, erSquare;
    
    for(int k = 0; k<ChannelList.size(); k++){
        
        A = ChannelList[k].runForOneChannel[0].ratioValue;
        DA = ChannelList[k].runForOneChannel[0].ratioError;
        numberofruns = ChannelList[k].runForOneChannel.size();
        
        for(int l =0; l<numberofruns; l++){
            
            B = ChannelList[k].runForOneChannel[l].ratioValue;
            DB = ChannelList[k].runForOneChannel[l].ratioError;
            
            if(B != 0){
                er = (B/A)*sqrt((DA/A)*(DA/A) + (DB/B)*(DB/B));
                erSquare = 1/(er*er);
                WAnormalization[l][k/4] += erSquare;
                weightedaverage[l][k/4] += (B/A)*erSquare;
            }
            
            if(k%4 == 3){//The last ieta one
                if(RADDAM_CH[k].ieta > 0){
                    //cout<< "eta: " << RADDAM_CH[k].ieta <<" number:" <<ratio2DHFP->GetN()<< " wa= "<< (weightedaverage[l][k/4]) << " norm: " << WAnormalization[l][k/4] << endl;
                    ratio2DHFP->SetPoint(ratio2DHFP->GetN(),ChannelList[k].runForOneChannel[l].datetime, RADDAM_CH[k].ieta,(weightedaverage[l][k/4])/WAnormalization[l][k/4]);
                    //ratioHFM3D->Fill(ChannelList[k].runForOneChannel[l].datetime, RADDAM_CH[k].ieta,(weightedaverage[l][k/4])/WAnormalization[l][k/4]);
                } else {
                    //cout<< "eta: " << RADDAM_CH[k].ieta <<" number:" <<ratio2DHFM->GetN() << " wa= "<< (weightedaverage[l][k/4])/WAnormalization[l][k/4] << endl;
                    ratio2DHFM->SetPoint(ratio2DHFM->GetN(),ChannelList[k].runForOneChannel[l].datetime, RADDAM_CH[k].ieta,(weightedaverage[l][k/4])/WAnormalization[l][k/4]);
                    //ratioHFP3D->Fill(ChannelList[k].runForOneChannel[l].datetime, RADDAM_CH[k].ieta,(weightedaverage[l][k/4])/WAnormalization[l][k/4]);
                }
            }
        }
    }
    
    TCanvas* canvas3 = new TCanvas("page","page",5000,6000);
    gStyle->SetOptStat(0);
    gStyle->SetTitleFontSize(0.1);
    canvas3->Divide(1,2,0.001,0.001);
    
    canvas3->cd(1);
    ratio2DHFP->GetHistogram()->SetMaximum(1.15); ratio2DHFP->GetHistogram()->SetMinimum(0.85);
    ratio2DHFP->Draw("lego2Z");
    ratio2DHFP->Write();
    
    canvas3->cd(2);
    ratio2DHFM->GetHistogram()->SetMaximum(1.15); ratio2DHFM->GetHistogram()->SetMinimum(0.85);
    ratio2DHFM->Draw("lego2Z");
    ratio2DHFM->Write();
    
    canvas3->SaveAs("RatiosAll2D.pdf");
    delete canvas3;
    //END OF ALL RATIOS HISTOGRAMS DRAW
    
    system("mv Ratios.root ../Histos/Ratios.root");

    outfile->Close();
    return;
}

int main(int argc, char *argv[]) {
    
    getmap();
    
    if(atoi(argv[1]) == 0) { //Draw the histograms of Q2/Q1 ratios of all runs
        plotratios();
    } else {
        RunNo=atoi(argv[1]);//Converts string to integer
        plotleds();
    }
}
