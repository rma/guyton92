//============================================================================
// Name        : GuytonC++.cpp
// Author      : thibault Grosse
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in //, Ansi-style
//============================================================================

#include <cmath>
#include <iostream>
#include <cstdlib>
using namespace std;

/* Collect parameters into a single struct and allow parameter values to be
   specified in an external file. */
#include "read_params.h"
/* Collect state variables into a single struct, and allow the initial values
   to be specified in an external file. */
#include "read_vars.h"

/* Provides the ability to output the entire model state (for debugging). */
#include "debug.h"

/* Provides the utility function funct(), which is used by the circulatory
   dynamics and capillary dynamics modules of the model. */
#include "utils.h"

/* The renal module, separated from the original (monolithic) code. */
#include "module_renal.h"
/* The circulatory dynamics module. */
#include "module_circdyn.h"
/* The autonomic circulation module. */
#include "module_autonom.h"
/* The aldosterone module. */
#include "module_aldost.h"
/* The angiotensin module. */
#include "module_angio.h"
/* The atrial natriuretic peptide (ANP) module. */
#include "module_anp.h"
/* The red blood cell (RBC) and viscosity module. */
#include "module_rbc.h"

int main(int argc, char *argv[]) {

	double t = 0.00E+000;
	double i = 3.00E-003;
	double a1k=0.50000E+00;
	double a2k=0.60000E+02;
	double a3k=0.40000E+05;
	double a4k=0.10000E+00;
	double a4k2=0.40000E+05;
	double aark=0.10000E+01;
	double adhpam=0.30000E+00;
	double adhtc=0.15000E+02;
	double ah10=0.33300E+00;
	double ah11=0.10000E+04;
	double ah9=0.10000E+01;
	double ahmnar=0.30000E+00;
	double ahthm=0.20000E+01;
	double alclk=0.30000E+00;
	double aldmm=0.25000E+01;
	double amcsns=0.65000E+00;
	double amkm=0.50000E+00;
	double amkmul=0.12000E+02;
	double amnam=0.80000E+00;
	double amt=0.60000E+02;
	double ancsns=0.40000E+00;
	double anmald=0.25000E+01;
	double anmam=0.14000E+01;
	double anmem=0.16000E+01;
	double anmkem=0.20000E+01;
	double anmnam=0.10000E+01;
	double anmslt=0.20000E+01;
	double anmtm=0.15000E+01;
	double anptc=0.40000E+01;
	double anpxaf=0.15000E+01;
	double ant=0.12000E+02;
	double anum=0.60000E+01;
	double anv=0.50000E+04;
	double any=-0.20000E+00;
	double auc1=0.30000E+00;
	double audmp=0.30000E+00;
	double auk=0.40000E-02;
	double aumk1=0.30000E+00;
	double aun1=0.50000E+00;
	double aus=0.10000E+01;
	double autogn=0.70000E+01;
	double autok=0.10000E+00;
	double autosn=0.90000E+00;
	double auv=0.55000E+00;
	double aux=0.10000E+01;
	double barotc=0.16000E+00;
	double cfc=0.11700E-01;
	double cnr=0.13900E+03;
	double cpf=0.30000E-03;
	double cpr=0.40000E+02;
	double cv=0.50000E-01;
	double dhdtr=0.50000E-01;
	double diuret=0.10000E+01;
	double dr=0.00000E+00;
	double dtnar=0.67500E+00;
	double eark=0.10000E+01;
	double exc=0.10000E+01;
	double excml=0.10000E-01;
	double exe=0.00000E+00;
	double fis=0.00000E+00;
	double gflc=0.20800E-01;
	double gfndmp=0.30000E+01;
	double gppd=0.10000E+01;
	double hm6=0.18500E+04;
	double hm8=0.47100E-07;
	double hmk=0.90000E+02;
	double hmtrns=0.40000E+02;
	double hsl=0.10000E+01;
	double hsr=0.10000E+01;
	double htauml=0.40000E+00;
	double hyl=0.60000E+02;
	double kid=0.80000E-01;
	double korgn=0.10000E+01;
	double kortc=0.10000E+05;
	double lppr=0.30000E-01;
	double mdflkm=0.66700E+00;
	double mdflw2=0.50000E-02;
	double mdflwx=0.10000E-01;
	double nid=0.10000E+00;
	double o2a=0.10000E+00;
	double o2chmo=0.10000E-01;
	double o2m=0.16400E+03;
	double omm=0.57100E+02;
	double pcr=0.15000E+02;
	double pm5=0.30000E+02;
	double por=0.35000E+02;
	double pxtp=0.80000E+01;
	double qrf=0.15000E+00;
	double rabsc=0.50000E+00;
	double rcdfpc=0.00000E+00;
	double rek=0.10000E+01;
	double rfabdm=0.30000E+00;
	double rfabdp=0.10000E+01;
	double rfabkm=0.30000E-01;
	double rnagtc=0.15000E+02;
	double rnaugn=0.60000E+00;
	double rtppr=0.90000E+00;
	double rtpprs=0.15200E+02;
	double rtsprs=0.60000E+01;
	double rvsm=0.10000E+01;
	double sr=0.10000E+01;
	double sr2=0.10000E+01;
	double srk=0.50000E+01;
	double srk2=0.10000E+05;
	double tenstc=0.50000E-01;
	double tsslml=0.15000E+00;
	double tssltc=0.50000E-02;
	double tvddl=0.30000E+02;
	double u=0.40000E+01;
	double urform=0.24000E+00;
	double vidml=0.10000E-01;
	double vntstm=0.10000E+01;
	double vptiss=0.17500E-01;
	double vv9=0.25100E+01;
	double au4=-0.23416E-01;
	double au2=0.28254E-02;
	double au8=0.11302E-04;
	double amk=0.10378E+01;
	double amna=0.10605E+01;
	double amrbsc=0.10049E+01;
	double amr=0.10591E+01;
	double amr1=0.10591E+01;
	double am1=0.10303E+01;
	double am=0.10757E+01;
	double amc=0.10587E+01;
	double anu=0.93542E+00;
	double anuvn=0.10000E+01;
	double anm=0.98924E+00;
	double mdflw3=0.10019E+01;
	double angscr=0.87886E+00;
	double anx=0.00000E+00;
	double anx1=0.00000E+00;
	double anpr=0.87886E+00;
	double anpr1=0.87886E+00;
	double anc=0.87831E+00;
	double anpx=0.23134E+00;
	double anpl=0.96895E+00;
	double anpr2=0.15231E+01;
	double anp=0.83069E+00;
	double anp1=0.83069E+00;
	double anpc=0.84263E+00;
	double adhc=0.10157E+01;
	double adhmk=0.10105E+01;
	double adhmv=0.10010E+01;
	double adhna=0.10157E+01;
	double adhpa=0.85000E+02;
	double adhpr=0.00000E+00;
	double adh=0.10157E+01;
	double adhz=-0.15640E+01;
	double au=0.10000E+01;
	double au6=0.10028E+01;
	double auh=0.10000E+01;
	double aum=0.10000E+01;
	double aur=0.10000E+01;
	double ave=0.10000E+01;
	double pa1=0.10124E+03;
	double vvr=0.25100E+01;
	double auo=0.15997E-05;
	double auc=0.00000E+00;
	double auc2=0.00000E+00;
	double aub=0.97939E+00;
	double a1b=0.97939E+00;
	double aun=0.00000E+00;
	double aulp=0.10000E+01;
	double auttl=0.10000E+01;
	double dau=0.28270E-04;
	double au1=0.10000E+01;
	double rmult1=0.10000E+01;
	double vrc=0.19287E+01;
	double hm=0.40201E+02;
	double hm1=0.40201E+00;
	double vb=0.47976E+01;
	double vim=0.10048E+01;
	double vie=0.15146E+01;
	double vib=0.30146E+01;
	double po2am1=0.80000E+02;
	double hm3=0.16080E+04;
	double hm4=0.11000E+03;
	double hm5=0.17180E+04;
	double hm7=0.13196E+03;
	double rc1=0.11215E-04;
	double rc2=0.11240E-04;
	double rcd=-0.24274E-07;
	double prp=0.20663E+03;
	double cpp=0.71968E+02;
	double pc=0.16822E+02;
	double ppc=0.29992E+02;
	double vp=0.28711E+01;
	double vtc=0.25921E-02;
	double vpd=0.10317E-02;
	double cppd=0.31968E+02;
	double dlp=0.22539E-03;
	double prcd=0.18222E+01;
	double vtcpl=0.46101E-03;
	double dpc=0.83650E-01;
	double dpp=0.17926E-02;
	double vts=0.11971E+02;
	double vts2=-0.41939E-02;
	double vts1=0.11975E+02;
	double dpi=-0.15690E-02;
	double tsp=0.28061E+03;
	double cpi=0.23441E+02;
	double dpl=0.84921E-01;
	double ptcpr=0.76074E+01;
	double chy=0.10049E+01;
	double pgh=-0.10173E+01;
	double poshyl=0.20097E+01;
	double ptc=0.12369E+02;
	double ptt=0.99171E+00;
	double pif=-0.30270E+01;
	double pts=0.40187E+01;
	double pld=0.18126E+00;
	double vtl=0.36251E-02;
	double vif=0.59854E+00;
	double bfm=0.97084E+00;
	double bfn=0.27700E+01;
	double myogrs=0.10000E+01;
	double pa=0.10124E+03;
	double pamkrn=0.10000E+01;
	double pla=0.19689E+01;
	double pra=-0.23844E+00;
	double ppa=0.16517E+02;
	double pvs=0.36841E+01;
	double qao=0.49607E+01;
	double qlo=0.49592E+01;
	double qro=0.49633E+01;
	double rpv=0.14027E+01;
	double rvs=0.27899E+01;
	double vae=0.15692E+00;
	double vle=0.19689E-01;
	double vpe=0.79283E-01;
	double vre=-0.11922E-02;
	double vve=0.73921E+00;
	double vbd=-0.25692E-02;
	double vvs=0.32617E+01;
	double vas=0.65192E+00;
	double vla=0.39969E+00;
	double vpa=0.38553E+00;
	double vra=0.98808E-01;
	double pam=0.10249E+01;
	double rad=0.10000E+01;
	double pamk=0.10000E+01;
	double pa2=0.10406E+03;
	double pra1=-0.23844E+00;
	double qrn=0.47139E+01;
	double pp1=0.42945E+00;
	double cpa=0.65532E+00;
	double rpa=0.15260E+01;
	double pp2=0.16977E+02;
	double rvm=0.10123E+01;
	double pla1=0.19689E+01;
	double qln=0.50050E+01;
	double pl1=0.19969E+02;
	double rpt=0.29287E+01;
	double pgl=0.14548E+02;
	double qpo=0.49613E+01;
	double pr1=0.00000E+00;
	double rvg=0.74317E+00;
	double qvo=0.49618E+01;
	double cn3=0.35996E+00;
	double rv1=0.27781E+01;
	double pgs=0.97554E+02;
	double r1=0.91745E+00;
	double rsn=0.35217E+02;
	double rsm=0.10048E+03;
	double fisflo=0.00000E+00;
	double sysflo=0.49607E+01;
	double qlo1=-0.33090E+02;
	double hpef=0.10529E+01;
	double dvs=-0.10590E-02;
	double dpa=0.20313E-02;
	double das=-0.15314E-02;
	double dla=0.20714E-02;
	double dra=-0.15123E-02;
	double cna=0.14205E+03;
	double cke=0.44361E+01;
	double vec=0.14855E+02;
	double vtw=0.39889E+02;
	double ned=0.43872E-02;
	double amk1=0.10114E+01;
	double ktotd=0.37230E-03;
	double ktot=0.36220E+04;
	double vic=0.25035E+02;
	double cki=0.14205E+03;
	double nae=0.21101E+04;
	double ccd=-0.16718E-04;
	double vid=-0.16718E-06;
	double hmd=0.10000E+01;
	double dhm=0.12508E+01;
	double hpl=0.10032E+01;
	double hpr=0.10510E+01;
	double nod=0.93142E-01;
	double kod=0.79628E-01;
	double vud=0.96624E-03;
	double rbf=0.12185E+01;
	double mdflw=0.99852E+00;
	double par=0.10124E+03;
	double aumk=0.10000E+01;
	double anmer=0.98278E+00;
	double anmar=0.98493E+00;
	double rnaug1=0.99863E+00;
	double rnaug2=0.99863E+00;
	double rnaug3=0.00000E+00;
	double aar=0.40496E+02;
	double ear=0.42587E+02;
	double rr=0.83083E+02;
	double rfn=0.12185E+01;
	double efafpr=0.12068E+01;
	double glpc=0.37884E+02;
	double apd=0.49345E+02;
	double glp=0.51893E+02;
	double pfl=0.60086E+01;
	double rcprs=0.23324E+02;
	double rtsppc=0.18896E+02;
	double rabspr=0.16640E+01;
	double rfab1=0.83201E+00;
	double rfab=0.83201E+00;
	double rfabk=-0.15119E-02;
	double dtnai=0.87399E+00;
	double dtnara=0.68192E+00;
	double dtki=0.27294E-01;
	double anmke=0.97847E+00;
	double mdflk=0.99902E+00;
	double dtksc=0.87587E-01;
	double dtnang=0.98924E-01;
	double nodn=0.93142E-01;
	double dtka=0.36765E-01;
	double kodn=0.79628E-01;
	double osmopn=0.58581E+00;
	double vudn=0.96624E-03;
	double plur=0.15950E+03;
	double plurc=0.39985E+01;
	double dturi=0.23984E+00;
	double urod=0.23984E+00;
	double amm=0.56866E+00;
	double pdo=-0.21628E+00;
	double poe=0.99135E+00;
	double amm1=0.99156E+00;
	double amm2=0.57350E+00;
	double aom=0.10000E+01;
	double pmo=0.37784E+02;
	double rmo=0.56804E+02;
	double mmo=0.57100E+02;
	double ovs=0.69538E+00;
	double pvo=0.39734E+02;
	double do2m=-0.29588E+00;
	double qom=0.47707E+02;
	double p2o=0.37784E+02;
	double arm=0.10800E+01;
	double pod=0.16270E-01;
	double pob=0.10016E+01;
	double ar1=0.10042E+01;
	double poa=0.10016E+01;
	double ar2=0.10043E+01;
	double poc=0.10325E+01;
	double ar3=0.10797E+01;
	double dob=0.16351E+03;
	double pot=0.35016E+02;
	double osv=0.69316E+00;
	double pov=0.39607E+02;
	double mo2=0.16400E+03;
	double do2n=-0.49273E+00;
	double qo2=0.72034E+02;
	double p1o=0.35000E+02;
	double dfp=-0.94955E-06;
	double ppd=0.47219E-05;
	double vpf=0.12530E-01;
	double pcp=0.89370E+01;
	double ppi=-0.99694E+01;
	double cpn=0.30283E+02;
	double pos=0.12113E+02;
	double plf=0.30917E-03;
	double ppo=0.93626E-02;
	double ppn=0.93791E-02;
	double ppz=0.16548E-04;
	double pfi=0.30835E-03;
	double dfz=-0.82401E-06;
	double ppr=0.37951E+00;
	double o2util=0.22189E+03;
	double alvent=0.59172E+01;
	double po2alv=0.10072E+03;
	double rspdfc=0.59936E+02;
	double o2dfs=0.22272E+03;
	double dova=0.16670E+00;
	double o2vtst=0.10000E+01;
	double o2vts2=0.10000E+01;
	double do2vad=-0.41691E-34;
	double o2vad1=0.83294E-31;
	double o2vad2=0.10000E+01;
	double osa=0.97281E+00;
	double ova=0.20532E+03;
	double po2art=0.97008E+02;
	double pmc=0.90355E+01;
	double pms=0.95459E+01;
	double pmp=0.60906E+01;
	double hr=0.72000E+02;
	double rtp=0.20456E+02;
	double svo=0.68878E-01;
	double vv6=-0.28235E-03;
	double vv7=-0.37111E-03;
	double tvd=0.98480E-03;
	double sth=0.97529E+00;
	double ke=0.65896E+02;
	double ki=0.35561E+04;
	double atrrfb=0.10000E+01;
	double atrvfb=0.00000E+00;
	double ahz=-0.62040E+00;
	double ahy=-0.60930E+00;
	double ah7=-0.11096E-01;
	double aarll=0.40000E+01;
	double adhinf=0.00000E+00;
	double adhkll=0.20000E+00;
	double adhkul=0.50000E+01;
	double adhpul=0.85000E+02;
	double adhvll=0.93600E+00;
	double adhvul=0.25000E+01;
	double agk=0.40000E+01;
	double ahk=0.15000E+02;
	double ahth=0.10060E-02;
	double aldinf=0.00000E+00;
	double aldkns=0.00000E+00;
	double am1ll=0.00000E+00;
	double am1ul=0.50000E+01;
	double amm1lm=0.50000E-02;
	double amm3=0.99158E+00;
	double amm4=0.50000E-02;
	double amnall=0.40000E-01;
	double amnaul=0.15000E+02;
	double anginf=0.00000E+00;
	double angkns=0.00000E+00;
	double anmarl=0.86000E+00;
	double anmkel=0.30000E+00;
	double anmll=0.70000E+00;
	double anmsml=0.97847E+00;
	double anmth=-0.16145E-04;
	double anmul=0.18000E+01;
	double anpinf=0.00000E+00;
	double anpkns=0.00000E+00;
	double anpxul=0.10000E+02;
	double anull=0.80000E+00;
	double anuvm=0.00000E+00;
	double anxm=0.00000E+00;
	double ar1lm=0.50000E-02;
	double ar2lm=0.50000E-02;
	double ar4=0.10042E+01;
	double ar5=0.10044E+01;
	double arf=0.50000E+00;
	double atrfbm=0.00000E+00;
	double atrvm=0.00000E+00;
	double au6a=0.10028E+01;
	double au6b=0.99580E+00;
	double au6c=0.10028E+01;
	double aul=0.15000E+01;
	double aulpm=0.00000E+00;
	double aum1=0.30000E+01;
	double aum2=0.10000E+01;
	double aumax=0.50000E+01;
	double aumin=0.40000E+00;
	double aurg=0.10000E+01;
	double auslp=0.15000E+01;
	double autoC=0.00000E+00;
	double auy=0.00000E+00;
	double ckeex=0.40000E+01;
	double cmptss=0.20000E+01;
	double cn2=0.21200E-01;
	double cn7=0.20000E+00;
	double cpk=0.25300E-03;
	//	double c=0.00000E+00;
	double crrflx=0.00000E+00;
	double dtnarl=0.10000E-05;
	double earll=0.24000E+02;
	double efafr=0.00000E+00;
	double excxp=0.10000E+01;
	double excxp2=0.17000E+00;
	double gbl=0.00000E+00;
	double gcopf=0.80900E+00;
	double gfn=0.12498E+00;
	double gfnll=0.10000E-02;
	double gfr=0.12498E+00;
	double glpca=0.10000E+01;
	double hkm=0.53300E+00;
	double i1=0.32435E+01;
	double i2=0.30000E-02;
	double i3=0.20000E+02;
	double i4=0.10000E+00;
	double i5=0.00000E+00;
	double i6=0.10000E+00;
	double i8=0.20000E-02;
	double i9=0.00000E+00;
	double i10=0.10000E-02;
	double i11=0.00000E+00;
	double i12=0.10000E-02;
	double i13=0.00000E+00;
	double i14=0.10000E+01;
	double i15=0.00000E+00;
	double i16=0.50000E-03;
	double i17=0.00000E+00;
	double i18=0.10000E+01;
	double i19=0.00000E+00;
	double i20=0.10000E-01;
	double i21=0.00000E+00;
	double korner=0.00000E+00;
	double korren=0.00000E+00;
	double lpde=0.80000E+01;
	double lpk=0.27300E-13;
	double lvm=0.98772E+00;
	double mdfl1=0.10000E+02;
	double mdflw1=0.99862E+00;
	double mdmp=0.00000E+00;
	double osmop1=0.00000E+00;
	double pa3=0.10166E+03;
	double pa4=0.10000E+01;
	double paex=0.20000E+01;
	double pcd=0.30000E+00;
	double pce=0.10000E+01;
	double pghf=-0.20000E+01;
	double pgp=0.41200E+01;
	double pgv=0.36841E+01;
	double pk2=0.79200E+00;
	double pl2=0.18000E+01;
	double pldf=0.42000E+01;
	double po2amb=0.15000E+03;
	double po2adv=0.10000E-02;
	double po2ar1=0.97007E+02;
	double pok=0.10000E+00;
	double pom=0.40000E-01;
	double pom2=0.20000E+01;
	double pon=0.10000E+00;
	double poz=0.20000E+01;
	double pr1ll=0.00000E+00;
	double ptfl=0.99985E+00;
	double qaolm=0.20000E+00;
	//	double ra=0.00000E+00;
	double raprsp=0.00000E+00;
	double rar=0.30500E+02;
	double rcdfdp=0.20000E+04;
	double rfabd=0.94960E+00;
	double rfabx=0.80000E+00;
	double rfcdft=0.00000E+00;
	double rkc=0.58000E-05;
	double rmult=0.10000E+01;
	double rnauad=0.00000E+00;
	double rnaull=0.30000E+00;
	double rnauul=0.10000E+02;
	double rps=0.00000E+00;
	double rvrs=0.19200E+02;
	double sta=0.00000E+00;
	double tens=0.10000E+01;
	double tens1=0.10000E+01;
	double tens2=0.10000E+01;
	double tensgn=0.00000E+00;
	double timetr=0.00000E+00;
	double trnstm=0.00000E+00;
	double trnsfs=0.00000E+00;
	double trpl=0.00000E+00;
	double trrbc=0.00000E+00;
	double tvz=0.98981E-03;
	double vg=0.11372E+02;
	double vid1=0.10000E-01;
	double vp1=0.10000E-01;
	double vtsf=0.60000E+01;
	double vud1=0.10047E-02;
	double vud2=0.10000E-03;
	double x=0.40000E+01;
	double z=0.10000E+01;
	double z4=0.50000E+00;
	double z5=0.30000E+00;
	double z6=0.20000E+02;
	double z7=0.50000E+01;
	double z10=0.45000E+02;
	double z11=0.10000E-01;
	double z12=0.50000E+01;
	double z13=0.62500E+00;
	double z14=0.50000E+01;
	double z16=0.50000E+01;
	double z18=0.50000E+01;
	double z19=0.50000E+01;
	double ram=0.19260E+03;

	double *parameters_morris[114] = { &a1k, &a2k, &a3k, &a4k, &a4k2, &aark,
			&adhpam, &adhtc, &ah10, &ah11, &ah9, &ahmnar, &ahthm, &alclk,
			&aldmm, &amcsns, &amkm, &amkmul, &amnam, &amt, &ancsns, &anmald,
			&anmam, &anmem, &anmkem, &anmnam, &anmslt, &anmtm, &anptc, &anpxaf,
			&ant, &anum, &anv, &any, &auc1, &audmp, &auk, &aumk1, &aun1, &aus,
			&autogn, &autok, &autosn, &auv, &aux, &barotc, &cfc, &cnr, &cpf,
			&cpr, &cv, &dhdtr, &diuret, &dr, &dtnar, &eark, &exc, &excml, &exe,
			&fis, &gflc, &gfndmp, &gppd, &hm6, &hm8, &hmk, &hmtrns, &hsl, &hsr,
			&htauml, &hyl, &kid, &korgn, &kortc, &lppr, &mdflkm, &mdflw2,
			&mdflwx, &nid, &o2a, &o2chmo, &o2m, &omm, &pcr, &pm5, &por, &pxtp,
			&qrf, &rabsc, &rcdfpc, &rek, &rfabdm, &rfabdp, &rfabkm, &rnagtc,
			&rnaugn, &rtppr, &rtpprs, &rtsprs, &rvsm, &sr, &sr2, &srk, &srk2,
			&tenstc, &tsslml, &tssltc, &tvddl, &u, &urform, &vidml, &vntstm,
			&vptiss, &vv9 };

	double *variables_morris[296] = { &au4, &au2, &au8, &amk, &amna, &amrbsc,
			&amr, &amr1, &am1, &am, &amc, &anu, &anuvn, &anm, &mdflw3, &angscr,
			&anx, &anx1, &anpr, &anpr1, &anc, &anpx, &anpl, &anpr2, &anp,
			&anp1, &anpc, &adhc, &adhmk, &adhmv, &adhna, &adhpa, &adhpr, &adh,
			&adhz, &au, &au6, &auh, &aum, &aur, &ave, &pa1, &vvr, &auo, &auc,
			&auc2, &aub, &a1b, &aun, &aulp, &auttl, &dau, &au1, &rmult1, &vrc,
			&hm, &hm1, &vb, &vim, &vie, &vib, &po2am1, &hm3, &hm4, &hm5, &hm7,
			&rc1, &rc2, &rcd, &prp, &cpp, &pc, &ppc, &vp, &vtc, &vpd, &cppd,
			&dlp, &prcd, &vtcpl, &dpc, &dpp, &vts, &vts2, &vts1, &dpi, &tsp,
			&cpi, &dpl, &ptcpr, &chy, &pgh, &poshyl, &ptc, &ptt, &pif, &pts,
			&pld, &vtl, &vif, &bfm, &bfn, &myogrs, &pa, &pamkrn, &pla, &pra,
			&ppa, &pvs, &qao, &qlo, &qro, &rpv, &rvs, &vae, &vle, &vpe, &vre,
			&vve, &vbd, &vvs, &vas, &vla, &vpa, &vra, &pam, &rad, &pamk, &pa2,
			&pra1, &qrn, &pp1, &cpa, &rpa, &pp2, &rvm, &pla1, &qln, &pl1, &rpt,
			&pgl, &qpo, &pr1, &rvg, &qvo, &cn3, &rv1, &pgs, &r1, &rsn, &rsm,
			&fisflo, &sysflo, &qlo1, &hpef, &dvs, &dpa, &das, &dla, &dra, &cna,
			&cke, &vec, &vtw, &ned, &amk1, &ktotd, &ktot, &vic, &cki, &nae,
			&ccd, &vid, &hmd, &dhm, &hpl, &hpr, &nod, &kod, &vud, &rbf, &mdflw,
			&par, &aumk, &anmer, &anmar, &rnaug1, &rnaug2, &rnaug3, &aar, &ear,
			&rr, &rfn, &efafpr, &glpc, &apd, &glp, &pfl, &rcprs, &rtsppc,
			&rabspr, &rfab1, &rfab, &rfabk, &dtnai, &dtnara, &dtki, &anmke,
			&mdflk, &dtksc, &dtnang, &nodn, &dtka, &kodn, &osmopn, &vudn,
			&plur, &plurc, &dturi, &urod, &amm, &pdo, &poe, &amm1, &amm2, &aom,
			&pmo, &rmo, &mmo, &ovs, &pvo, &do2m, &qom, &p2o, &arm, &pod, &pob,
			&ar1, &poa, &ar2, &poc, &ar3, &dob, &pot, &osv, &pov, &mo2, &do2n,
			&qo2, &p1o, &dfp, &ppd, &vpf, &pcp, &ppi, &cpn, &pos, &plf, &ppo,
			&ppn, &ppz, &pfi, &dfz, &ppr, &o2util, &alvent, &po2alv, &rspdfc,
			&o2dfs, &dova, &o2vtst, &o2vts2, &do2vad, &o2vad1, &o2vad2, &osa,
			&ova, &po2art, &pmc, &pms, &pmp, &hr, &rtp, &svo, &vv6, &vv7, &tvd,
			&sth, &ke, &ki, &atrrfb, &atrvfb, &ahz, &ahy, &ah7, &gfr };


	double *variables[VAR_COUNT]= {&au4, &au2, &au8, &amk, &amna, &amrbsc, &amr, &amr1,
			&am1, &am, &amc, &anu, &anuvn, &anm, &mdflw3, &angscr, &anx, &anx1,
			&anpr, &anpr1, &anc, &anpx, &anpl, &anpr2, &anp, &anp1, &anpc, &adhc,
			&adhmk, &adhmv, &adhna, &adhpa, &adhpr, &adh, &adhz, &au, &au6, &auh,
			&aum, &aur, &ave, &pa1, &vvr, &auo, &auc, &auc2, &aub, &a1b, &aun,
			&aulp, &auttl, &dau, &au1, &rmult1, &vrc, &hm, &hm1, &vb, &vim, &vie,
			&vib, &po2am1, &hm3, &hm4, &hm5, &hm7, &rc1, &rc2, &rcd, &prp, &cpp,
			&pc, &ppc, &vp, &vtc, &vpd, &cppd, &dlp, &prcd, &vtcpl, &dpc, &dpp,
			&vts, &vts2, &vts1, &dpi, &tsp, &cpi, &dpl, &ptcpr, &chy, &pgh, &poshyl,
			&ptc, &ptt, &pif, &pts, &pld, &vtl, &vif, &bfm, &bfn, &myogrs, &pa,
			&pamkrn, &pla, &pra, &ppa, &pvs, &qao, &qlo, &qro, &rpv, &rvs, &vae,
			&vle, &vpe, &vre, &vve, &vbd, &vvs, &vas, &vla, &vpa, &vra, &pam, &rad,
			&pamk, &pa2, &pra1, &qrn, &pp1, &cpa, &rpa, &pp2, &rvm, &pla1, &qln,
			&pl1, &rpt, &pgl, &qpo, &pr1, &rvg, &qvo, &cn3, &rv1, &pgs, &r1, &rsn,
			&rsm, &fisflo, &sysflo, &qlo1, &hpef, &dvs, &dpa, &das, &dla, &dra,
			&cna, &cke, &vec, &vtw, &ned, &amk1, &ktotd, &ktot, &vic, &cki, &nae,
			&ccd, &vid, &hmd, &dhm, &hpl, &hpr, &nod, &kod, &vud, &rbf, &mdflw,
			&par, &aumk, &anmer, &anmar, &rnaug1, &rnaug2, &rnaug3, &aar, &ear,
			&rr, &rfn, &efafpr, &glpc, &apd, &glp, &pfl, &rcprs, &rtsppc, &rabspr,
			&rfab1, &rfab, &rfabk, &dtnai, &dtnara, &dtki, &anmke, &mdflk, &dtksc,
			&dtnang, &nodn, &dtka, &kodn, &osmopn, &vudn, &plur, &plurc, &dturi,
			&urod, &amm, &pdo, &poe, &amm1, &amm2, &aom, &pmo, &rmo, &mmo, &ovs,
			&pvo, &do2m, &qom, &p2o, &arm, &pod, &pob, &ar1, &poa, &ar2, &poc,
			&ar3, &dob, &pot, &osv, &pov, &mo2, &do2n, &qo2, &p1o, &dfp, &ppd,
			&vpf, &pcp, &ppi, &cpn, &pos, &plf, &ppo, &ppn, &ppz, &pfi, &dfz,
			&ppr, &o2util, &alvent, &po2alv, &rspdfc, &o2dfs, &dova, &o2vtst,
			&o2vts2, &do2vad, &o2vad1, &o2vad2, &osa, &ova, &po2art, &pmc, &pms,
			&pmp, &hr, &rtp, &svo, &vv6, &vv7, &tvd, &sth, &ke, &ki, &atrrfb,
			&atrvfb, &ahz, &ahy, &ah7, &gfr, &t, &i, &agk, &ahk, &ahth, &amm3,
			&anmsml, &anmth, &anpinf, &anpkns, &ar4, &ar5, &atrfbm, &atrvm, &au6a,
			&au6b, &au6c, &aurg, &autoC, &auy, &crrflx, &gbl, &gfn, &glpca, &hkm,
			&i1, &i5, &i9, &i11, &i13, &i15, &i17, &i19, &i21, &korner, &korren,
			&lvm, &mdflw1, &osmop1, &pa3, &pcd, &pgp, &pgv, &po2amb, &po2ar1,
			&ptfl, &ram, &rfabd, &rfabx, &rfcdft, &rmult, &rnaull, &rnauul, &rps,
			&tens, &tens1, &tens2, &trnstm, &trrbc, &tvz, &vg, &vud1, &vud2};

	double *parameters[PARAM_COUNT] = { &a1k, &a2k, &a3k, &a4k, &a4k2, &aark, &adhpam,
			&adhtc, &ah10, &ah11, &ah9, &ahmnar, &ahthm, &alclk, &aldmm,
			&amcsns, &amkm, &amkmul, &amnam, &amt, &ancsns, &anmald, &anmam,
			&anmem, &anmkem, &anmnam, &anmslt, &anmtm, &anptc, &anpxaf, &ant,
			&anum, &anv, &any, &auc1, &audmp, &auk, &aumk1, &aun1, &aus,
			&autogn, &autok, &autosn, &auv, &aux, &barotc, &cfc, &cnr, &cpf,
			&cpr, &cv, &dhdtr, &diuret, &dr, &dtnar, &eark, &exc, &excml, &exe,
			&fis, &gflc, &gfndmp, &gppd, &hm6, &hm8, &hmk, &hmtrns, &hsl, &hsr,
			&htauml, &hyl, &kid, &korgn, &kortc, &lppr, &mdflkm, &mdflw2,
			&mdflwx, &nid, &o2a, &o2chmo, &o2m, &omm, &pcr, &pm5, &por, &pxtp,
			&qrf, &rabsc, &rcdfpc, &rek, &rfabdm, &rfabdp, &rfabkm, &rnagtc,
			&rnaugn, &rtppr, &rtpprs, &rtsprs, &rvsm, &sr, &sr2, &srk, &srk2,
			&tenstc, &tsslml, &tssltc, &tvddl, &u, &urform, &vidml, &vntstm,
			&vptiss, &vv9, &aarll, &adhinf, &adhkll, &adhkul, &adhpul, &adhvll,
			&adhvul, &aldinf, &aldkns, &am1ll, &am1ul, &amm1lm, &amm4, &amnall,
			&amnaul, &anginf, &angkns, &anmarl, &anmkel, &anmll, &anmul,
			&anpxul, &anull, &anuvm, &anxm, &ar1lm, &ar2lm, &arf, &aul, &aulpm,
			&aum1, &aum2, &aumax, &aumin, &auslp, &ckeex, &cmptss, &cn2, &cn7,
			&cpk, &dtnarl, &earll, &efafr, &excxp, &excxp2, &gcopf, &gfnll,
			&i2, &i3, &i4, &i6, &i8, &i10, &i12, &i14, &i16, &i18, &i20, &lpde,
			&lpk, &mdfl1, &mdmp, &pa4, &paex, &pce, &pghf, &pk2, &pl2, &pldf,
			&po2adv, &pok, &pom, &pom2, &pon, &poz, &pr1ll, &qaolm, &raprsp,
			&rar, &rcdfdp, &rkc, &rnauad, &rvrs, &sta, &tensgn, &timetr,
			&trnsfs, &trpl, &vid1, &vp1, &vtsf, &x, &z, &z4, &z5, &z6, &z7,
			&z10, &z11, &z12, &z13, &z14, &z16, &z18, &z19 };

	/**
	 * This struct holds pointers to all model parameters.
	 */
	PARAMS p;

	/**
	 * This struct holds pointers to all state variables.
	 */
	VARS v;

	/* Initialise the PARAMS struct (p). */
	PARAMS_INIT(p);
	/* Initialise the VARS struct (V). */
	VARS_INIT(v);

	/* Allow parameter values to be defined in an external file. */
	switch (argc) {
	case 1:
	  /* No arguments, only the command name. */
	  break;
	case 2:
	  /* One argument, which specifies the parameters file. */
	  read_params(p, argv[1]);
	  break;
	default:
	  cerr << "USAGE: " << argv[0] << " [parameter file]" << endl;;
	  exit(EXIT_FAILURE);;
	}

	double fun1[12] = { 0.0, 1.04, 60.0, 1.025, 125.0, 0.97, 160.0, 0.88,
			200.0, 0.59, 240.0, 0.0 };
	// fun2: pra,qrn,fun2
	double fun2[10] = { -8., 0., -6., .75, -2., 2.6, 4., 9.8, 12., 13.5 };
	// fun3: pp2,rvm,fun3
	double fun3[12] = { 0., 1.06, 32, .97, 38.4, .93, 48., .8, 60.8, .46, 72.,
			0. };
	// fun4: pla,qln,fun4
	double fun4[10] = { -2., .01, 1., 3.6, 5., 9.4, 8., 11.6, 12., 13.5 };
	// fun6: vts, vg, fun6
	double fun6[12] = { 0., 0., 12., 11.4, 15., 14., 18., 16., 21., 17.3, 24.,
			18. };
	double time[]={10070,10075.0,10130.0,11510.0,40140.0};
	//###########################################################################################################################################
	//########################################SENSITIVITY ANALYSIS OF THE ORIGINAL GUYTON MODEL(92 Version)######################################
	//###########################################################################################################################################
	//################################################PROGRAM MADE BY BAZIN JEROME 19/01/10######################################################
	//###########################################################################################################################################
	//###########################################################################################################################################
	//########################## The purpose of this program is to produce a matrix of normalized output/paramters per ##########################
	//########################## percentage  of modification  on a given time of the simulation. The name of the       ##########################
	//########################## produced files are significants ... Normalized_.percentage.60min.res                  ##########################
	//########################## We begin every simulations with a vector of stabilized variables (each simulations    ##########################
	//########################## begin in a stabilized state). The future versions will allow the introduction of      ##########################
	//########################## benchmarked variables                                                                 ##########################
	//##########################################################################################################################################
	//########################### Fortran is quite old but this program is compatible with the G77, G90, G95 and       ##########################
	//########################### GFortran. We advise you to process it with Gfortran or Highter  (Performances)       ##########################
	//###########################################################################################################################################


	int iptm = 0; //!reinitialize prtime vector increment

	t = 0.0; //!t is in minutes

	i = 0.0030; //!TIME INCREMENTATION STEP

	double tend = 60.0 * 24.0 * 14.0 * 2; //!*120.0  !t is in minutes *7.0 ainsi la simulation dure une semaine
	//tend= tend*4.0;
	double ptm = 1.0; //!60.0;   //!print time increment

	double prtime = 0.001;
	double thours = t / 60.0;


	cout << "t: " << t << "    pa: " << pa << endl;
	int indexTime=0;

	while (t < tend) {
		//cout << "t: "<< t << "   pa: " << pa << endl;
		if (t >= time[indexTime] and indexTime<5){
			cout << time[indexTime] << "\t" << "t: "<< t << "   pa: " << pa << endl;
			++indexTime;

		}

		//##########################################################################################################################################
		//##########################################################################################################################################

		//##########################################################################################################################################
		//##########################################################################################################################################
		//#################################################### START OF THE GUYTON MODEL ###########################################################
		//##########################################################################################################################################
		//##########################################################################################################################################
		//##########################################################################################################################################
		//##########################################################################################################################################


		if (aurg <= 0.) {

			poz = 0.;

			pon = 0.;

			pok = 0.;

			pom = 0.;

			pom2 = 0.;

		}

		//

		if (rps > 0.) {

			rar = 60.;

			ram = 180.;

		}

		//##########################################################################################################################################
		//#################################################### Circulatory dynamics part ###########################################################
		//##########################################################################################################################################

#ifdef MODULAR
		module_circdyn(p, v);
#else
		//    /* blood volume change partitioned into the various circulation sections */


		vbd = vp + vrc - vvs - vas - vla - vpa - vra;

		vvs = vvs + dvs * i2 + vbd * .3986;/* 40% venous */

		vpa = vpa + dpa * i2 + vbd * .155; /* 15% pulmonary */

		vas = vas + das * i2 + vbd * .261; /* 26% arterial */

		vla = vla + dla * i2 + vbd * .128; /* 13% left atrium */

		vra = vra + dra * i2 + vbd * .0574; /*  6% right atrium */

		//    /* arterial compartment */

		vae = vas - .495;/* excess vol = vol - unstressed vol */

		pa3 = pa;

		//        pa=vae/.00355   ;/* PA = excess vol / arterial compliance */
		pa = vae / .00155;/// Ici je reduis la compliance artŽrielle afin de pouvoir simuler un hypertendu dont les vaisseaux sont - souples

		pam = pow(pa / 100., paex);//**paex  ;/* pressure effect on art. distension */

		// --- /* special feature: MYOGENIC AUTOREGULATION */

		myogrs = 1.;

		if (tensgn > 0.) {

			tens1 = (pa + pc) / 117. * pow(pam, 0.25);//pam**.25;

			tens2 = (tens1 - 1.) / (tensgn + 1.) + 1.;

			tens = tens1 * rad;

			rad = rad + (tens2 - tens) / tenstc * i2;

			myogrs = pow(1. / rad, 4.);//**4.;

		}

		// --- korner effect

		// SRT: see: Jennings, G. L., Nelson, L., Esler, M. D., Leonard, P. & Korner, P. I. 1984

		//   Effects of changes in physical activity on blood pressure and sympathetic tone.

		//     J Hypertens Suppl 2, S139-41.


		pamkrn = 1.;

		if (korner > 0) {

			pamk = pamk + ((pa / 100. - 1.) * korgn + 1. - pamk) / kortc * i;

			if (korren > 0)
				pamkrn = pamk;

		}

		// 				/* pressure effect on left ventr. pumping */

		pa2 = pa / auh / osa;

		//        call funct (pa2,lvm,fun1,12)

		//call funct(pa2,lvm,fun1);
		funct(&pa2, &lvm, fun1, 12);

		vre = vra - .1;/* volume in excess */

		pra = vre / .005;/* pressure = excess vol / compliance */

		pra1 = (pra + 8.) * (htauml * (au - 1.) + 1.) - 8.;

		//        call funct (pra1,qrn,fun2,10)

		//call funct (pra1,qrn,fun2)   ;/* Starling right heart */
		funct(&pra1, &qrn, fun2, 10);/* Starling right heart */

		//

		//    /* pulmonary vasculature */

		//

		vpe = vpa - .30625;/* volume in excess */

		ppa = vpe / .0048;/* pressure = excess vol / compliance */

		// 				/* pressure effect on art. pulm. distension */

		pp1 = .026 * ppa;

		if (pp1 < .00001)
			pp1 = .00001;

		cpa = pow(pp1, 0.5);

		rpa = 1. / cpa;

		//				/* pressure effect on right ventric. pumping */

		pp2 = ppa / auh / osa;

		//        call funct (pp2,rvm,fun3,12)

		//call funct(pp2,rvm,fun3);
		funct(&pp2, &rvm, fun3, 12);

		//

		//   /* left atrium */

		//

		vle = vla - .38;/* volume in excess */

		pla = vle / .01;/* pressure = excess vol / compliance */

		pla1 = (pla + 4.) * (htauml * (au - 1.) + 1.) - 4.;

		//        call funct (pla1,qln,fun4,10)

		//call funct (pla1,qln,fun4)  ;/* Starling left heart */
		funct(&pla1, &qln, fun4, 10);/* Starling left heart */

		pl1 = pla + 18.;/* atrial presure effect on pulm vein distension */

		rpv = 1. / pl1 / .0357;

		rpt = rpv + rpa;/* Total pulmon. resistance, Delta P and Flow */

		pgl = ppa - pla;

		qpo = pgl / rpt;

		//

		//   /* venous system */

		//

		//		/* unstressed vol = f (angiotensin, stress relax, vol. receptors) */

		vve = vvs - vvr - (anu - 1.) * any - vv7 - vv6 - atrvfb;

		pvs = 3.7 + (vve - .74) / cv;/* pressure = excess vol / compliance */

		if (pvs < .0001)
			pvs = .0001;

		pr1 = pra;

		if (pra < pr1ll)
			pr1 = pr1ll; /* venous collapse phenomenon */

		pgv = pvs - pr1;
		;/* venous pressure and resistance computations */

		rvg = .74 / pow((pvs / vim / 3.7), 0.5);

		qvo = pgv / rvg;

		cn3 = cn3 + (((pc - 17.) * cn7 + 17.) * cn2 - cn3) * .1;

		rv1 = rvsm / cn3;

		rvs = ave * rv1 * vim * anuvn;

		pgs = pa - pvs;

		//

		//   /* systemic blood flows and resistances */

		//

		r1 = anu * adhmv * aum * vim * pamk / pam / atrrfb;

		rsn = rar * arm * r1 * myogrs * rmult1 + rvs * 1.79;/* nonmuscle, nonrenal */

		bfn = pgs / rsn;

		rsm = amm * ram * r1 * myogrs * rmult1;/* muscle */

		//cNB: turboc version:	RSM = RAM*AMM*R1;				/* muscle */

		bfm = pgs / rsm;

		fisflo = (pa - pra) * fis;/* optional AV fistula */

		sysflo = bfm + bfn + rbf;

		qao = sysflo + fisflo;

		qlo = lvm * qln * auh * hsl * hmd * hpl;/* left ventricular output */

		qlo1 = (pla - pa) / 3.;/* passive flow due to elevated PLA */

		if (qlo1 > 0.)
			qlo = qlo + qlo1;

		hpef = (1. - qrf) * auh * rvm * hsr * hmd * hpr + qrf * qlo / qln;

		qro = qrn * hpef;/* right ventricular output */

		qpo = qlo + (qpo - qlo) / u;

		qvo = qro + (qvo - qro) / x;

		//

		//   /* volume change in each circulation section = inflow - outflow */

		//

		dvs = qao - qvo;/* venous */

		dpa = qro - qpo;/* pulmonary */

		das = qlo - qao;/* arterial */

		dla = qpo - qlo;/* left atrium */

		dra = qvo - qro;/* right atrium */
#endif

		//##########################################################################################################################################
		//#################################################### Autonomic control of the circulation #################################################
		//##########################################################################################################################################

#ifdef MODULAR
		if (! module_autonom(p, v)) {
			continue;
		}
#else
		////////if(sta-aumin)120,120,116     	;/* to fix autonomic output */
		if (sta - aumin > 0) {
			au = sta;
		} else {

			//si 1 donc true 116 et 120
			//si 0 donc false 120

			//si >0 116 et 128 pas 120
			//si <=0 120

			///////116     au=sta;

			////////goto 128   ; //!<<<<<<<<<<<<<<<<<<<<<<<???????????????????????

			//

			pa1 = pa - exe; /* PA1: pressure driving autonomic receptors 120*/

			if (crrflx > .0000001)
				pa1 = crrflx;

			//

			//				/* chemoreceptors: effect of PA */

			auc = 0.;

			if (pa1 < 80.)
				auc = .005 * (80. - pa1);
			if (pa1 < 40.)
				auc = .2;
			auc = auc * auc1;

			//				/* chemoreceptors: effect of art PO2 */

			auc2 = 0.;

			if (po2art <= 40.) {
				auc2 = o2chmo * 40.;
			}

			else if (po2art <= 80.) {
				auc2 = (80. - po2art) * o2chmo;
			}

			// 					/* arterial baroreceptors */
			aub = 0.;//123


			if (pa1 < 160.)
				aub = .016667 * (160. - pa1);

			if (pa1 < 80.)
				aub = 1.3336;

			a1b = (aub - 1.) * aux + 1.;

			//					/* CNS ischemic reflex */

			aun = 0;///124

			if (pa1 < 40.)
				aun = .04 * (40. - pa1);

			aun = aun * aun1;

			au6a = a1b - au4;

			au6b = au6;

			au6 = (au6a - au6) / barotc * i + au6;

			if (i > .166666)
				au6 = au6a;

			au6c = au6 + (au6 - au6b) * mdmp;

			aulp = (15. / (pla + pra + ppa) - 1.) * aulpm + 1.;

			//auttl=(aulp*(auc+auc2+au6c+aun)*exc**excxp-1.)*excml+1.;
			auttl = (aulp * (auc + auc2 + au6c + aun) * pow(exc, excxp) - 1.)
							* excml + 1.;
			if (auttl < 0.)
				auttl = 0.;

			dau = auttl - au1;

			au1 = au1 + dau * i2 / audmp;

			//au=aumax-(aumax-1.)/2.7183**(auslp*(au1-1.));
			au = aumax - (aumax - 1.) / pow(2.7183, auslp * (au1 - 1.));

			if (au < aumin)
				au = aumin;

			//

			//   /* autonomic drive on the various target organs or tissues */

			//
		}
		auo = au - 1.;//128

		auh = auo * auv + 1.;/* heart strength */

		aur = auo * aus + 1.;/* heart rate */

		vvr = vv9 - (au * aul) + aul;/* basic venous volume */

		aum = pow(auo * aum1 + 1., aum2);/* arterial resistance */

		ave = auo * auy + 1.;/* venous resistance */

		i5 = i5 + i2;

		if ((i5 <= i) and (fabs(pa - pa3) > pa4 or fabs(pa - pa3) > pa4 or fabs(
				qao - qlo) > qaolm or fabs(qao - qpo) > qaolm or fabs(qao - qro)
		> qaolm * 2)) {//goto 140;
			continue;
		}
		//if(fabs(pa-pa3) > pa4)goto 100;

		//

		//-------stability test: repeat short loop (DT = I2) if change too large

		//

		//if (fabs(pa-pa3) > pa4) goto 100  ;//NB: imported this from turboc version, TCIRC92

		//if (fabs(qao-qlo) > qaolm) goto 100;

		//if (fabs(qao-qpo) > qaolm) goto 100;

		//if (fabs(qao-qro) > qaolm*2) goto 100;


		//

		//--------------time test

		//    /* ---  computes long loop variable incrementation step (DT = I) --- */

		i5 = 0.;//140

		i = i * 1.2; /* adds 20% to current I */

		if (i3 < i)
			i = i3; /* I3 = upper limit */

		i1 = fabs(vp1 / vpd / i); /* VP1/VPD = overall stability test */

		if (i1 < i)
			i = i1;

		t = t + i;

#endif
		//

		//##########################################################################################################################################
		//############################################################# Aldosterone ################################################################
		//##########################################################################################################################################

#ifdef MODULAR
		module_aldost(p, v);
#else
		///*

		// block input  = CKE (from Electrolytes block)

		//		ANM (from Angiotensin block);

		// block output = AMK (to DTKSC, renal potassium; AMK1, electrolytes)

		//		AMNA (to DTNARA, renal sodium reabsorption);

		//*/

		//

		amrbsc = ((anm - 1.) * anmald + 1.) * .909 * (cke - 3.3); /* aldo secretion   170*/

		amr = (amrbsc - 1.) * amkmul + 1.;

		if (amr < 0.)
			amr = 0.;

		amr1 = amr + aldinf; /* } + exo */

		if (aldkns > 0.)
			amr1 = aldkns;

		//amc=amc+(amr1-amc)*(1.-1./2.7183**(i/amt));
		amc = amc + (amr1 - amc) * (1. - 1. / pow(2.7183, i / amt));

		am1 = am1ul - (am1ul - 1) / ((am1ll - 1) / (am1ll - am1ul) * (amc - 1)
				* amcsns + 1);

		am = (am1 - 1.) * aldmm + 1.;

		amk = (am - 1.) * amkm + 1.;/* effect on potassium transport */

		if (amk < .2)
			amk = .2;

		amna = (am - 1.) * amnam + 1.;/* effect on sodium reabsorption */

		if (amna < amnall)
			amna = amnall;

		if (amna > amnaul)
			amna = amnaul;

#endif
		//

		//##########################################################################################################################################
		//############################################################# Angiotensin ################################################################
		//##########################################################################################################################################

#ifdef MODULAR
		module_angio(p, v);
#else
		// block input  = MDFLW (from Kidney block);

		// block output = ANU   (to VVE, venous volume; R1, composite arterial resistance)

		//		ANUVN (to RVS, venous resistance)

		//		ANM   (to AMRBSC, aldo secretion; ANMSML, salt intake drive;

		//		       ANMER + ANMAR : pre and post-glomerular resistances;

		//		       DTNANG, renal sodium reabsorption);

		//*/

		//

		mdflw3 = mdflw3 + (mdflw - mdflw3) * mdflwx;

		if (mdflw3 > 1.)
			angscr = 1. / (1. + (mdflw3 - 1.) * 72.);

		if (mdflw3 <= 1.)
			angscr = 10. - 9. / (1. + (1. - mdflw3) * 8.);

		anx = (angscr - 1.) * anxm;

		anx1 = anx1 + (anx - anx1) / anv * i;

		anpr = (angscr + anx1) * rek;

		if (anpr < .00001)
			anpr = .00001;

		anpr1 = anpr + anginf; /* } + exo */

		if (angkns > 0.)
			anpr1 = angkns;

		//anc=anc+(anpr1-anc)*(1.-1./2.7183**(i/ant/z12));
		anc = anc + (anpr1 - anc) * (1. - 1. / pow(2.7183, i / ant / z12));

		anm = anmul - (anmul - 1) / ((anmll - 1) / (anmll - anmul) * (anc - 1)
				* ancsns + 1);

		anu = (anm - 1.) * anum + 1.;

		if (anu < anull)
			anu = anull;

		anuvn = (anu - 1.) * anuvm + 1.;
#endif

		//##########################################################################################################################################
		//############################################################# ATRIAL NATRIURETIC PEPTIDE #################################################
		//##########################################################################################################################################

#ifdef MODULAR
		module_anp(p, v);
#else
		///*

		// block input  = PLA, PRA (from Circulation);

		// block output = ANPX (to AAR, afferent resistance);

		//*/

		//

		anpl = pla - 1.;/* left atrium ANP */

		if (anpl < 0.)
			anpl = 0.;

		anpr2 = (pra + 1.) * 2.;/* more sensitive to PRA than PLA */

		if (anpr2 < 0.)
			anpr2 = 0.;

		anp = (anpl + anpr2) / 3.;

		anp1 = anp + anpinf;/* } + exo */

		if (anpkns > 0.)
			anp1 = anpkns;/* to fix ANP */

		//anpc=anpc+(anp1-anpc)*(1.-1./2.7183**(i/anptc/z14));
		anpc = anpc + (anp1 - anpc) * (1. - 1. / pow(2.7183, i / anptc / z14));

		anpx = anpxul - anpxul / (.5555556 * (1. + anpc));

		if (anpx < -1.)
			anpx = -1.;
#endif

		//##########################################################################################################################################
		//############################################################# RED CELLS AND VISCOSITY ####################################################
		//##########################################################################################################################################

#ifdef MODULAR
		module_rbc(p, v);
#else

		// block input  = VP  (from Capillary dynamics);

		// block output = VRC (to VBD, Circulation)

		//		HM  (to OSA, art O2 saturation; to OSV and OVS, venous saturations);

		//*/

		//

		//  blood viscosity

		//

		vb = vp + vrc;

		hm1 = vrc / vb;/* hematocrit */

		hm = 100. * hm1;

		vie = hm / (hmk - hm) / hkm; /* viscosity due to red blood cells */

		vib = vie + 1.5;

		vim = .3333 * vib;

		//

		//  /* --- red blood cells formation and destruction --- */

		//

		po2am1 = po2amb; /* oxygen and hematocrit stimulus */

		if (po2am1 > 80.)
			po2am1 = 80.;

		hm3 = (po2am1 - 40.) * hm;

		hm4 = po2amb - 40.;

		hm5 = hm3 + hm4;

		if (hm5 < 0.)
			hm5 = 0.;

		hm7 = hm6 - hm5;

		//    /* red cell kinetics */

		rc1 = hm7 * hm8 * rek + .000005;/* RBC production */

		if (rc1 < 0.)
			rc1 = 0.;

		rc2 = vrc * rkc * vim;/* RBC destruction */

		rcd = rc1 - rc2 + trrbc;

		vrc = vrc + rcd * i;

		//

		// --pulmonary o2 uptake into blood

		//

		// 				/* alveolar ventilation and PO2 */

		o2util = dob + rmo;

		alvent = o2util * vntstm * .026667 * o2vts2 * o2vad2;

		po2alv = po2amb - o2util / alvent / .761;

		rspdfc = pl2 / (vptiss + vpf);/* resistance to O2 diffusion */

		i9 = 0; // !NB: imported from TCIRC92

		// 				/* arterial PO2 computation loop */

		do {
			po2ar1 = po2art;//178

			o2dfs = (po2alv - po2art) * rspdfc;/* pulmonary O2 diffusion */

			dova = (o2dfs - o2util) / qro;

			ova = ova + dova * i8;/* art O2 content */

			osa = ova / hm / 5.25;/* art oxygen saturation */

			// 						/* function curve OSA - PO2ART */

			/*if(osa > .8)goto 181;

			 po2art=osa*57.5;

			 goto 184;

			 181     if(osa > .936)goto 182;

			 po2art=46.+(osa-.8)*205.882;

			 goto 184;

			 182     if(osa > 1.)goto 183;

			 po2art=74.+(osa-.936)*625.;

			 goto 184;

			 183     po2art=114.+(osa-1.)*6667.;

			 184     i9=i9+i8;*/
			if (osa > 1.) {
				po2art = 114. + (osa - 1.) * 6667.;
			} else if (osa > .936) {
				po2art = 74. + (osa - .936) * 625.;
			} else if (osa > .8) {
				po2art = 46. + (osa - .8) * 205.882;
			} else {
				po2art = osa * 57.5;
			}
			i9 = i9 + i8;

			if (i9 > i) {//goto 186;

				//if(fabs(po2art-po2ar1) > po2adv)goto 178       ;/* iteration until stable */

				i9 = 0.;//186
				break;
			}

		} while (fabs(po2art - po2ar1) > po2adv);

		o2vtst = (po2art - 67.) / 30.;

		if (o2vtst > 1.)
			o2vtst = 1.;
		;/* limits */

		if (o2vtst < .6)
			o2vtst = .6;

		o2vts2 = 1. / o2vtst;

		do2vad = ((o2vts2 - 1.) * 3. - o2vad1) * .0005;

		o2vad1 = o2vad1 + do2vad * i;

		o2vad2 = o2vad1 + 1.;
#endif

		//##########################################################################################################################################
		//########################################### O2 DELIVERY and BLOOD FLOW AUTOREGULATION ####################################################
		//##########################################################################################################################################


		// muscle o2 delivery

		//

		aom = auo * o2a + 1.;

		//mmo=aom*omm*exc*(1.-(38.0001-p2o)**3./54872.)    ;/* O2 consumption */
		mmo = aom * omm * exc * (1. - pow(38.0001 - p2o, 3.) / 54872.);/* O2 consumption */

		ovs = ovs + ((bfm * ova - rmo) / hm / 5.25 / bfm - ovs) / z6;

		//pvo=57.14*ovs*exc**excxp2             ;/* venous PO2 */
		pvo = 57.14 * ovs * pow(exc, excxp2); /* venous PO2 */

		i13 = 0;//               !NB: added this just to be neat


		/*187     rmo=(pvo-pmo)*pm5*bfm    ;// muscle tissue O2 delivery

		 do2m=rmo-mmo;

		 if(fabs(do2m) < z5)goto 188;

		 qom=qom+do2m*i12          ;// muscle O2 volume

		 if(qom < .0001)qom=.0001;

		 pmo=pk2*qom               ;// muscle tissue PO2

		 i13=i13+i12;

		 if(i13 < i)goto 187;

		 188     i13=0.;*/

		do {
			rmo = (pvo - pmo) * pm5 * bfm; // muscle tissue O2 delivery
			do2m = rmo - mmo;
			if (fabs(do2m) < z5) {
				i13 = 0.;
				break;
			}
			qom = qom + do2m * i12; // muscle O2 volume
			if (qom < .0001)
				qom = .0001;
			pmo = pk2 * qom; // muscle tissue PO2
			i13 = i13 + i12;

		} while (i13 < i);

		p2o = pmo;

		if (p2o > 38.)
			p2o = 38.;

		//

		// muscle local blood flow control

		//

		pdo = pmo - 38.;/* driving PO2 for autoregulation */

		poe = pom * pdo + 1.;

		/*1882    amm3=amm1;

		 amm1=amm1+(poe-amm1)/a4k*i20;

		 //					 autoregulation, rapid

		 i21=i21+i20;

		 if(i21 > i)goto 1884;

		 if(fabs(amm1-amm3) > amm1lm)goto 1882;

		 1884    i21=0.;*/

		do {
			amm3 = amm1;
			amm1 = amm1 + (poe - amm1) / a4k * i20;
			//					 autoregulation, rapid
			i21 = i21 + i20;
			if (i21 > i) {
				i21 = 0.;
				break;
			}

		} while (fabs(amm1 - amm3) > amm1lm);

		if (amm1 < amm4)
			amm1 = amm4;

		//					/* autoregulation, long-term */

		amm2 = amm2 + (pdo * pom2 + 1. - amm2) / a4k2 * i;

		//					/* global: short + long-term */

		amm = amm1 * amm2;

		//

		// non muscle o2 delivery

		//

		osv = osv + ((bfn * ova - dob) / hm / 5.25 / bfn - osv) / z7;

		pov = osv * 57.14;/* non muscle venous PO2 */

		//mo2=aom*o2m*(1.-(35.0001-p1o)**3./42875.)      ;/* O2 consumption */
		mo2 = aom * o2m * (1. - pow(35.0001 - p1o, 3.) / 42875.);/* O2 consumption */

		i11 = 0; //!NB: added this just to be neat

		/*189     dob=(pov-pot)*12.857*bfn;

		 do2n=dob-mo2;

		 if(fabs(do2n) < z4)goto 190

		 if(qo2 < 6..and.do2n < 0.)do2n=.1*do2n;

		 qo2=qo2+do2n*i10             ;// non muscle O2 volume

		 if(qo2 < 0.)qo2=0.;

		 pot=qo2*.48611               ;// non muscle tissue PO2

		 i11=i11+i10;

		 if(i11 < i)goto 189

		 190     i11=0.;*/
		do {
			dob = (pov - pot) * 12.857 * bfn;
			do2n = dob - mo2;

			if (fabs(do2n) < z4) {
				i11 = 0.;
				break;
			}

			if (qo2 < 6. and do2n < 0.)
				do2n = .1 * do2n;

			qo2 = qo2 + do2n * i10;// non muscle O2 volume

			if (qo2 < 0.)
				qo2 = 0.;

			pot = qo2 * .48611;// non muscle tissue PO2
			i11 = i11 + i10;

		} while (i11 < i);

		p1o = pot;

		if (pot > 35.)
			p1o = 35.;

		//

		// non muscle local blood flow control

		//

		//  autoregulation, rapid

		pod = pot - por;

		pob = pod * pok + 1.;

		/*1902    ar4=ar1;

		 ar1=ar1+(pob-ar1)/a1k*i16;

		 i17=i17+i16;

		 if(i17 > i)goto 1904;

		 if(fabs(ar1-ar4) > ar1lm)goto 1902;

		 1904    i17=0.;*/

		do {
			ar4 = ar1;
			ar1 = ar1 + (pob - ar1) / a1k * i16;
			i17 = i17 + i16;
			if (i17 > i) {
				i17 = 0.;
				break;
			}
		} while (fabs(ar1 - ar4) > ar1lm);

		if (ar1 < .5)
			ar1 = .5;

		//  autoregulation, intermediate

		poa = pon * pod + 1.;

		/*1906    ar5=ar2;

		 ar2=ar2+(poa-ar2)/a2k*i18;

		 i19=i19+i18;

		 if(i19 > i)goto 1908;

		 if(fabs(ar2-ar1) > ar2lm)goto 1906;

		 1908    i19=0.;*/

		do {
			ar5 = ar2;
			ar2 = ar2 + (poa - ar2) / a2k * i18;
			i19 = i19 + i18;
			if (i19 > i) {
				i19 = 0.;
				break;
			}

		} while (fabs(ar2 - ar1) > ar2lm);

		if (ar2 < .5)
			ar2 = .5;

		//  autoregulation, long-term

		poc = poz * pod + 1.;//192

		ar3 = ar3 + (poc - ar3) * i / a3k;

		if (ar3 < 0.3)
			ar3 = 0.3;

		//						/* global */

		arm = (ar1 * ar2 * ar3 - 1.) * autosn + 1.;

		//

		//##########################################################################################################################################
		//########################################################## VOLUME RECEPTORS ##############################################################
		//##########################################################################################################################################

		// block input  = PRA (from Circulation);

		// block output = ATRRFB (to R1, composite arterial resistance)

		//		ATRVFB (to VVE, venous volume)

		//*/

		//

		ahz = pow(fabs(pra), ah10);/* volume receptor output = f (PRA) */

		if (pra <= .0)
			ahz = -ahz;

		ahz = ahz * ah9;

		ahy = ahy + (ahz - ahy) / ah11 * i;/* volume receptor adaptation */

		ah7 = ahz - ahy;

		atrrfb = ah7 * atrfbm + 1.;/* effect on arteries */

		atrvfb = ah7 * atrvm;/* effect on untressed venous volume */

		//##########################################################################################################################################
		//######################################################### ANTIDIUREETIC HORMONE ##########################################################
		//##########################################################################################################################################


		// block input  = CNA (from Electrolytes)

		//		PA1 (from Circulation);

		// block output = ADHC (to AHTH, thirst drive)

		//		ADHMK (to DTNARA, sodium reabsorption; VUDN, water excretion)

		//		ADHMV (to R1, composite arterial resistance);

		//*/

		//

		adhna = (cna - cnr) / (142. - cnr);/* osmoreceptors */

		if (adhna < 0.)
			adhna = 0.;

		adhpa = pa1;/* art pressure stimulus */

		if (adhpa > adhpul)
			adhpa = adhpul;

		adhpr = pow(adhpul - adhpa, 2.) * adhpam;/* ADH production */

		adh = adhna + adhpr + adhinf;

		if (adh < 0.0)
			adh = 0.0; // !NB: imported from TCIRC92     ;/* left out? */

		//adhc=adhc+(adh-adhc)*(1.-1./2.7183**(i/adhtc/z16));
		adhc = adhc + (adh - adhc) * (1. - 1. / pow(2.7183, i / adhtc / z16));

		//						/* ADH effect on kidneys */

		adhz = adhkll - adhkul;

		adhmk = adhkul - (adhkul - 1.) / ((adhkll - 1.) / adhz * (adhc - 1.)
				+ 1.);

		if (adhmk < adhkll)
			adhmk = adhkll;

		//					/* ADH effect on vascular resistance */

		adhz = adhvll - adhvul;

		adhmv = adhvul - (adhvul - 1.) / ((adhvll - 1.) / adhz * (adhc - 1.)
				+ 1.);

		if (adhmv < adhvll)
			adhmv = adhvll;

		//##########################################################################################################################################
		//########################################################## STRESS RELAXATION #############################################################
		//##########################################################################################################################################


		// block input  = VVE;

		// block output = VV6 + VV7 (to VVE, venous volume);

		//*/

		//

		vv6 = vv6 + ((vve - .74) * sr2 - vv6) / srk2 * i;

		//vv7=vv7+((vve-.74)*sr-vv7)*(1.-1./2.7183**(i/srk));
		vv7 = vv7 + ((vve - .74) * sr - vv7) * (1. - 1. / pow(2.7183, i / srk));

		//
		//##########################################################################################################################################
		//########################################################## THIRST AND DRINK ##############################################################
		//##########################################################################################################################################


		// block input  = ADHC (from vasopressin), ANM (from angiotensin)

		// block output = TVD (to VTW, total body water)

		//		STH (salt intake drive, to electrolytes);

		//*/

		//		/* --- tissue effect on thirst and salt intake : moved here from below --- */

		//

		anmsml = (anm - 1.) * anmslt + 1.;

		sth = pow(z10 - pot, 2.) * z11 * anmsml;

		if (sth < .8)
			sth = .8;

		if (sth > 8.)
			sth = 8.;

		ahth = ((adhc - 1.) * ahthm + 1.) * sth * .001;

		if (ahth < 0.)
			ahth = 0.;

		//						/* thirst drive */

		anmth = (anm - 1.) * anmtm * .001;

		tvz = anmth + ahth;

		if (tvz < 0.)
			tvz = 0.;

		tvd = tvd + (tvz + dr - tvd) / tvddl * i;

		//##########################################################################################################################################
		//######################################################### ADAPTATION OF BARORECEPTORS ####################################################
		//##########################################################################################################################################

		au2 = au6 - 1.;

		au8 = auk * au2;

		au4 = au4 + au8 * i;

		//

		// ---------- heart vicious cycle

		///* block input = POT;		block output = HMD */

		//

		dhm = (pot - 10.) * dhdtr;

		hmd = hmd + dhm * i;

		if (hmd > 1.)
			hmd = 1.;

		//##########################################################################################################################################
		//############################################################ SPECIAL CIRCULATORY #########################################################
		//##########################################################################################################################################


		///* --------------- SPECIAL CIRCULATORY VARIABLES ------------------------- */

		// /* These variables are used only for display, NOT as input to other blocks */

		//

		// ---------- mean circulatory pressures

		//

		pmc = (vae + vve + vre + vpe + vle) / .11;

		pms = (vae + vve + vre) / .09375;

		pmp = (vpe + vle) / .01625;

		//

		// ---------- heart rate,total peripheral resistance, stroke volume

		//

		hr = (72. * aur + pow(pr1, 0.5) * 5.) * ((hmd - 1.) * .5 + 1.);

		rtp = (pa - pra) / qao;

		svo = qlo / hr;

		//
		//##########################################################################################################################################
		//############################################################# CAPILLARY DYNAMICS #########################################################
		//##########################################################################################################################################


		// ------------------------------------------------------------------------------

		///* ----------------------------------------------------------------------- */

		///* ------ CAPILLARY DYNAMICS, TISSUE FLUID AND TISSUE PROTEIN ------------ */

		///*

		// block input  = BFN, PVS (from Circulation)

		//		DFP (from Pulmonary dynamics);

		// block output = PC (to CN3, venous pressure computation; [myogenic autoreg])

		//		VP (to VDB, circulation; VB, red cell block; [renal] );

		//*/

		//

		//  capillary membrane dynamics

		pc = rvs * 1.7 * bfn + pvs;///2000

		vtc = (pc - ppc - pgh + ptc) * cfc + vtcpl;/* Starling forces + leakage */

		//   plasma volume and protein

		vpd = vtl - vtc - dfp + trpl;/* global fluid shifts out/into plasma */

		vp = vp + vpd * i;/* plasma volume */

		cpp = prp / vp;/* plasma protein conc. */

		cppd = cpp - cpr;/* drive for protein destruction */

		if (cppd < 0.)
			cppd = 0.;

		dlp = lppr - pow(cppd, lpde) * lpk; /* protein balance */
		dlp = lppr - pow(cppd, lpde) * lpk; /* protein balance */

		//cerror        prcd=pc-pcrn                 ;/* driving pressure for plasma leakage */

		prcd = pc - pcr;/* driving pressure for plasma leakage */

		if (prcd < 0.)
			prcd = 0.;

		vtcpl = pow(prcd * cpk, pce);/* fluid leakage */

		dpc = vtcpl * cpp + (cpp - cpi) * .00104;/* protein leakage */

		dpp = dlp + dpl - dpc - ppd + trpl * 72.;/* global protein shifts out/into plasma */

		prp = prp + dpp * i; /* total plasma protein */

		ppc = .28 * cpp + .0019 * pow(cpp, 2.);/* plasma oncotic pressure */

		//   systemic tissue fluid volume


		vts = vec - vp - vpf;

		vts2 = vts2 + ((vts - 12.) * tsslml - vts2) * tssltc * i;

		vts1 = vts - vts2;

		//   tissue protein

		dpi = dpc - dpl;

		tsp = tsp + dpi * i;/* total interstitial protein */

		cpi = tsp / vts;

		dpl = cpi * vtl;/* lymph protein flow */

		ptcpr = .28 * cpi + .0019 * pow(cpi, 2.);/* interstitial protein oncotic pressure */

		//   tissue gel and fluid

		chy = pow(hyl / vts / 5., cmptss);

		pgh = chy * pghf + ptt;/* hydrostatic pressure of tissue gel */

		poshyl = chy * 2.;

		ptc = poshyl * ptcpr * gcopf;/* oncotic pressure of tissue gel */

		ptt = pow((vts1 - vtsf) / vtsf, 2.);/* total tissue pressure */

		pif = pgh - poshyl;/* interstitial free fluid pressure */

		pts = ptt - pif;/* solid tissue pressure */

		pld = pif + pldf - ptt;/* pressure drive for lymph flow */

		if (pld > 7.)
			pld = 7.;

		vtl = pld * .02;/* lymph flow */

		if (vtl < 0.)
			vtl = 0.;

		//        call funct (vts,vg,fun6,12)

		//		call funct(vts,vg,fun6)     ;/* interstitial gel vol:ume */
		funct(&vts, &vg, fun6, 12);/* interstitial gel vol:ume */

		vif = vts - vg;/* VIF, VG for display only */

		//##########################################################################################################################################
		//########################################### PULMONARY FLUID DYNAMICS ####################################################
		//##########################################################################################################################################


		// ----------  pulmonary fluid dynamics

		///* -------------------  PULMONARY FLUID DYNAMICS ------------------------- */

		///*

		// block input  = PPA, PLA (from Circulation)

		//		PPC, CPP (from Capillary dynamics);

		// block output = DFP (to VP, plasma volume)

		//		PPD (to DPP, plasma protein)

		//		VPF (to O2 uptake; VTS, tissue fluid volume);

		//*/

		//

		pcp = (ppa - pla) * rpv / (rpv + rpa) + pla;//220

		ppi = 2. - .150 / vpf;

		cpn = ppr / vpf;

		pos = cpn * .4;

		plf = (ppi + 11.) * .0003;

		ppo = plf * cpn;/* lung lymphatic protein flow */

		ppn = (cpp - cpn) * .000225;

		ppz = ppn - ppo;

		ppd = ppd + (ppz - ppd) / z18;

		if (ppr + ppd * i - .025 < 0.)
			ppd = (.025 - ppr) / i;

		pfi = (pcp - ppi + pos - ppc) * cpf;/* Starling forces at capillary */

		dfz = pfi - plf;

		dfp = dfp + (dfz - dfp) / z19;

		if (vpf + dfp * i - .001 < 0.)
			dfp = (.001 - vpf) / i;

		vpf = vpf + dfp * i;/* pulmonary fluid ... */

		ppr = ppr + ppd * i;/* ... and protein */

		//

		// ---------- hypertrophy of heart

		//

		hpl = hpl + (pow((pa * qao / 500. / hsl), z13) - hpl) * i / 57600.;

		hpr = hpr + (pow((ppa * qao / 75. / hsr), z13) - hpr) * i / 57600.;

		//

		//##########################################################################################################################################
		//#################################################### KYDNEY FLUID AND SALT OUTPUT ########################################################
		//##########################################################################################################################################

		// block input  = PA  (from Circulation)

		//		AUM (from Autonomic)

		//		ANM (from Angiotensin), AMNA (from Aldosterone)

		//		ANPX (from Atrial natriuretic peptide)

		//		HM1 (from Red cells), PPC (from Capillary dyanmics)

		//		ADHMK (from vasopressin), CNA, CKE (from Electrolytes);

		// block output = NOD (to NED, sodium balance / Electrolytes)

		//		KOD (to KTOTD, potassium balance / Electrolytes)

		//		VUD (to VTW, water balance / Electrolytes);

		//*/

		//

#ifdef MODULAR
		module_renal(p, v);
#else
		par = pa - gbl;/* Goldblatt clamp  240*/

		if (raprsp > 0.)
			par = raprsp;/* servocontrol of PAR */

		//					/* exp: renal function drift */

		if (rfcdft > 0.)
			par = par + ((100. + (pa - 100.) * rcdfpc) - par) / rcdfdp * i;

		aumk = (aum - 1.) * arf + 1.;/* autonomic effect on afferent resist */

		if (aumk < .8)
			aumk = .8;

		//			/* angiotensin effect on afferent + efferent resistances */

		anmer = (anm - 1.) * anmem + 1.;

		anmar = (anm - 1.) * anmam + 1.;

		if (anmar < anmarl)
			anmar = anmarl;

		//

		//   /* --- renal autoregulation --- */

		//

		//         /* renal autoregulation feedback in afferent + efferent */

		do {

			rnaug1 = rnaug1 + (((mdflw - 1.) * rnaugn + 1.) - rnaug1) / rnagtc;//242

			if (rnaug1 < rnaull)
				rnaug1 = rnaull;

			if (rnaug1 > rnauul)
				rnaug1 = rnauul;

			rnaug2 = rnaug1 - rnaug3;

			rnaug3 = rnaug3 + (rnaug2 - 1.) * rnauad * i4;

			double aar1 = aark * pamkrn * aumk * rnaug2 * anmar * 40. * myogrs;

			aar = aar1 - anpx * anpxaf + anpxaf; /* afferent */

			if (aar < aarll)
				aar = aarll;

			ear = 43.333 * eark * anmer * ((rnaug2 - 1.) * efafr + 1.) * myogrs;/* efferent */

			ear = ear * ((aumk - 1.) * aumk1 + 1.);

			if (ear < earll)
				ear = earll;

			rr = aar + ear;/* total renal resistance */

			rfn = par / rr;/* normal renal blood flow */

			//

			/*if(glpca)247,247,248          ;// effect of filtration fraction on GLPC

			 247     glpc=ppc+4.                  ;//glomerular oncotic pressure

			 go to 249;

			 248     efafpr=rfn*vp/vb/(rfn*vp/vb-gfn);

			 if(efafpr < 1.)efafpr=1.;

			 glpc=glpc+(efafpr**1.35*ppc*.98-glpc)/gppd;*/

			if (glpca > 0) { // effect of filtration fraction on GLPC
				efafpr = rfn * vp / vb / (rfn * vp / vb - gfn);
				if (efafpr < 1.)
					efafpr = 1.;
				glpc = glpc + (pow(efafpr, 1.35) * ppc * .98 - glpc) / gppd;
			} else {
				glpc = ppc + 4; //glomerular oncotic pressure
			}
			rbf = rek * rfn;//249
			//						/* GFR computation */

			apd = aar * rfn;//250

			glp = par - apd;/* glomerular pressure */

			pfl = glp - glpc - pxtp;/* Starling forces */

			gfn = gfn + (pfl * gflc - gfn) / gfndmp;

			if (gfn < gfnll)
				gfn = gfnll;

			gfr = gfn * rek;

			// 					/* proximal tubular and macula densa flow */

			ptfl = gfn * 8.;

			mdflw1 = mdflw;

			mdflw = (ptfl - 1.) * mdfl1 + 1.;

			if (mdflw < 0.)
				mdflw = 0.;

			i5 = i5 + i4;

			if (i5 > i) {///goto 251;
				i5 = 0.; /* also used in short loop, but reset here */
				break;
			}
			///////if(fabs(mdflw-mdflw1) > mdflw2)goto 242;

			//

			//   /* -- peritubular capillaries by physical forces -- */

			//

			//////251     i5=0.;              	/* also used in short loop, but reset here */

		} while (fabs(mdflw - mdflw1) > mdflw2);

		rcprs = ((rfn - 1.2) * rfabx + 1.2) * rvrs;/* renal peritub. cap pressure */

		rtsppc = glpc * rtppr - rtpprs;/* renal tissue oncotic pressure */

		if (rtsppc < 1.)
			rtsppc = 1.;

		rabspr = glpc + rtsprs - rcprs - rtsppc;/* Starling forces across tubule */
		rfab1 = rabspr * rabsc;

		rfab = rfab + (rfab1 - rfab) / rfabdp;

		rfabd = (rfab - 1.) * rfabdm + 1.;

		if (rfabd < 0.0001)
			rfabd = 0.0001;

		rfabk = (rfabd - 1.) * rfabkm;/* effect on patassium */

		//

		//   /* -- sodium and potassium handling -- */

		//

		dtnai = mdflw * cna * .0061619;/* Na distal delivery */

		dtnara = amna * rfabd * dtnar / diuret * ((adhmk - 1.) * ahmnar + 1.);

		if (dtnara < dtnarl)
			dtnara = dtnarl;/* non AII distal + collecting Na reabs. */

		//

		dtki = dtnai * cke / cna;/* potassium handling */

		anmke = (anm - 1.) * anmkem + 1.;

		if (anmke < anmkel)
			anmke = anmkel;

		mdflk = (mdflw - 1.) * mdflkm + 1.;

		if (mdflk < .1)
			mdflk = .1;

		dtksc = pow(cke / 4.4, ckeex) * amk * .08 * mdflk / anmke;/* tubular K secretion */

		dtnang = ((anm - 1.) * anmnam + 1.) * .1;/* AII effect on distal Na reabs */

		if (dtnang < 0.)
			dtnang = 0.;

		//

		//   /* -- begin urinary excretion loop -- */

		//
		do {
			nodn = dtnai - dtnara - dtnang;/* sodium excretion 253*/

			if (nodn < .00000001)
				nodn = .00000001;

			dtka = dtka + (kodn / vudn * .0004518 - dtka) * i6;/* I6 */

			kodn = dtki + dtksc - dtka - rfabk;/* potassium excretion */

			if (kodn < 0.)
				kodn = 0.;

			osmopn = dturi + 2. * (nodn + kodn);/* osmolar and water excretion */

			osmop1 = osmopn - .6;

			if (osmopn > .6)
				osmopn = .6;

			if (osmop1 < 0.)
				osmop1 = 0.;

			vud1 = vudn;

			vudn = osmopn / 600. / adhmk + osmop1 / 360.;

			//

			//	DTNAC = DTNAC + (NODN/VUDN-DTNAC)*I6;

			//	I7 = I7 + I6;

			//	if (I7 > I) break;	left out from previous version OCT 91

			//

			//if(fabs(vudn-vud1) > vud2)goto 253;
		} while (fabs(vudn - vud1) > vud2);

		//   /* -- end urinary excretion loop -- */


		//					/* water and electrolytes excretion */

		vud = vudn * rek;

		nod = nodn * rek;

		kod = kodn * rek;

		//						/* urea dynamics */


		plur = plur + (urform - urod) * i;

		plurc = plur / vtw;

		dturi = pow(gfn, 2.) * plurc * 3.84;

		urod = dturi * rek;

#endif
		//

		//##########################################################################################################################################
		//############################################################### ELECTROLYTES #############################################################
		//##########################################################################################################################################


		// block input  = NOD, KOD, VUD, TVD, STH, AMK;

		// block output = CNA, CKE;

		//*/

		//

		ned = nid * sth - nod + trpl * 142.;/* sodium balance */

		nae = nae + ned * i;

		amk1 = (amk - 1.) * alclk + 1.;

		ktotd = kid - kod;/* potassium balance */

		ktot = ktot + ktotd * i;

		vtw = vtw + (tvd - vud) * i;/* water balance */

		//

		i15 = 0;

		//  begin loop
		do {
			ke = (ktot - 3000.) / amk1 / 9.3333;//260

			ki = ktot - ke;

			vec = vtw - vic;/* extracellular volume */

			cna = nae / vec;

			cki = ki / vic;

			ccd = cki - cna;

			vid = ccd * vidml;

			vic = vic + vid * i14;/* intracellular volume */

			if (fabs(vid) < vid1) {//goto 265;
				i15 = 0.;
				break;
			}

			i15 = i15 + i14;

			//if(i15 < i)goto 260;

			//  end of loop

			//265     i15=0.;
		} while (i15 < i);
		cke = ke / vec;

		//##########################################################################################################################################
		//####################################################### SPECIAL EXPERIMENT ###############################################################
		//##########################################################################################################################################

		//---very rapid autoregulation

		///* to cause very rapid autoregulation in all vascular beds except kidneys */

		//

		//      the purpose of this simulation is to cause rapid

		//  blood flow autoregulation in all the vascular beds

		//  besides the kidneys so that other experiments can be run

		//  without waiting for autoregulation to develop.  the

		//  pressure-stretch effect,paex, has been set to 0, and sta

		//  has been set to 1 to eliminate autonomic feedback.  z has

		//  been set to 1 to decrease damping, and the normal

		//  autoregulation in the model (poz,pon,pok,pom,pom2) has

		//  been set to 0.  the gain of the new autoregulation is

		//  set by autogn, initially to 7.  feedback to the arterial

		//  resistances is through rmult, as delayed by rmult1, to

		//  multiply rsn and rsm.

		//

		//      the rate of development of autoregulation is

		//  controlled by autok, which has a normal value of 0.1.

		//

		//      to run, set autoC to 1 to turn on automatic rapid

		//  autoregulation.  { to test the effect of small amounts

		//  of increase in volume, a transfusion can be given or

		//  fluid can be retained by setting rek to .3 to reduce

		//  kidney mass to 30% of normal and after a short run nid to

		// .5 to increase salt intake 5 fold.  record increases in

		//  vec,vb,qao,pa.  note only small increases in all except

		//  pa which is very large.  when much blood is given at

		//  rapid rates, the iteration time limit i3 must be set from

		//  20 minutes down to very short intervals, maybe as little

		//  as .005 minutes.

		//

		if (autoC > 0.) {

			///* no pressure stretch, nor autonomic feedback */

			z = 1.;

			sta = 1.;

			paex = 0.;

			///* no autoregulation */

			poz = 0.;

			pon = 0.;

			pok = 0.;

			pom = 0.;

			pom2 = 0.;

			rmult = ((bfn + bfm) / 3.8 - 1.) * autogn + 1.;

			rmult1 = rmult1 + (rmult - rmult1) * autok;/* feedback to art resistances */

		}

		//

		//---transfusion or loss of whole blood, plasma, or rbc

		//

		//      the purpose of this is to allow transfusion (or blood

		//  loss if negative values are used) and to allow

		//  transfusion or loss of blood with hematocrits from zero

		//  (pure plasma) up to 100% (pure red cells).

		//      to give a transfusion of blood with 40% hematocrit,

		//  set trnsfs to

		//  any rate of transfusion; a value of .001 is 1 ml per

		//  minute.  { when ready to give transfusion, set timetr

		//  to number of minutes for the transfusion to continue

		//  before stopping.  for other hematocrits, set hmtrns to

		//  hematocrit desired before activating timetr.

		//      to cause blood or plasma loss (zero hematocrit), use

		//  same procedure, but with negative values.

		//

		// /* to allow transfusion or loss of blood of various hematocrits if TIMETR > 0:

		//   TRNSFS = transfusion rate in l/min; TRNSTM = total transfusion time

		//   HMTRNS = hematocrit of transfused blood;   */

		//

		if (timetr > 0.) {

			vic = vic + trnsfs * hmtrns * .01 * i;

			vrc = vrc + trnsfs * hmtrns * .01 * i;

			vp = vp + trnsfs * (100. - hmtrns) * .01 * i;

			nae = nae + trnsfs * (100. - hmtrns) * .01 * cna * i;

			ke = ke + trnsfs * (100. - hmtrns) * .01 * cke * i;

			ki = ki + trnsfs * hmtrns * .01 * cki * i;

			prp = prp + trnsfs * (100. - hmtrns) * .01 * cpp * i;

			trnstm = trnstm + i;

			if (trnstm > timetr) {

				trnstm = 0.;

				timetr = 0.;

			}

		}

		//

		//        goto 100
	}
	/////////     if (t <= tend) go to 100
	//      ypar_value(iloop)=ypar0

	//##########################################################################################################################################
	//##########################################################################################################################################

	//##########################################################################################################################################
	//##########################################################################################################################################
	//#################################################### END OF THE GUYTON MODEL #############################################################
	//##########################################################################################################################################
	//##########################################################################################################################################
	//##########################################################################################################################################
	//##########################################################################################################################################

	// 666     continue !end of percentloop
	//       close(5)
	//       close(6)
	//       close(7)
	//      close(8)

	/*close(67)

	 888   continue  !(end of yval iloop loop)
	 //887   continue  !(end boucle iloop)


	 995   format(1f9.3)
	 996   format(1A8)
	 997   format(600e16.5)!295format(354f9.3) sortie des variable
	 998   format(353A8)

	 999   format(50f8.3)
	 1000   format(114e14.5)!format(152f12.3) sortie des parametre

	 end*/
	//##########################################################################################################################################
	//##########################################################################################################################################

	//##########################################################################################################################################
	//##########################################################################################################################################
	//#################################################### END OF THE SENSITIVITY PROG #########################################################
	//##########################################################################################################################################
	//##########################################################################################################################################
	//##########################################################################################################################################
	//##########################################################################################################################################


	//##########################################################################################################################################
	//##########################################################################################################################################

	//##########################################################################################################################################
	//##########################################################################################################################################
	//############################################################# SUBROUTINES ################################################################
	//##########################################################################################################################################
	//##########################################################################################################################################
	//##########################################################################################################################################
	//##########################################################################################################################################


	//##########################################################################################################################################
	//##########################################################################################################################################
	//####################################################### Special thanks to all the mainteners of Gfortran #################################
	//##########################################################################################################################################
	//##########################################################################################################################################
	//                                    ooo

	//                               ooo$$$$$$$$$$$oo

	//                            o$$$$$$$$$$$$$$$$$$$ooo

	//                          o$$$$$$$$$$$$$$$$$"$$$$$$$oo

	//                       o$$$$$$$$$$$$$$$$$$$  o $$$$$$$$$$$$$$oooo

	//                      o$$$$"""$$$$$$$$$$$$$oooo$$$$$$$$$$$$$"""

	//                    oo$""      "$$$$$$$$$$$$$$$$$$$$$$$$"

	//                   o$$          $$$$$$$$$$$$$$$$$$$$$$"

	//                  o$$            $$$$$$$$$$$$$$$$$$$$

	//                o$$$$             $$$$$$$$$$$$$$$$$"

	//               o$$$$$$oooooooo "                $"

	//              $$$$$$$$$$$$$$"

	//             $$$$$$$$$$$$$$

	//            o$$$$$$$$$$$$$                         o

	//           o$$$$$$$$$$$$$                           o

	//          o$$$$$$$$$$$$$$                            "o

	//         o$$$$$$$$$$$$$$$                             "o

	//        o$$$$$$$$$$$$$$$$                              $

	//       o$$$$$$$$$$$$$$$$"                              "

	//       $$$$$$$$$$$$$$$$$                                $

	//      o$$$$$$$$$$$$$$$$$                                $

	//      $$$$$$$$$$$$$$$$$                                 $

	//     $$$$$$$$$$$$$$$$$$                                 "

	//     $$$$$$$$$$$$$$$$$                                   $

	//    $$$$$$$$$$$$$$$$$                                    $

	//    $$$$$$$$$$$$$$$$"                                    $$

	//   $$$$$$$$$$$$$$$$$                                      $o

	//   $$$$$$$$$$$$$$$$$                                      $$

	//  $$$$$$$$$$$$$$$$$$                                       $

	//  $$$$$$$$$$$$$$$$$$o                                      $$

	// $$$$$$$$$$$$$$$$$$$$o                                     $$

	// $$$$$$$$$$$$$$$$$$$$$$o                                   "$

	// $$$$$$$$$$$$$$$$$$$$$$$$o                                  $

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$                                 $$

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$                                $$

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$                               $$

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o                              $$

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o                             $$

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$                             $$

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$                             $"

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$                            $$

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$                            $"

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$                            $

	// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$                            o$

	// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$                            $"

	// $$$$$$$$$$$$$$$$$$$$$$$$$$$$"                            $

	// $$$$$$$$$$$$$$$$$$$$$$$$$$$"                             $

	//  $$$$$$$$$$$$$$$$$$$$$$$$$"                             $$

	//  $$$$$$$$$$$$$$$$$$$$$$$$"                              $$

	//  $$$$$$$$$$$$$$$$$$$$$$$                                $$

	//   $$$$$$$$$$$$$$$$$$$$$                                o$$

	//   $$$$$$$$$$$$$$$$$$$$                                 $$"

	//   "$$$$$$$$$$$$$$$$$$                                  $$

	//    $$$$$$$$$$$$$$$$$                                  o$$

	//    "$$$$$$$$$$$$$$$"                                  $$

	//     $$$$$$$$$$$$$$$                                   $$

	//     "$$$$$$$$$$$$$"                                  o$

	//      $$$$$$$$$$$$"                                   $$

	//      $$$$$$$$$$$"                                    $$

	//       $$$$$$$$$"                                    $$"

	//       $$$$$$$$$                                     $$

	//       "$$$$$$$"                                    $$

	//        $$$$$$$o                                    $"

	//       o$$$$$$$$                                   $$

	//       $$$$$$$$$                                   $$

	//      o$$$$$$$$$                                   $"

	//      $$$$$$$$$$                                  $$

	//      "$$$$$$$$$                                o$""

	//       "$$$$$$$$                          ooooo$$oo

	//          ""$$$$$o                oooo$$$$$$$$$$$$$$ooo

	//             "$$$$$$ooooooo     """""""""$$$""""$$o   ""

	//                                                  "

	return EXIT_SUCCESS;
}
