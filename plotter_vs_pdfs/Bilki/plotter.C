#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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

struct edata
{
	vector <Int_t> *ieta;
	vector <Int_t> *iphi;
	vector <Int_t> *depth;
	vector <vector <Int_t>> *pulse;
	vector <vector <Int_t>> *tdc;
	vector <vector <Int_t>> *capid;
};
edata ed;

struct semapin
{
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

struct semap
{
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
semap sem;vector <semap> SEM;

struct ledlist
{
	int mapind;
	float Qmean;
	float Qsigma;
	float npemean;
	float npeerr;
	float gain;
	float gainerr;
	float nbadcapid;
	float qf[32];
	float ng[32];
	TH1F* p;//pulse
	TH1F* pn;//pulse norm
	TH1F* t;//tdc
	double PT[10][3];//pulse, TDC, norm
	TGraph* PR;//signal ratio of later/sooner for the two largest consecutive TS signals
	TGraph* TT[2];//TDC trend for the largest two signals, black low TS, red high TS
	TGraph* QF;//Charge fraction (TS2+TS3)/all
	TH2F* TDCSel;//TDC TS2 vs TS3 for Qfrac>0.95
	TH1F* Qfrac23;//(Q2+Q3)/Qtot
	TH1F* S2_over_S1;//Q3/Q2
	TH1F* Qtot;//Qtot
	TH1F* Q[6][32];//Q
	TGraph* QP[6];//Q profile
	TGraph* PR23;//Q2/Q3
};

//from JM 070516

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

int RunNo=0;int RunType=0;

int plindMap[2][50][75][5]={{{{0}}}};

typedef struct
{
    int ieta;
    int iphi;
    int depth;
} Raddam_ch;

static const Raddam_ch RADDAM_CH[56]= {{30,1,2},{30,21,1},{30,37,2},{30,57,1},{32,1,1},{32,21,2},{32,37,1},{32,57,2},{34,1,2},{34,21,1},{34,37,2},{34,57,1},{36,1,1},{36,21,2 },{36,37,1},{36,57,2},{38,1,2},{38,21,1},{38,37,2},{38,57,1},{40,19,2},{40,35,1},{40,55,2},{40,71,1},{41,19,1},{41,35,2},{41,55,1},{41,71,2},{-30,15,2},{-30,35,1},{-30,51,2},{-30,71,1},{-32,15,1},{-32,35,2},{-32,51,1},{-32,71,2},{-34,15,2},{-34,35,1},{-34,51,2},{-34,71,1},{-36,15,1},{-36,35,2},{-36,51,1},{-36,71,2},{-38,15,2},{-38,35,1},{-38,51,2},{-38,71,1}, {-40,15,1},{-40,35,2},{-40,51,1},{-40,71,2},{-41,15,2},{-41,35,1},{-41,51,2},{-41,71,1}};

int getmap()
{
	ifstream semapinf("semapex_v3.txt");
	while(!semapinf.eof())
	{
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
		
// 		cout<<semin.crate<<" "<<semin.slot<<" "<<semin.fiber<<" "<<semin.channel<<" "<<semin.PMTname<<" "<<semin.Channelname<<" "<<semin.winchester<<" "<<semin.qiecard<<" "<<semin.qiecrate<<" "<<semin.qieslot<<" "<<semin.PMTid<<" "<<semin.BBid<<" "<<semin.ieta<<" "<<semin.iphi<<" "<<semin.depth<<" "<<semin.box<<" "<<semin.boxname<<" "<<semin.boxbarcode<<" "<<semin.VA<<" "<<semin.VB<<" "<<semin.VC<<" "<<semin.SX5gain<<" "<<semin.SX5gainerr<<" "<<semin.B904pedestal<<" "<<semin.B904pedestalsigma<<endl;
	}
	semapinf.close();
}

int findQ(int b)
{
	int retb=-1;
	if(b>=1 && b<=9) return 1;
	else if(b>=10 && b<=18) return 2;
	else if(b>=19 && b<=27) return 3;
	else if(b>=28 && b<=36) return 4;
}

int plotleds(int e1,int e2)
{
	char hname[500];char hname2[500];
	sprintf(hname,"../NTuples/N_%d.root",RunNo);
	TFile* inroot=new TFile(hname);
	TTree *tree = (TTree*)inroot->Get("Events");
	tree->SetBranchAddress("ieta",&ed.ieta);
	tree->SetBranchAddress("iphi",&ed.iphi);
	tree->SetBranchAddress("depth",&ed.depth);
	tree->SetBranchAddress("pulse",&ed.pulse);
	tree->SetBranchAddress("tdc",&ed.tdc);
	tree->SetBranchAddress("capid",&ed.capid);
	
	int ev1=e1;int ev2=e2;
	if(ev1==-1) {ev1=0;ev2=tree->GetEntries();}
	
	ledlist pl;vector <ledlist> PL;int plind=-1;
	tree->GetEntry(0);
	int nTS=ed.pulse->at(0).size();
	cout<<nTS<<endl;
	int qpcols[6]={9,1,2,3,4,6};
	for(int i1=0;i1<ed.ieta->size();i1++)
	{
		plind=-1;
		for(int i2=0;i2<PL.size();i2++)
		{
			if(SEM[PL[i2].mapind].ieta==ed.ieta->at(i1) && SEM[PL[i2].mapind].iphi==ed.iphi->at(i1) && SEM[PL[i2].mapind].depth==ed.depth->at(i1))
			{
				plind=i2;break;
			}
		}
		if(plind==-1)
		{
			pl.mapind=-1;
			for(int i2=0;i2<SEM.size();i2++)
			{
				if(SEM[i2].ieta==ed.ieta->at(i1) && SEM[i2].iphi==ed.iphi->at(i1) && SEM[i2].depth==ed.depth->at(i1))
				{
					pl.mapind=i2;break;
				}
			}
			pl.Qmean=0.;
			pl.Qsigma=0.;
			pl.npemean=0.;
			pl.npeerr=0.;
			pl.gain=0.;
			pl.gainerr=0.;
			pl.nbadcapid=0.;
			for(int ik1=0;ik1<32;ik1++)
			{
				pl.qf[ik1]=0.;
				pl.ng[ik1]=0.;
			}
			
			sprintf(hname,"SignalRatio %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			pl.PR=new TGraph();
			pl.PR->SetName(hname);pl.PR->SetTitle(hname);
			pl.PR->SetMarkerStyle(20);pl.PR->SetMarkerColor(1);
			sprintf(hname,"Q2/Q3 %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			pl.PR23=new TGraph();
			pl.PR23->SetName(hname);pl.PR23->SetTitle(hname);
			pl.PR23->SetMarkerStyle(20);pl.PR23->SetMarkerColor(1);
			pl.TT[0]=new TGraph();
			pl.TT[1]=new TGraph();
			sprintf(hname,"TDCTrend1 %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			pl.TT[0]->SetName(hname);pl.TT[0]->SetTitle(hname);
			sprintf(hname,"TDCTrend2 %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			pl.TT[1]->SetName(hname);pl.TT[1]->SetTitle(hname);
			pl.TT[0]->SetMarkerStyle(20);pl.TT[0]->SetMarkerColor(1);
			pl.TT[1]->SetMarkerStyle(21);pl.TT[1]->SetMarkerColor(2);
			sprintf(hname,"Qfracin_TS2TS3 %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			pl.QF=new TGraph();
			pl.QF->SetName(hname);pl.PR->SetTitle(hname);
			pl.QF->SetMarkerStyle(20);pl.PR->SetMarkerColor(1);
			
			sprintf(hname,"TDCSel_TS2TS3 %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			pl.TDCSel=new TH2F(hname,hname,64,-0.5,63.5,64,-0.5,63.5);
			pl.TDCSel->GetXaxis()->SetTitle("TDC TS2");pl.TDCSel->GetXaxis()->CenterTitle();
			pl.TDCSel->GetYaxis()->SetTitle("TDC TS3");pl.TDCSel->GetYaxis()->CenterTitle();
			
			if(SEM[pl.mapind].depth==1)
			{
				sprintf(hname,"Qfrac (%d, %d, 1) (%d, %d, 3)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
			}
			else
			{
				sprintf(hname,"Qfrac (%d, %d, 2) (%d, %d, 4)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
			}
			sprintf(hname2,"Qfrac %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			pl.Qfrac23=new TH1F(hname2,hname,50,0.51,1.01);
			pl.Qfrac23->GetXaxis()->SetTitle("(Q_{2}+Q_{3})/Q_{tot}");pl.Qfrac23->GetXaxis()->CenterTitle();
			if(SEM[pl.mapind].depth<=2) pl.Qfrac23->SetLineColor(1);
			else pl.Qfrac23->SetLineColor(2);
			
			if(SEM[pl.mapind].depth==1)
			{
				sprintf(hname,"S2/S1 (%d, %d, 1) (%d, %d, 3)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
			}
			else
			{
				sprintf(hname,"S2/S1 (%d, %d, 2) (%d, %d, 4)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
			}
			sprintf(hname2,"S2/S1 %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			pl.S2_over_S1=new TH1F(hname2,hname,100,0.,10.);
			pl.S2_over_S1->GetXaxis()->SetTitle("S_{2}/S_{1}");pl.S2_over_S1->GetXaxis()->CenterTitle();
			pl.S2_over_S1->GetYaxis()->SetTitle("Events / 0.1");pl.S2_over_S1->GetYaxis()->CenterTitle();
			if(SEM[pl.mapind].depth<=2) pl.S2_over_S1->SetLineColor(1);
			else pl.S2_over_S1->SetLineColor(2);
			
			if(SEM[pl.mapind].depth==1)
			{
				sprintf(hname,"Qtot (%d, %d, 1) (%d, %d, 3)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
			}
			else
			{
				sprintf(hname,"Qtot (%d, %d, 2) (%d, %d, 4)",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].ieta,SEM[pl.mapind].iphi);
			}
			sprintf(hname2,"Qtot %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			pl.Qtot=new TH1F(hname2,hname,1000,0.,100000.);
			pl.Qtot->GetXaxis()->SetTitle("Q_{2}+Q_{3} (fC)");pl.Qtot->GetXaxis()->CenterTitle();
			pl.Qtot->GetYaxis()->SetTitle("Events / 100 fC");pl.Qtot->GetYaxis()->CenterTitle();
			if(SEM[pl.mapind].depth<=2) pl.Qtot->SetLineColor(1);
			else pl.Qtot->SetLineColor(2);
			
			for(int is1=0;is1<6;is1++)
			{
				for(int is2=0;is2<32;is2++)
				{
					sprintf(hname,"Q (%d, %d, %d) %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth,is1,is2);
					sprintf(hname2,"Q %d %d %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth,is1,is2);
					pl.Q[is1][is2]=new TH1F(hname2,hname,1000,0.,100000.);
				}
				sprintf(hname,"Qprofile %d %d %d %d",SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth,is1);
				pl.QP[is1]=new TGraph();
				pl.QP[is1]->SetName(hname);pl.QP[is1]->SetTitle(hname);
				pl.QP[is1]->SetMarkerStyle(20);pl.QP[is1]->SetMarkerColor(qpcols[is1]);
			}
			
// 			cout<<SEM[pl.mapind].ieta<<" "<<SEM[pl.mapind].iphi<<" "<<SEM[pl.mapind].depth<<endl;
			
			for(int is1=0;is1<10;is1++)
			{
				for(int is2=0;is2<3;is2++)
				{
					pl.PT[is1][is2]=0.;
				}
			}
			
			sprintf(hname,"#splitline{Pulse %d %s %s}{      (%d, %d, %d)}",SEM[pl.mapind].box,SEM[pl.mapind].PMTname.c_str(),SEM[pl.mapind].Channelname.c_str(),SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			sprintf(hname2,"Pulse %d %s %s",SEM[pl.mapind].box,SEM[pl.mapind].PMTname.c_str(),SEM[pl.mapind].Channelname.c_str());
			pl.p=new TH1F(hname2,hname,nTS,-0.5,((float)nTS)-0.5);
			pl.p->GetXaxis()->SetTitle("TS (x25 ns)");pl.p->GetXaxis()->CenterTitle();
			pl.p->GetYaxis()->SetTitle("Mean Charge per TS (fC)");pl.p->GetYaxis()->CenterTitle();
			pl.p->SetFillColor(4);pl.p->SetLineColor(4);
			sprintf(hname,"PulseNorm %d %s %s",SEM[pl.mapind].box,SEM[pl.mapind].PMTname.c_str(),SEM[pl.mapind].Channelname.c_str());
			pl.pn=new TH1F(hname,hname,nTS,-0.5,((float)nTS)-0.5);
			
			sprintf(hname,"#splitline{TDC %d %s %s}{      (%d, %d, %d)}",SEM[pl.mapind].box,SEM[pl.mapind].PMTname.c_str(),SEM[pl.mapind].Channelname.c_str(),SEM[pl.mapind].ieta,SEM[pl.mapind].iphi,SEM[pl.mapind].depth);
			sprintf(hname2,"TDC %d %s %s",SEM[pl.mapind].box,SEM[pl.mapind].PMTname.c_str(),SEM[pl.mapind].Channelname.c_str());
			pl.t=new TH1F(hname2,hname,nTS,-0.5,((float)nTS)-0.5);
			pl.t->GetXaxis()->SetTitle("TS (x25 ns)");pl.t->GetXaxis()->CenterTitle();
			pl.t->GetYaxis()->SetTitle("Mean TDC per TS");pl.t->GetYaxis()->CenterTitle();
			pl.t->SetLineColor(2);
			
			PL.push_back(pl);
		}
	}
	
	int MI=-1;
	for(int i2=0;i2<PL.size();i2++)
	{
		MI=PL[i2].mapind;
		plindMap[(SEM[MI].ieta<0)?0:1][abs(SEM[MI].ieta)][SEM[MI].iphi][SEM[MI].depth]=i2;
	}
	
	cout<<PL.size()<<endl;
	
// 	TH1F* Qfracin2TS=new TH1F("Qfracin2TS","Qfracin2TS",101,0.,1.01);
	
	double ped=0.;double sig=0.;bool capidOK=true;int cap0=0;
	
	for(int i=ev1;i<ev2;i++)
	{
		tree->GetEntry(i);
// 		cout<<i<<" "<<endl;
// 		for(int i1=0;i1<ed.ieta->size();i1++)
// 		{
// 			cout<<ed.ieta->at(i1)<<" "<<ed.iphi->at(i1)<<" "<<ed.depth->at(i1)<<" : ";
// 			for(int i2=0;i2<nTS;i2++)
// 			{
// 				cout<<ed.pulse->at(i1)[i2]<<" ";
// 			}
// 			cout<<endl;
// 		}
		
		if(i%200==0) cout<<i<<" / "<<tree->GetEntries()<<endl;
		for(int i1=0;i1<ed.ieta->size();i1++)
		{
			plind=plindMap[(ed.ieta->at(i1)<0)?0:1][abs(ed.ieta->at(i1))][ed.iphi->at(i1)][ed.depth->at(i1)];
			
			ped=0.;sig=0.;capidOK=true;
			int TSmax1=0;int TSmax2=0;float Smax1=0.;float Smax2=0.;
			float QQ[10]={0.};float TDC[10]={0.};float Qtot=0.;float qf=0.;
			for(int i2=0;i2<nTS;i2++)
			{
				PL[plind].PT[i2][0]+=adc2fC_QIE10[ed.pulse->at(i1)[i2]];
				PL[plind].PT[i2][1]+=ed.tdc->at(i1)[i2];
				PL[plind].PT[i2][2]+=1.;
				QQ[i2]=adc2fC_QIE10[ed.pulse->at(i1)[i2]];
				TDC[i2]=ed.tdc->at(i1)[i2];
				Qtot+=QQ[i2];
				
				if(QQ[i2]>Smax1){Smax1=QQ[i2];TSmax1=i2;}
				
				if(i2>0)
				{
					if(!((ed.capid->at(i1)[i2]-ed.capid->at(i1)[i2-1])==1 || (ed.capid->at(i1)[i2]-ed.capid->at(i1)[i2-1])==-3))
					{
						capidOK=false;
					}
				}
			}
			if(!capidOK) PL[plind].nbadcapid+=1.;
			
			for(int i2=0;i2<nTS;i2++)
			{
				if(i2==TSmax1) continue;
				if(QQ[i2]>Smax2)
				{
					Smax2=QQ[i2];TSmax2=i2;
				}
			}
// 			for(int i2=0;i2<nTS;i2++){cout<<QQ[i2]<<" ";}cout<<" | "<<Smax1<<" "<<TSmax1<<" "<<Smax2<<" "<<TSmax2<<endl;
			if(abs(TSmax1-TSmax2)!=1)
			{
				cout<<"Pulse shape suspicious! (eta,phi,depth)=("<<ed.ieta->at(i1)<< ","<<ed.iphi->at(i1)<<","<<ed.depth->at(i1)<<") | ";
				for(int i2=0;i2<nTS;i2++){cout<<QQ[i2]<<" ";}
				cout<<endl;
			}
			
			int TSearly=0;int TSlate=0;
			if(TSmax1>=TSmax2){TSearly=TSmax2;TSlate=TSmax1;}
			else{TSearly=TSmax1;TSlate=TSmax2;}
			
// 			Qfracin2TS->Fill((QQ[TSlate]+QQ[TSearly])/Qtot);
			
			PL[plind].PR->SetPoint(PL[plind].PR->GetN(),i,QQ[TSlate]/QQ[TSearly]);
			PL[plind].PR23->SetPoint(PL[plind].PR23->GetN(),i,QQ[2]/QQ[3]);
			PL[plind].TT[0]->SetPoint(PL[plind].TT[0]->GetN(),i,TDC[TSearly]);
			PL[plind].TT[1]->SetPoint(PL[plind].TT[1]->GetN(),i,TDC[TSlate]);
			
			qf=(QQ[2]+QQ[3])/Qtot;
			
			PL[plind].QF->SetPoint(PL[plind].QF->GetN(),i,qf);
			PL[plind].qf[i/5000]+=(qf/5000.);
			
			if(qf>0.95)
			{
				PL[plind].TDCSel->Fill(TDC[2],TDC[3]);
				if(TDC[2]>=4 && TDC[2]<=12 && TDC[3]>=4 && TDC[3]<=12)
				{
					PL[plind].ng[i/5000]++;
				}
			}
			
			PL[plind].Qfrac23->Fill(qf);
			PL[plind].S2_over_S1->Fill(QQ[3]/QQ[2]);
			PL[plind].Qtot->Fill(Qtot);
			
			for(int i2=0;i2<nTS;i2++)
			{
				PL[plind].Q[i2][i/5000]->Fill(QQ[i2]);
			}
		}
	}
	double Pmax=0.;
	double qfmax=0.;
	double s2s1max=0.;
	double qtotmax=0.;
	int nn=0;
	int iside=0;int iquad=0;
	float fnevt=((float)(ev2-ev1));
	TF1* tf=new TF1("gaus","gaus",0.,250000.);
	
	TFile* outfile=new TFile("RadDam.root","recreate");
	
	for(int i2=0;i2<PL.size();i2++)
	{
		for(int is1=0;is1<nTS;is1++)
		{
			if(PL[i2].PT[is1][2]>0) {PL[i2].PT[is1][0]/=PL[i2].PT[is1][2];PL[i2].PT[is1][1]/=PL[i2].PT[is1][2];}
			PL[i2].p->SetBinContent(is1+1,PL[i2].PT[is1][0]);
			PL[i2].t->SetBinContent(is1+1,PL[i2].PT[is1][1]);
		}
		PL[i2].nbadcapid/=fnevt;
		if(PL[i2].p->GetBinContent(PL[i2].p->GetMaximumBin())>Pmax){Pmax=PL[i2].p->GetBinContent(PL[i2].p->GetMaximumBin());}
		if(PL[i2].Qfrac23->GetBinContent(PL[i2].Qfrac23->GetMaximumBin())>qfmax){qfmax=PL[i2].Qfrac23->GetBinContent(PL[i2].Qfrac23->GetMaximumBin());}
		if(PL[i2].S2_over_S1->GetBinContent(PL[i2].S2_over_S1->GetMaximumBin())>s2s1max){s2s1max=PL[i2].S2_over_S1->GetBinContent(PL[i2].S2_over_S1->GetMaximumBin());}
		if(PL[i2].Qtot->GetBinContent(PL[i2].Qtot->GetMaximumBin())>qtotmax){qtotmax=PL[i2].Qtot->GetBinContent(PL[i2].Qtot->GetMaximumBin());}
		
// 		for(int i3=0;i3<6;i3++)
		for(int i3=2;i3<=3;i3++)
		{
			double max23=0.;
			for(int i4=0;i4<32;i4++)
			{
				if(PL[i2].Q[i3][i4]->GetMean()>max23) max23=PL[i2].Q[i3][i4]->GetMean();
			}
			
			for(int i4=0;i4<32;i4++)
			{
				nn=PL[i2].QP[i3]->GetN();
				PL[i2].QP[i3]->SetPoint(nn,i4*2,PL[i2].Q[i3][i4]->GetMean()/max23);
			}
			PL[i2].QP[i3]->Write();
		}
	}
	
	TGraph* aPR=new TGraph();
	aPR->SetPoint(aPR->GetN(),0,0);
	aPR->SetPoint(aPR->GetN(),tree->GetEntries(),5);
	
	TGraph* aTT=new TGraph();
	aTT->SetPoint(aTT->GetN(),0,0);
	aTT->SetPoint(aTT->GetN(),tree->GetEntries(),65);
	
// 	Pmax+=100.;
	Pmax*=1.05;
	qfmax*=1.05;
	s2s1max*=1.05;
	qtotmax*=1.05;
	{
		char cname[400];
		char cname1[400];
		char dname[400];
		char dname1[400];
		char bname[100];
		char ename[400];
		char ename1[400];
		char fname[400];
		char fname1[400];
		string pmtnames[24]={"A2","A4","A6","A8","A1","A3","A5","A7","B2","B4","B6","B8","B1","B3","B5","B7","C2","C4","C6","C8","C1","C3","C5","C7"};
		string chnames[2]={"1+2","3+4"};
		string side[2]={"P","M"};
		
		sprintf(cname,"PulseShapes.pdf");
		sprintf(dname,"TDCShapes.pdf");
		sprintf(ename,"SignalRatios.pdf");
		sprintf(fname,"TDCTrends.pdf");
		
// 		sprintf(cname,"PulseShapes_%d_%d.pdf",ev1,ev2);
// 		sprintf(dname,"TDCShapes_%d_%d.pdf",ev1,ev2);
// 		sprintf(ename,"SignalRatios_%d_%d.pdf",ev1,ev2);
// 		sprintf(fname,"TDCTrends_%d_%d.pdf",ev1,ev2);
		
		TCanvas* cc1[2];
		cc1[0]=new TCanvas("cc10","cc10",5000,6000);
		cc1[1]=new TCanvas("cc11","cc11",5000,6000);
		TCanvas* cc2[2];
		cc2[0]=new TCanvas("cc20","cc20",5000,6000);
		cc2[1]=new TCanvas("cc21","cc21",5000,6000);
		TCanvas* cc3[2];
		cc3[0]=new TCanvas("cc30","cc30",5000,6000);
		cc3[1]=new TCanvas("cc31","cc31",5000,6000);
		TCanvas* cc4[2];
		cc4[0]=new TCanvas("cc40","cc40",5000,6000);
		cc4[1]=new TCanvas("cc41","cc41",5000,6000);
		gStyle->SetOptStat(0);
		gStyle->SetTitleFontSize(0.1);
		cc1[0]->Divide(8,7,0,0);cc1[1]->Divide(8,7,0,0);
		cc2[0]->Divide(8,7,0,0);cc2[1]->Divide(8,7,0,0);
		cc3[0]->Divide(8,7,0,0);cc3[1]->Divide(8,7,0,0);
		cc4[0]->Divide(8,7,0,0);cc4[1]->Divide(8,7,0,0);
		int cc1i=1;int cc2i=1;int cc3i=1;int cc4i=1;
		int cc1I=0;int cc2I=0;int cc3I=0;int cc4I=0;
		for(int i1=0;i1<56;i1++)
		{
			plind=-1;
			for(int ik1=0;ik1<PL.size();ik1++)
			{
				MI=PL[ik1].mapind;
				if(SEM[MI].ieta==RADDAM_CH[i1].ieta && SEM[MI].iphi==RADDAM_CH[i1].iphi && SEM[MI].depth==RADDAM_CH[i1].depth)
				{
					plind=ik1;break;
				}
			}
// 			cout<<RADDAM_CH[i1].ieta<<" "<<RADDAM_CH[i1].iphi<<" "<<RADDAM_CH[i1].depth<<endl;
// 			cout<<plind<<endl;
			cc1[cc1I]->cd(cc1i);gPad-> SetLogy(0);
			PL[plind].p->Draw("hist");
			PL[plind].p->GetYaxis()->SetRangeUser(-10.,Pmax);
			PL[plind].p->Write();
			cc1i++;
			if(cc1i>56){cc1I++;cc1i=1;}
			
			cc2[cc2I]->cd(cc2i);gPad-> SetLogy(0);
			PL[plind].t->Draw();
			PL[plind].t->GetYaxis()->SetRangeUser(0.,75.);
			PL[plind].t->Write();
			cc2i++;
			if(cc2i>56){cc2I++;cc2i=1;}
			
			cc3[cc3I]->cd(cc3i);gPad-> SetLogy(0);
			aPR->Draw("AP");
			PL[plind].PR->Draw("P same");
			PL[plind].PR->Write();
			cc3i++;
			if(cc3i>56){cc3I++;cc3i=1;}
			
			PL[plind].PR23->Write();
			
// 			cc4[cc4I]->cd(cc4i);gPad-> SetLogy(0);
// 			aTT->Draw("AP");
// 			PL[plind].TT[0]->Draw("P same");
// 			PL[plind].TT[1]->Draw("P same");
// 			PL[plind].TT[0]->Write();
// 			PL[plind].TT[1]->Write();
// 			cc4i++;
// 			if(cc4i>56){cc4I++;cc4i=1;}
			
// 			PL[plind].QF->Write();
			PL[plind].TDCSel->Write();
			
			plind=-1;
			for(int ik1=0;ik1<PL.size();ik1++)
			{
				MI=PL[ik1].mapind;
				if(SEM[MI].ieta==RADDAM_CH[i1].ieta && SEM[MI].iphi==RADDAM_CH[i1].iphi && SEM[MI].depth==(RADDAM_CH[i1].depth+2))
				{
					plind=ik1;break;
				}
			}
			cc1[cc1I]->cd(cc1i);gPad-> SetLogy(0);
			PL[plind].p->Draw("hist");
			PL[plind].p->GetYaxis()->SetRangeUser(-10.,Pmax);
			PL[plind].p->Write();
			cc1i++;
			if(cc1i>56){cc1I++;cc1i=1;}
			
			cc2[cc2I]->cd(cc2i);gPad-> SetLogy(0);
			PL[plind].t->Draw();
			PL[plind].t->GetYaxis()->SetRangeUser(0.,75.);
			PL[plind].t->Write();
			cc2i++;
			if(cc2i>56){cc2I++;cc2i=1;}
			
			cc3[cc3I]->cd(cc3i);gPad-> SetLogy(0);
			aPR->Draw("AP");
			PL[plind].PR->Draw("P same");
			PL[plind].PR->Write();
			cc3i++;
			if(cc3i>56){cc3I++;cc3i=1;}
			
			PL[plind].PR23->Write();
			
// 			cc4[cc4I]->cd(cc4i);gPad-> SetLogy(0);
// 			aTT->Draw("AP");
// 			PL[plind].TT[0]->Draw("P same");
// 			PL[plind].TT[1]->Draw("P same");
// 			PL[plind].TT[0]->Write();
// 			PL[plind].TT[1]->Write();
// 			cc4i++;
// 			if(cc4i>56){cc4I++;cc4i=1;}
			
// 			PL[plind].QF->Write();
			PL[plind].TDCSel->Write();
		}
		sprintf(cname1,"%s(",cname);cc1[0]->SaveAs(cname1);
		sprintf(cname1,"%s)",cname);cc1[1]->SaveAs(cname1);
		sprintf(dname1,"%s(",dname);cc2[0]->SaveAs(dname1);
		sprintf(dname1,"%s)",dname);cc2[1]->SaveAs(dname1);
// 		sprintf(ename1,"%s(",ename);cc3[0]->SaveAs(ename1);
// 		sprintf(ename1,"%s)",ename);cc3[1]->SaveAs(ename1);
// 		sprintf(fname1,"%s(",fname);cc4[0]->SaveAs(fname1);
// 		sprintf(fname1,"%s)",fname);cc4[1]->SaveAs(fname1);
		delete cc1[0];delete cc1[1];
		delete cc2[0];delete cc2[1];
		delete cc3[0];delete cc3[1];
		delete cc4[0];delete cc4[1];
		
		sprintf(hname,"mv %s ../Plots/%d",cname,RunNo);system(hname);
		sprintf(hname,"mv %s ../Plots/%d",dname,RunNo);system(hname);
		
		
		//if it takes too long comment out the below two lines
// 		sprintf(hname,"mv %s ../Plots/%d",ename,RunNo);system(hname);
// 		sprintf(hname,"mv %s ../Plots/%d",fname,RunNo);system(hname);
		
// 		Qfracin2TS->Write();
		
		for(int ik1=0;ik1<PL.size();ik1++)
		{
			int max=0;float qfmax=0.;
			for(int ik2=0;ik2<32;ik2++)
			{
				if(PL[ik1].qf[ik2]>qfmax){qfmax=PL[ik1].qf[ik2];max=ik2;}
			}
// 			cout<<SEM[PL[ik1].mapind].ieta<<" "<<SEM[PL[ik1].mapind].iphi<<" "<<SEM[PL[ik1].mapind].depth<<" "<<max<<" "<<(max*5000)<<" - "<<((max+1)*5000)<<" "<<qfmax<<endl;
// 			cout<<SEM[PL[ik1].mapind].ieta<<" "<<SEM[PL[ik1].mapind].iphi<<" "<<SEM[PL[ik1].mapind].depth<<" "<<max<<" "<<(max*5000)<<" - "<<((max+1)*5000)<<" "<<qfmax<<" ";
// 			for(int ik2=0;ik2<32;ik2++)
// 			{
// 				cout<<PL[ik1].ng[ik2]<<" ";
// 			}
// 			cout<<endl;
		}
		
		for(int ik1=0;ik1<PL.size();ik1++)
		{
			PL[ik1].p=0;PL[ik1].pn=0;PL[ik1].t=0;
		}
	}
// 	{
// 		char c1name[400];
// 		char c2name[400];
// 		char c3name[400];
// 		
// 		sprintf(c1name,"Qfrac_%d_%d.pdf",ev1,ev2);
// 		sprintf(c2name,"S2_over_S1_%d_%d.pdf",ev1,ev2);
// 		sprintf(c3name,"Qtot_%d_%d.pdf",ev1,ev2);
// 		
// 		TCanvas* cc1n[3];
// 		cc1n[0]=new TCanvas("cc1n0","cc1n0",5000,6000);
// 		cc1n[1]=new TCanvas("cc1n1","cc1n1",5000,6000);
// 		cc1n[2]=new TCanvas("cc1n2","cc1n2",5000,6000);
// 		gStyle->SetOptStat(0);
// 		gStyle->SetTitleFontSize(0.1);
// 		cc1n[0]->Divide(8,7,0,0);
// 		cc1n[1]->Divide(8,7,0,0);
// 		cc1n[2]->Divide(8,7,0,0);
// 		int cc1i=1;int cc2i=1;int cc3i=1;int cc4i=1;
// 		int plind1=0;int plind2=0;
// 		for(int i1=0;i1<56;i1++)
// 		{
// 			plind1=-1;
// 			for(int ik1=0;ik1<PL.size();ik1++)
// 			{
// 				MI=PL[ik1].mapind;
// 				if(SEM[MI].ieta==RADDAM_CH[i1].ieta && SEM[MI].iphi==RADDAM_CH[i1].iphi && SEM[MI].depth==RADDAM_CH[i1].depth)
// 				{
// 					plind1=ik1;break;
// 				}
// 			}
// 			plind2=-1;
// 			for(int ik1=0;ik1<PL.size();ik1++)
// 			{
// 				MI=PL[ik1].mapind;
// 				if(SEM[MI].ieta==RADDAM_CH[i1].ieta && SEM[MI].iphi==RADDAM_CH[i1].iphi && SEM[MI].depth==(RADDAM_CH[i1].depth+2))
// 				{
// 					plind2=ik1;break;
// 				}
// 			}
// 			
// 			cc1n[0]->cd(cc1i);gPad-> SetLogy(1);
// 			PL[plind1].Qfrac23->Draw();PL[plind2].Qfrac23->Draw("same");
// 			PL[plind1].Qfrac23->GetYaxis()->SetRangeUser(1.,qfmax);
// 			PL[plind1].Qfrac23->Write();
// 			PL[plind2].Qfrac23->Write();
// 			cc1i++;
// 			
// 			cc1n[1]->cd(cc2i);gPad-> SetLogy(1);
// 			PL[plind1].S2_over_S1->Draw();PL[plind2].S2_over_S1->Draw("same");
// 			PL[plind1].S2_over_S1->GetYaxis()->SetRangeUser(1.,s2s1max);
// 			PL[plind1].S2_over_S1->Write();
// 			PL[plind2].S2_over_S1->Write();
// 			cc2i++;
// 			
// 			cc1n[2]->cd(cc3i);gPad-> SetLogy(1);
// 			PL[plind1].Qtot->Draw();PL[plind2].Qtot->Draw("same");
// 			PL[plind1].Qtot->GetYaxis()->SetRangeUser(1.,qtotmax);
// 			PL[plind1].Qtot->Write();
// 			PL[plind2].Qtot->Write();
// 			cc3i++;
// 		}
// 		cc1n[0]->SaveAs(c1name);
// 		cc1n[1]->SaveAs(c2name);
// 		cc1n[2]->SaveAs(c3name);
// 		delete cc1n[0];
// 		delete cc1n[1];
// 		delete cc1n[2];
// 		
// 		sprintf(hname,"mv %s ../Plots/%d",c1name,RunNo);system(hname);
// 		sprintf(hname,"mv %s ../Plots/%d",c2name,RunNo);system(hname);
// 		sprintf(hname,"mv %s ../Plots/%d",c3name,RunNo);system(hname);
// 	}
	sprintf(hname,"mv RadDam.root ../Histos/RadDam_%d.root",RunNo);system(hname);
// 	sprintf(hname,"mv RadDam.root ../Histos/RadDam_%d_%d_%d.root",RunNo,ev1,ev2);system(hname);
	outfile->Close();
}

int main(int argc, char *argv[])
{
	RunNo=atoi(argv[1]);
// 	int e1=atoi(argv[2]);
// 	int e2=atoi(argv[3]);
	
	getmap();
	plotleds(-1,-1);
// 	plotleds(e1,e2);
}

















