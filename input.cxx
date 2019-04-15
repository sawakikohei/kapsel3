/*!
  \file input.cxx
  \brief Read udf input file to start simulation
  \author Y. Nakayama
  \date 2006/11/14
  \version 1.3
 */

#include "input.h"

/////////////////////
/////////////////////
int Fixed_particle = 0;
/////////////////////
/////////////////////

//////
EQ          SW_EQ;
const char *EQ_name[] = {"Navier_Stokes",
                         "Shear_Navier_Stokes",
                         "Shear_Navier_Stokes_Lees_Edwards",
                         "Electrolyte",
                         "Navier_Stokes_FDM",
                         "Navier_Stokes_Cahn_Hilliard_FDM",
                         "Shear_Navier_Stokes_Lees_Edwards_FDM",
                         "Shear_NS_LE_CH_FDM"};
//////

ST          SW_NSST;
ST          SW_CHST;
PO          SW_POTENTIAL;
const char *NS_SOLVERTYPE_name[] = {"explicit_scheme", "implicit_scheme"};
const char *CH_SOLVERTYPE_name[] = {"explicit_scheme", "implicit_scheme"};
const char *POTENTIAL_name[]     = {"Landau", "Flory_Huggins"};
int         PHASE_SEPARATION;
int         VISCOSITY_CHANGE;
double      eps_ns;
int         maxiter_ns;
double      eps_ch;
int         maxiter_ch;
double      XYaspect;
double      ETA_A;
double      ETA_B;

gl_param gl;
fh_param fh;
ps_param ps;
//////
PT          SW_PT;
const char *PT_name[] = {"spherical_particle", "chain", "rigid"};
//////
OBL_INT     SW_OBL_INT;
const char *OBL_INT_name[] = {"linear", "spline"};

OUTFORMAT       SW_OUTFORMAT;
EXTFORMAT       SW_EXTFORMAT;
Field_crop      print_field_crop;
Field_output    print_field;
Particle_output print_particle;

const char *OUTFORMAT_name[] = {"NONE", "ASCII", "BINARY", "EXTENDED"};
const char *EXTFORMAT_name[] = {"HDF5"};
//////
const char *JAX_name[] = {"X", "Y", "Z", "NONE"};
const char *JP_name[]  = {"TUMBLER", "SQUIRMER", "OBSTACLE", "OFF"};

//////
SW_time SW_TIME;
//////
char Out_dir[128];
char Out_name[128];
//////
int SW_UDF;

//////
/////// 計算条件の設定
int Nmax;
int Nmin;
int Ns[DIM];
int Ns_shear[DIM];
int HNs[DIM];
int TRNs[DIM];
int N2s[DIM];
int HN2s[DIM];
int TRNs_QS[DIM];
//////
int &NX       = Ns[0];
int &NY       = Ns[1];
int &NZ       = Ns[2];
int &HNX      = HNs[0];
int &HNY      = HNs[1];
int &HNZ      = HNs[2];
int &N2X      = N2s[0];
int &N2Y      = N2s[1];
int &N2Z      = N2s[2];
int &HN2X     = HN2s[0];
int &HN2Y     = HN2s[1];
int &HN2Z     = HN2s[2];
int &TRN_X    = TRNs[0];
int &TRN_Y    = TRNs[1];
int &TRN_Z    = TRNs[2];
int &TRN_QS_X = TRNs_QS[0];
int &TRN_QS_Y = TRNs_QS[1];
int &TRN_QS_Z = TRNs_QS[2];
int  HNZ_;
int  NZ_;
int  HN2Z_;
int  N2Z_;
int  ROTATION;

Particle_IC DISTRIBUTION;
Particle_IO ORIENTATION;
int         N_iteration_init_distribution;
int         FIX_CELL;
int         FIX_CELLxyz[DIM];
int         PINNING;
int         N_PIN;
int *       Pinning_Numbers;
int         N_PIN_ROT;
int *       Pinning_ROT_Numbers;
//////

int RESUMED;
int last_ts;

double RHO;
double ETA;
double kBT;
double ikBT;
double Shear_rate;
double Shear_rate_eff;
double Shear_strain_realized;
// AC
double  Shear_frequency;
double  Inertia_stress;
double  dev_shear_stress[2];
double  rigid_dev_shear_stress[2];
double &dev_shear_stress_lj        = dev_shear_stress[0];
double &dev_shear_stress_rot       = dev_shear_stress[1];
double &rigid_dev_shear_stress_lj  = rigid_dev_shear_stress[0];
double &rigid_dev_shear_stress_rot = rigid_dev_shear_stress[1];
//////
double Delta_ETA;
double Nu_ratio;
//////
double Axel;
double DT;
//////
//////
double *MASS_RATIOS;
double *S_surfaces;  // pretilt scalar order
double *W_surfaces;  // spring cst. of anchoring
double  DX;
double  A_XI;
double  dmy_A;
//////
double G;
int    G_direction;
//////
int  Component_Number;
int  Particle_Number;
int *Particle_Numbers;
int *Beads_Numbers;
int *Chain_Numbers;
int  GTS;
int  Num_snap;
// per species janus specifications
int      SW_JANUS;
int      SW_JANUS_MOTOR;  // body force/torque
int      SW_JANUS_SLIP;   // surface slip velocity
int      MAX_SLIP_ITER;
double   MAX_SLIP_TOL;
JAX *    janus_axis;
JP *     janus_propulsion;
double **janus_force;
double **janus_torque;
double * janus_slip_vel;
double * janus_slip_mode;

////
int       Rigid_Number;
int **    Rigid_Motions_vel;    // 0 (fix) or 1 (free)
int **    Rigid_Motions_omega;  // 0 (fix) or 1 (free)
double ** Rigid_Velocities;
double ** Rigid_Omegas;
int *     RigidID_Components;
int *     Rigid_Particle_Numbers;
int *     Rigid_Particle_Cumul;
double ** xGs;
double ** xGs_previous;
double ** xGs_nopbc;
double *  Rigid_Masses;
double *  Rigid_IMasses;
double ***Rigid_Moments;
double ***Rigid_IMoments;
double ***Rigid_Moments_body;
double ** velocityGs;
double ** omegaGs;
double ** forceGs;   // hydro
double ** forceGrs;  // LJ
double ** torqueGs;
double ** torqueGrs;
double ** velocityGs_old;
double ** omegaGs_old;
double ** forceGs_previous;
double ** forceGrs_previous;
double ** torqueGs_previous;
double ** torqueGrs_previous;
int *     Particle_RigidID;
//
////
double **GRvecs;
double **GRvecs_body;
//
double  NU;
double  IRHO;
double *RHO_particle;
double *MASS;
//////
double *IMASS_RATIOS;
double *MOI;
double *IMASS;
double *IMOI;
//////
double Tdump;
double DT_noise;
//////
double DX3;
//////
double  L[DIM];
double  HL[DIM];
double  L_particle[DIM];
double  iL_particle[DIM];
double  HL_particle[DIM];
double &LX  = L[0];
double &LY  = L[1];
double &LZ  = L[2];
double &HLX = HL[0];
double &HLY = HL[1];
double &HLZ = HL[2];
double  WAVE_X;
double  WAVE_Y;
double  WAVE_Z;
double  KMAX2;
//////
// double RADIUS;
double  dmy_RADIUS;
double  dmy_SIGMA;
double *RADII;
// double* SIGMAS;

double XI;
double HXI;
double VF;
double Ivolume;
//////
int MSTEP;
////// LJ PARAMETERS
double *SIGMA;
double *EPSILON;
double *A_R_cutoff;
double *LJ_dia;
int *   LJ_truncate;
int *   LJ_powers;
int     LJ_TRUNCATE;
//
// double R_cutoff;
double T_LJ;
double VF_LJ;
double Srate_depend_LJ_cap;
/////// Electrolyte
int     N_spec;
double  Onsager_solute_coeff;
int     Poisson_Boltzmann;
int     External_field;
int     AC;
int     Shear_AC;
double *Surface_charge;
double *Surface_charge_e;
double  Elementary_charge = 1.;
double  Valency_counterion;
double  Valency_positive_ion;
double  Valency_negative_ion;
double  Onsager_coeff_counterion;
double  Onsager_coeff_positive_ion;
double  Onsager_coeff_negative_ion;
double  Dielectric_cst;
double  Debye_length;
double  E_ext[DIM];
double  Frequency;
double  Angular_Frequency;

// double kT_snap_v=0.59;
// double kT_snap_omega=0.61;
// double kT_snap_v=1.763;
double kT_snap_v = 1.439;
// ldouble kT_snap_v=2.11;
double kT_snap_o = 1.22;

double alpha_v;
double alpha_o;

//////shear_degree
double degree_oblique;

//////
inline void Set_global_parameters(void) {
  Particle_Number = 0;
  for (int i = 0; i < Component_Number; i++) {
    Particle_Number += Particle_Numbers[i];
    IMASS_RATIOS[i] = 1.0 / MASS_RATIOS[i];
    RHO_particle[i] = MASS_RATIOS[i] * RHO;
    MASS[i]         = RHO_particle[i] * 4. / 3. * M_PI * POW3(RADII[i]);
    IMASS[i]        = 1. / MASS[i];
    MOI[i]          = 2. / 5. * MASS[i] * SQ(RADII[i]);
    IMOI[i]         = 1. / MOI[i];
  }
  if (kBT > 0.) {
    ikBT = 1. / kBT;
  }
  IRHO = 1. / RHO;
  NU   = ETA * IRHO;
  //////
  DX3 = DX * DX * DX;
  //////
  HNZ_ = NZ / 2 + 1;
  NZ_  = 2 * HNZ_;
  for (int d = 0; d < DIM; d++) {
    HNs[d]  = Ns[d] / 2;
    TRNs[d] = (Ns[d] + 2) / 3;
    N2s[d]  = Ns[d] * 2;
    HN2s[d] = Ns[d];
  }
  HN2Z_ = N2s[2] / 2 + 1;
  N2Z_  = 2 * HN2Z_;
  //////
  {
    for (int d = 0; d < DIM; d++) {
      L[d]           = Ns[d] * DX;  // real-dimension
      L_particle[d]  = L[d];
      iL_particle[d] = 1. / L_particle[d];
      HL[d]          = L[d] * .5;
      HL_particle[d] = L[d] * .5;
    }
  }
  WAVE_X = PI2 / LX;
  WAVE_Y = PI2 / LY;
  WAVE_Z = PI2 / LZ;
  KMAX2  = SQ(WAVE_X * TRN_X) + SQ(WAVE_Y * TRN_Y) + SQ(WAVE_Z * TRN_Z);
  //////
  {
    if (SW_EQ == Navier_Stokes) {
      Tdump = 1. / (NU * KMAX2);
    } else if ((SW_EQ == Shear_Navier_Stokes) || (SW_EQ == Shear_Navier_Stokes_Lees_Edwards) ||
               (SW_EQ == Shear_Navier_Stokes_Lees_Edwards_FDM) || (SW_EQ == Shear_NS_LE_CH_FDM)) {
      double shear_CFL_time = DX / (Shear_rate * LY);
      // double shear_stokes_time = RADIUS/(Shear_rate*LY*0.5);
      double shear_stokes_time = XI / (Shear_rate * LY * 0.5);

      double mass_min = DBL_MAX;
      {
        for (int i = 0; i < Component_Number; i++) {
          mass_min = MIN(mass_min, MASS[i]);
        }
      }
      double LJ_stokes_time = sqrt(mass_min * XI / Srate_depend_LJ_cap);
      Tdump                 = 1. / (NU * KMAX2);
      fprintf(stderr, "# vis_time 2:shearCFLtime 3:shearstokestime 4:LJstokestime\n");
      fprintf(stderr, "# %g %g %g %g\n", Tdump * Axel, shear_CFL_time, shear_stokes_time, LJ_stokes_time);
      fprintf(stderr, "# Delta gamma = %.6g * %.6g = %.6g\n", Shear_rate, Tdump * Axel, Shear_rate * Tdump * Axel);
      // Tdump = MIN(Tdump, shear_CFL_time);
      // Tdump = MIN(Tdump, shear_stokes_time);
    } else if (SW_EQ == Electrolyte) {
      Tdump                    = 1. / (NU * KMAX2);
      double dmy_onsager_coeff = 0.0;
      if (N_spec == 1) {
        dmy_onsager_coeff = Onsager_coeff_counterion;
      } else if (N_spec == 2) {
        dmy_onsager_coeff = MAX(Onsager_coeff_positive_ion, Onsager_coeff_negative_ion);
      } else {
        fprintf(stderr, "# Error : N_spec should be 1 or 2\n");
        exit_job(EXIT_FAILURE);
      }
      double diffusion_time = 1. / (kBT * dmy_onsager_coeff * KMAX2);
      Tdump                 = MIN(Tdump, diffusion_time);
      if (External_field) {
        if (AC) {
          double dmy            = 1.e-2;
          double frequency_time = 1. / Frequency;
          Angular_Frequency     = PI2 * Frequency;
          Tdump                 = MIN(Tdump, dmy * frequency_time);
        }
      }
    } else if (SW_EQ == Navier_Stokes_FDM || SW_EQ == Navier_Stokes_Cahn_Hilliard_FDM) {
      Tdump = 1. / (NU * KMAX2);
    }
    if (SW_TIME == AUTO) {
      DT = Axel * Tdump;
      if (SW_EQ == Navier_Stokes || SW_EQ == Navier_Stokes_FDM || SW_EQ == Navier_Stokes_Cahn_Hilliard_FDM) {
        if (kBT > 0) {
          if (1) {
            double sdv2 = (NX * NY * NZ) / POW3(DX) * ETA * kBT;
            DT_noise    = 1.e0 / sdv2;
          } else {
            double mass_max = 0.0;
            for (int i = 0; i < Component_Number; i++) {
              mass_max = MAX(mass_max, MASS[i]);
            }
            double themal_speed = kBT / mass_max;
            DT_noise            = XI / themal_speed;
          }
          // DT = Axel * MIN(Tdump, DT_noise);
        }
      }
    } else if (SW_TIME == MANUAL) {
      ;
    }
  }
  //////
  HXI = XI * 0.5;
  //////
  //////
  MSTEP = GTS * Num_snap;
  //////
  Ivolume = 1. / (LX * LY * LZ);
  VF      = 0.0;
  for (int ci = 0; ci < Component_Number; ci++) {
    VF += (double)Particle_Numbers[ci] * 4. / 3. * M_PI * Ivolume * POW3(RADII[ci]);
  }

  if (SW_JANUS_SLIP) {
    for (int i = 0; i < Component_Number; i++) {
      janus_slip_mode[i] /= 2.0;  // \alpha/2 = B_2/(2*B_1)
    }
  }

  // Check if pinned particles exist
  for (int i = 0; i < N_PIN; i++) {
    if (Pinning_Numbers[i] < 0 || Pinning_Numbers[i] >= Particle_Number) {
      fprintf(stderr, "# Error: trying to pin non existing particle (%d/%d)\n!", Pinning_Numbers[i], Particle_Number);
      exit_job(EXIT_FAILURE);
    }
  }
  for (int i = 0; i < N_PIN_ROT; i++) {
    if (Pinning_ROT_Numbers[i] < 0 || Pinning_ROT_Numbers[i] >= Particle_Number) {
      fprintf(stderr,
              "# Error: trying to rot-pin non existing particle (%d/%d)\n!",
              Pinning_ROT_Numbers[i],
              Particle_Number);
      exit_job(EXIT_FAILURE);
    }
  }
}

UDFManager *ufin;
UDFManager *ufout;
UDFManager *ufres;
void        Gourmet_file_io(const char *infile,
                            const char *outfile,
                            const char *sumfile,
                            const char *deffile,
                            const char *ctrlfile,
                            const char *resfile) {
  if (file_check(infile)) ufin = new UDFManager(infile);

  // --------------------------------------------------------
  // レコードを追加するか新規にするか決めるため, 重複するけど
  // resumed or not は outfile開く前にも見とく
  {
    string str;
    ufin->get("resume.Calculation", str);
    if (str == "NEW") {
      RESUMED = 0;
    } else if (str == "CONTINUE_ORIGINAL" || str == "CONTINUE_FDM" || str == "CONTINUE_FDM_PHASE_SEPARATION") {
      RESUMED = 1;
    } else {
      fprintf(stderr, "invalid Calculation\n");
      exit_job(EXIT_FAILURE);
    }
  }

  // section_RESUMED
  if (!RESUMED) {
    // if(file_check(deffile)) ufout= new UDFManager(outfile,deffile,true);
    if (file_check(deffile)) ufout = new UDFManager(outfile, deffile, false);
  } else {
    // if(file_check(deffile)) ufout= new UDFManager(outfile);
    if (file_check(deffile)) ufout = new UDFManager(outfile, deffile, 2);
  }
  // if(file_check(resfile)) ufres= new UDFManager(resfile);
  // if(file_check(deffile)) ufres= new UDFManager(resfile,2);
  if (file_check(deffile)) ufres = new UDFManager(resfile, deffile, false);
  /*
    if(file_check(deffile)) ufout= new UDFManager(outfile,deffile,true);
    if(file_check(resfile)) ufres= new UDFManager(resfile);
  */
  // --------------------------------------------------------

  // section_udf version
  {  // check udf version
    string code_version = "v3.3";
    fprintf(stderr, "# Kapsel: UDF %s\n", code_version.c_str());
    fprintf(stderr, "# Git Version  : %s\n", GIT_VERSION);
    fprintf(stderr, "# Git Reference: %s\n", GIT_REF);

    string udf_name    = ufin->getEngineName();
    string udf_version = ufin->getEngineVersion();
    if (code_version != udf_version) {
      fprintf(stderr, "###############################################\n");
      fprintf(stderr, "#                                             #\n");
      fprintf(stderr, "#    Warning: Engine versions do not match    #\n");
      fprintf(stderr, "#                                             #\n");
      fprintf(stderr, "###############################################\n");
    }
  }

  /////// resumed or not
  {
    ufin->get("resume.CONTINUE.Saved_Data.jikan.ts", last_ts);
    // ufout->put("resume.CONTINUE.Saved_Data.jikan.ts",last_ts);
    // ufres->put("resume.CONTINUE.Saved_Data.jikan.ts",last_ts);
  }

  // section_select constitutive eq
  /////// select constitutive eq
  {
    Location target("constitutive_eq");
    string   str;
    ufin->get(target.sub("type"), str);
    ufout->put(target.sub("type"), str);
    ufres->put(target.sub("type"), str);
    if (str == EQ_name[Navier_Stokes]) {
      SW_EQ = Navier_Stokes;
      {
        target.down(EQ_name[SW_EQ]);
        {
          ufin->get(target.sub("DX"), DX);
          ufin->get(target.sub("RHO"), RHO);
          ufin->get(target.sub("ETA"), ETA);
          ufin->get(target.sub("kBT"), kBT);
          ufin->get(target.sub("alpha_v"), alpha_v);
          ufin->get(target.sub("alpha_o"), alpha_o);
        }
        {
          ufout->put(target.sub("DX"), DX);
          ufout->put(target.sub("RHO"), RHO);
          ufout->put(target.sub("ETA"), ETA);
          ufout->put(target.sub("kBT"), kBT);
          ufout->put(target.sub("alpha_v"), alpha_v);
          ufout->put(target.sub("alpha_o"), alpha_o);
        }
        {
          ufres->put(target.sub("DX"), DX);
          ufres->put(target.sub("RHO"), RHO);
          ufres->put(target.sub("ETA"), ETA);
          ufres->put(target.sub("kBT"), kBT);
          ufres->put(target.sub("alpha_v"), alpha_v);
          ufres->put(target.sub("alpha_o"), alpha_o);
        }
      }
    } else if (str == EQ_name[Shear_Navier_Stokes]) {
      SW_EQ = Shear_Navier_Stokes;
      {
        target.down(EQ_name[SW_EQ]);
        {
          ufin->get(target.sub("DX"), DX);
          ufin->get(target.sub("RHO"), RHO);
          ufin->get(target.sub("ETA"), ETA);
          ufin->get(target.sub("kBT"), kBT);
          ufin->get(target.sub("alpha_v"), alpha_v);
          ufin->get(target.sub("alpha_o"), alpha_o);
        }
        Shear_strain_realized = 0.0;
        { Srate_depend_LJ_cap = DBL_MAX; }
        {
          ufout->put(target.sub("DX"), DX);
          ufout->put(target.sub("RHO"), RHO);
          ufout->put(target.sub("ETA"), ETA);
          ufout->put(target.sub("kBT"), kBT);
          ufout->put(target.sub("alpha_v"), alpha_v);
          ufout->put(target.sub("alpha_o"), alpha_o);
        }
        {
          ufres->put(target.sub("DX"), DX);
          ufres->put(target.sub("RHO"), RHO);
          ufres->put(target.sub("ETA"), ETA);
          ufres->put(target.sub("kBT"), kBT);
          ufres->put(target.sub("alpha_v"), alpha_v);
          ufres->put(target.sub("alpha_o"), alpha_o);
        }
        {
          Location target("constitutive_eq.Shear_Navier_Stokes.External_field");
          ufin->get(target.sub("type"), str);
          ufout->put(target.sub("type"), str);
          ufres->put(target.sub("type"), str);
          if (str == "DC") {
            target.down("DC");
            Shear_AC = 0;
            ufin->get(target.sub("Shear_rate"), Shear_rate);
            ufout->put(target.sub("Shear_rate"), Shear_rate);
            ufres->put(target.sub("Shear_rate"), Shear_rate);
            fprintf(stderr, "# DC steady shear: shear rate %f \n", Shear_rate);
          }
          if (str == "AC") {
            target.down("AC");
            Shear_AC = 1;
            ufin->get(target.sub("Shear_rate"), Shear_rate);
            ufout->put(target.sub("Shear_rate"), Shear_rate);
            ufres->put(target.sub("Shear_rate"), Shear_rate);

            ufin->get(target.sub("Frequency"), Shear_frequency);
            ufout->put(target.sub("Frequency"), Shear_frequency);
            ufres->put(target.sub("Frequency"), Shear_frequency);

            fprintf(stderr,
                    "# AC oscillatory shear: (shear rate, frequency, the maximum amp of strain)= %f %f %f\n",
                    Shear_rate,
                    Shear_frequency,
                    Shear_rate / Shear_frequency);
          }
        }
      }
    } else if (str == EQ_name[Shear_Navier_Stokes_Lees_Edwards]) {
      SW_EQ = Shear_Navier_Stokes_Lees_Edwards;
      {
        target.down(EQ_name[SW_EQ]);
        {
          ufin->get(target.sub("DX"), DX);
          ufin->get(target.sub("RHO"), RHO);
          ufin->get(target.sub("ETA"), ETA);
          ufin->get(target.sub("kBT"), kBT);
          ufin->get(target.sub("alpha_v"), alpha_v);
          ufin->get(target.sub("alpha_o"), alpha_o);
        }
        Shear_strain_realized = 0.0;
        { Srate_depend_LJ_cap = DBL_MAX; }
        {
          ufout->put(target.sub("DX"), DX);
          ufout->put(target.sub("RHO"), RHO);
          ufout->put(target.sub("ETA"), ETA);
          ufout->put(target.sub("kBT"), kBT);
          ufout->put(target.sub("alpha_v"), alpha_v);
          ufout->put(target.sub("alpha_o"), alpha_o);
        }
        {
          ufres->put(target.sub("DX"), DX);
          ufres->put(target.sub("RHO"), RHO);
          ufres->put(target.sub("ETA"), ETA);
          ufres->put(target.sub("kBT"), kBT);
          ufres->put(target.sub("alpha_v"), alpha_v);
          ufres->put(target.sub("alpha_o"), alpha_o);
        }
        {
          Location target("constitutive_eq.Shear_Navier_Stokes_Lees_Edwards.External_field");
          ufin->get(target.sub("type"), str);
          ufout->put(target.sub("type"), str);
          ufres->put(target.sub("type"), str);
          if (str == "DC") {
            target.down("DC");
            Shear_AC = 0;
            ufin->get(target.sub("Shear_rate"), Shear_rate);
            ufout->put(target.sub("Shear_rate"), Shear_rate);
            ufres->put(target.sub("Shear_rate"), Shear_rate);
            fprintf(stderr, "# DC steady shear: shear rate %f \n", Shear_rate);
          }
          if (str == "AC") {  // in near future, someone will extend this section.
            // AC by otomura
            target.down("AC");
            Shear_AC = 1;
            ufin->get(target.sub("Shear_rate"), Shear_rate);
            ufout->put(target.sub("Shear_rate"), Shear_rate);
            ufres->put(target.sub("Shear_rate"), Shear_rate);
            ufin->get(target.sub("Frequency"), Shear_frequency);
            ufout->put(target.sub("Frequency"), Shear_frequency);
            ufres->put(target.sub("Frequency"), Shear_frequency);
            fprintf(stderr,
                    "# AC oscillatory shear: (shear rate, frequency, the maximum amp of strain)= %f %f %f\n",
                    Shear_rate,
                    Shear_frequency,
                    Shear_rate / Shear_frequency);
          }
        }
      }
    } else if (str == EQ_name[Electrolyte]) {
      SW_EQ = Electrolyte;
      {
        target.down(EQ_name[Electrolyte]);
        {
          ufin->get(target.sub("DX"), DX);
          ufin->get(target.sub("RHO"), RHO);
          ufin->get(target.sub("ETA"), ETA);
          ufin->get(target.sub("kBT"), kBT);
          ufin->get(target.sub("Dielectric_cst"), Dielectric_cst);

          {
            ufout->put(target.sub("DX"), DX);
            ufout->put(target.sub("RHO"), RHO);
            ufout->put(target.sub("ETA"), ETA);
            ufout->put(target.sub("kBT"), kBT);
            ufout->put(target.sub("Dielectric_cst"), Dielectric_cst);
          }
          {
            ufres->put(target.sub("DX"), DX);
            ufres->put(target.sub("RHO"), RHO);
            ufres->put(target.sub("ETA"), ETA);
            ufres->put(target.sub("kBT"), kBT);
            ufres->put(target.sub("Dielectric_cst"), Dielectric_cst);
          }
          {
            ufin->get(target.sub("INIT_profile"), str);
            ufout->put(target.sub("INIT_profile"), str);
            ufres->put(target.sub("INIT_profile"), str);
            if (str == "Uniform") {
              Poisson_Boltzmann = 0;
            } else if (str == "Poisson_Boltzmann") {
              Poisson_Boltzmann = 1;
            } else {
              fprintf(stderr, "invalid INIT_profile\n");
              exit_job(EXIT_FAILURE);
            }
          }
          {
            Location target("constitutive_eq.Electrolyte.Add_salt");
            ufin->get(target.sub("type"), str);
            ufout->put(target.sub("type"), str);
            ufres->put(target.sub("type"), str);
            if (str == "saltfree") {
              N_spec = 1;
              target.down("saltfree");
              {
                ufin->get(target.sub("Valency_counterion"), Valency_counterion);
                ufout->put(target.sub("Valency_counterion"), Valency_counterion);
                ufres->put(target.sub("Valency_counterion"), Valency_counterion);
                ufin->get(target.sub("Onsager_coeff_counterion"), Onsager_coeff_counterion);
                ufout->put(target.sub("Onsager_coeff_counterion"), Onsager_coeff_counterion);
                ufres->put(target.sub("Onsager_coeff_counterion"), Onsager_coeff_counterion);
              }
            } else if (str == "salt") {
              N_spec = 2;
              target.down("salt");
              {
                {
                  ufin->get(target.sub("Valency_positive_ion"), Valency_positive_ion);
                  ufin->get(target.sub("Valency_negative_ion"), Valency_negative_ion);
                  ufin->get(target.sub("Onsager_coeff_positive_ion"), Onsager_coeff_positive_ion);
                  ufin->get(target.sub("Onsager_coeff_negative_ion"), Onsager_coeff_negative_ion);
                  ufin->get(target.sub("Debye_length"), Debye_length);
                }
                {
                  ufout->put(target.sub("Valency_positive_ion"), Valency_positive_ion);
                  ufout->put(target.sub("Valency_negative_ion"), Valency_negative_ion);
                  ufout->put(target.sub("Onsager_coeff_positive_ion"), Onsager_coeff_positive_ion);
                  ufout->put(target.sub("Onsager_coeff_negative_ion"), Onsager_coeff_negative_ion);
                  ufout->put(target.sub("Debye_length"), Debye_length);
                }
                {
                  ufres->put(target.sub("Valency_positive_ion"), Valency_positive_ion);
                  ufres->put(target.sub("Valency_negative_ion"), Valency_negative_ion);
                  ufres->put(target.sub("Onsager_coeff_positive_ion"), Onsager_coeff_positive_ion);
                  ufres->put(target.sub("Onsager_coeff_negative_ion"), Onsager_coeff_negative_ion);
                  ufres->put(target.sub("Debye_length"), Debye_length);
                }
              }
              target.up();
            } else {
              fprintf(stderr, "invalid Add_salt\n");
              exit_job(EXIT_FAILURE);
            }
          }
          {
            Location target("constitutive_eq.Electrolyte.Electric_field");
            ufin->get(target.sub("type"), str);
            ufout->put(target.sub("type"), str);
            ufres->put(target.sub("type"), str);
            if (str == "OFF") {
              External_field = 0;
              for (int d = 0; d < DIM; d++) {
                E_ext[d] = 0.0;
              }
            } else if (str == "ON") {
              External_field = 1;
              target.down("ON");
              {
                ufin->get(target.sub("type"), str);
                ufout->put(target.sub("type"), str);
                ufres->put(target.sub("type"), str);
                if (str == "DC") {
                  target.down("DC");
                  AC = 0;
                  {
                    ufin->get(target.sub("Ex"), E_ext[0]);
                    ufin->get(target.sub("Ey"), E_ext[1]);
                    ufin->get(target.sub("Ez"), E_ext[2]);
                  }
                  {
                    ufout->put(target.sub("Ex"), E_ext[0]);
                    ufout->put(target.sub("Ey"), E_ext[1]);
                    ufout->put(target.sub("Ez"), E_ext[2]);
                  }
                  {
                    ufres->put(target.sub("Ex"), E_ext[0]);
                    ufres->put(target.sub("Ey"), E_ext[1]);
                    ufres->put(target.sub("Ez"), E_ext[2]);
                  }
                } else if (str == "AC") {
                  target.down("AC");
                  AC = 1;
                  {
                    ufin->get(target.sub("Ex"), E_ext[0]);
                    ufin->get(target.sub("Ey"), E_ext[1]);
                    ufin->get(target.sub("Ez"), E_ext[2]);
                    ufin->get(target.sub("Frequency"), Frequency);
                  }
                  {
                    ufout->put(target.sub("Ex"), E_ext[0]);
                    ufout->put(target.sub("Ey"), E_ext[1]);
                    ufout->put(target.sub("Ez"), E_ext[2]);
                    ufout->put(target.sub("Frequency"), Frequency);
                  }
                  {
                    ufres->put(target.sub("Ex"), E_ext[0]);
                    ufres->put(target.sub("Ey"), E_ext[1]);
                    ufres->put(target.sub("Ez"), E_ext[2]);
                    ufres->put(target.sub("Frequency"), Frequency);
                  }

                } else {
                  fprintf(stderr, "invalid switch for DC or AC\n");
                  exit_job(EXIT_FAILURE);
                }
              }
              target.up();
            } else {
              fprintf(stderr, "invalid Electric_field\n");
              exit_job(EXIT_FAILURE);
            }
          }
        }
      }
    } else if (str == EQ_name[Navier_Stokes_FDM]) {
      SW_EQ            = Navier_Stokes_FDM;
      PHASE_SEPARATION = 0;
      {
        target.down(EQ_name[SW_EQ]);
        ufin->get(target.sub("NS_solver.type"), str);
        ufout->put(target.sub("NS_solver.type"), str);
        ufres->put(target.sub("NS_solver.type"), str);
        {
          if (str == NS_SOLVERTYPE_name[explicit_scheme]) {
            SW_NSST = explicit_scheme;
          } else if (str == NS_SOLVERTYPE_name[implicit_scheme]) {
            SW_NSST = implicit_scheme;
            target.down("NS_solver.implicit_scheme");

            ufin->get(target.sub("tolerance"), eps_ns);
            ufout->put(target.sub("tolerance"), eps_ns);
            ufres->put(target.sub("tolerance"), eps_ns);

            ufin->get(target.sub("maximum_iteration"), maxiter_ns);
            ufout->put(target.sub("maximum_iteration"), maxiter_ns);
            ufres->put(target.sub("maximum_iteration"), maxiter_ns);
            target.up();
            target.up();
          } else {
            fprintf(stderr, "invalid NS SOLVER TYPE\n");
            exit_job(EXIT_FAILURE);
          }
        }
        {
          ufin->get(target.sub("DX"), DX);
          ufin->get(target.sub("RHO"), RHO);
          ufin->get(target.sub("ETA"), ETA);
          ufin->get(target.sub("kBT"), kBT);
          ufin->get(target.sub("alpha_v"), alpha_v);
          ufin->get(target.sub("alpha_o"), alpha_o);
        }
        {
          ufout->put(target.sub("DX"), DX);
          ufout->put(target.sub("RHO"), RHO);
          ufout->put(target.sub("ETA"), ETA);
          ufout->put(target.sub("kBT"), kBT);
          ufout->put(target.sub("alpha_v"), alpha_v);
          ufout->put(target.sub("alpha_o"), alpha_o);
        }
        {
          ufres->put(target.sub("DX"), DX);
          ufres->put(target.sub("RHO"), RHO);
          ufres->put(target.sub("ETA"), ETA);
          ufres->put(target.sub("kBT"), kBT);
          ufres->put(target.sub("alpha_v"), alpha_v);
          ufres->put(target.sub("alpha_o"), alpha_o);
        }
      }
    } else if (str == EQ_name[Navier_Stokes_Cahn_Hilliard_FDM]) {
      SW_EQ            = Navier_Stokes_Cahn_Hilliard_FDM;
      PHASE_SEPARATION = 1;
      {
        target.down(EQ_name[SW_EQ]);
        ufin->get(target.sub("NS_solver.type"), str);
        ufout->put(target.sub("NS_solver.type"), str);
        ufres->put(target.sub("NS_solver.type"), str);
        {
          if (str == NS_SOLVERTYPE_name[explicit_scheme]) {
            SW_NSST = explicit_scheme;
          } else if (str == NS_SOLVERTYPE_name[implicit_scheme]) {
            SW_NSST = implicit_scheme;
            target.down("NS_solver.implicit_scheme");

            ufin->get(target.sub("tolerance"), eps_ns);
            ufout->put(target.sub("tolerance"), eps_ns);
            ufres->put(target.sub("tolerance"), eps_ns);

            ufin->get(target.sub("maximum_iteration"), maxiter_ns);
            ufout->put(target.sub("maximum_iteration"), maxiter_ns);
            ufres->put(target.sub("maximum_iteration"), maxiter_ns);

            ufin->get(target.sub("viscosity_change"), str);
            ufout->put(target.sub("viscosity_change"), str);
            ufres->put(target.sub("viscosity_change"), str);
            {
              if (str == "OFF") {
                VISCOSITY_CHANGE = 0;
              } else if (str == "ON") {
                VISCOSITY_CHANGE = 1;
                target.down("ON");
                ufin->get(target.sub("ETA_A"), ETA_A);
                ufout->put(target.sub("ETA_A"), ETA_A);
                ufres->put(target.sub("ETA_A"), ETA_A);

                ufin->get(target.sub("ETA_B"), ETA_B);
                ufout->put(target.sub("ETA_B"), ETA_B);
                ufres->put(target.sub("ETA_B"), ETA_B);
                target.up();
              } else {
                fprintf(stderr, "invalid viscosity change slection \n");
                exit_job(EXIT_FAILURE);
              }
            }

            target.up();
            target.up();
          } else {
            fprintf(stderr, "invalid NS SOLVER TYPE\n");
            exit_job(EXIT_FAILURE);
          }
        }
        ufin->get(target.sub("CH_solver.type"), str);
        ufout->put(target.sub("CH_solver.type"), str);
        ufres->put(target.sub("CH_solver.type"), str);
        {
          if (str == CH_SOLVERTYPE_name[explicit_scheme]) {
            SW_CHST = explicit_scheme;
          } else if (str == CH_SOLVERTYPE_name[implicit_scheme]) {
            SW_CHST = implicit_scheme;
            target.down("CH_solver.implicit_scheme");

            ufin->get(target.sub("tolerance"), eps_ch);
            ufout->put(target.sub("tolerance"), eps_ch);
            ufres->put(target.sub("tolerance"), eps_ch);

            ufin->get(target.sub("maximum_iteration"), maxiter_ch);
            ufout->put(target.sub("maximum_iteration"), maxiter_ch);
            ufres->put(target.sub("maximum_iteration"), maxiter_ch);

            target.up();
            target.up();
          } else {
            fprintf(stderr, "invalid CH SOLVER TYPE\n");
            exit_job(EXIT_FAILURE);
          }
        }
        {
          ufin->get(target.sub("DX"), DX);
          ufin->get(target.sub("RHO"), RHO);
          ufin->get(target.sub("ETA"), ETA);
          ufin->get(target.sub("kBT"), kBT);
          ufin->get(target.sub("alpha_v"), alpha_v);
          ufin->get(target.sub("alpha_o"), alpha_o);
        }
        {
          ufout->put(target.sub("DX"), DX);
          ufout->put(target.sub("RHO"), RHO);
          ufout->put(target.sub("ETA"), ETA);
          ufout->put(target.sub("kBT"), kBT);
          ufout->put(target.sub("alpha_v"), alpha_v);
          ufout->put(target.sub("alpha_o"), alpha_o);
        }
        {
          ufres->put(target.sub("DX"), DX);
          ufres->put(target.sub("RHO"), RHO);
          ufres->put(target.sub("ETA"), ETA);
          ufres->put(target.sub("kBT"), kBT);
          ufres->put(target.sub("alpha_v"), alpha_v);
          ufres->put(target.sub("alpha_o"), alpha_o);
        }
        {
          Location target("constitutive_eq.Navier_Stokes_Cahn_Hilliard_FDM.Potential");
          ufin->get(target.sub("type"), str);
          ufout->put(target.sub("type"), str);
          ufres->put(target.sub("type"), str);

          if (str == "Landau") {
            SW_POTENTIAL = Landau;
            target.down("Landau");
            ufin->get(target.sub("composition_ratio"), ps.ratio);
            ufout->put(target.sub("composition_ratio"), ps.ratio);
            ufres->put(target.sub("composition_ratio"), ps.ratio);

            ufin->get(target.sub("initial_fluctuation"), ps.init_fluct);
            ufout->put(target.sub("initial_fluctuation"), ps.init_fluct);
            ufres->put(target.sub("initial_fluctuation"), ps.init_fluct);

            ufin->get(target.sub("a"), gl.a);
            ufout->put(target.sub("a"), gl.a);
            ufres->put(target.sub("a"), gl.a);

            ufin->get(target.sub("b"), gl.b);
            ufout->put(target.sub("b"), gl.b);
            ufres->put(target.sub("b"), gl.b);

            ufin->get(target.sub("d"), ps.d);
            ufout->put(target.sub("d"), ps.d);
            ufres->put(target.sub("d"), ps.d);

            ufin->get(target.sub("w"), ps.w);
            ufout->put(target.sub("w"), ps.w);
            ufres->put(target.sub("w"), ps.w);

            ufin->get(target.sub("alpha"), ps.alpha);
            ufout->put(target.sub("alpha"), ps.alpha);
            ufres->put(target.sub("alpha"), ps.alpha);

            ufin->get(target.sub("kappa"), ps.kappa);
            ufout->put(target.sub("kappa"), ps.kappa);
            ufres->put(target.sub("kappa"), ps.kappa);

          } else if (str == "Flory_Huggins") {
            SW_POTENTIAL = Flory_Huggins;
            target.down("Flory_Huggins");
            ufin->get(target.sub("composition_ratio"), ps.ratio);
            ufout->put(target.sub("composition_ratio"), ps.ratio);
            ufres->put(target.sub("composition_ratio"), ps.ratio);

            ufin->get(target.sub("initial_fluctuation"), ps.init_fluct);
            ufout->put(target.sub("initial_fluctuation"), ps.init_fluct);
            ufres->put(target.sub("initial_fluctuation"), ps.init_fluct);

            ufin->get(target.sub("na"), fh.na);
            ufout->put(target.sub("na"), fh.na);
            ufres->put(target.sub("na"), fh.na);

            ufin->get(target.sub("nb"), fh.nb);
            ufout->put(target.sub("nb"), fh.nb);
            ufres->put(target.sub("nb"), fh.nb);

            ufin->get(target.sub("chi"), fh.chi);
            ufout->put(target.sub("chi"), fh.chi);
            ufres->put(target.sub("chi"), fh.chi);

            ufin->get(target.sub("d"), ps.d);
            ufout->put(target.sub("d"), ps.d);
            ufres->put(target.sub("d"), ps.d);

            ufin->get(target.sub("w"), ps.w);
            ufout->put(target.sub("w"), ps.w);
            ufres->put(target.sub("w"), ps.w);

            ufin->get(target.sub("alpha"), ps.alpha);
            ufout->put(target.sub("alpha"), ps.alpha);
            ufres->put(target.sub("alpha"), ps.alpha);

            ufin->get(target.sub("kappa"), ps.kappa);
            ufout->put(target.sub("kappa"), ps.kappa);
            ufres->put(target.sub("kappa"), ps.kappa);
          } else {
            fprintf(stderr, "invalid potential_type\n");
            exit_job(EXIT_FAILURE);
          }
          target.up();
        }
      }
    } else if (str == EQ_name[Shear_Navier_Stokes_Lees_Edwards_FDM]) {
      SW_EQ            = Shear_Navier_Stokes_Lees_Edwards_FDM;
      PHASE_SEPARATION = 0;
      {
        target.down(EQ_name[SW_EQ]);
        ufin->get(target.sub("NS_solver.type"), str);
        ufout->put(target.sub("NS_solver.type"), str);
        ufres->put(target.sub("NS_solver.type"), str);
        {
          if (str == NS_SOLVERTYPE_name[explicit_scheme]) {
            SW_NSST = explicit_scheme;
          } else if (str == NS_SOLVERTYPE_name[implicit_scheme]) {
            SW_NSST = implicit_scheme;
            target.down("NS_solver.implicit_scheme");

            ufin->get(target.sub("tolerance"), eps_ns);
            ufout->put(target.sub("tolerance"), eps_ns);
            ufres->put(target.sub("tolerance"), eps_ns);

            ufin->get(target.sub("maximum_iteration"), maxiter_ns);
            ufout->put(target.sub("maximum_iteration"), maxiter_ns);
            ufres->put(target.sub("maximum_iteration"), maxiter_ns);
            target.up();
            target.up();
          } else {
            fprintf(stderr, "invalid NS SOLVER TYPE\n");
            exit_job(EXIT_FAILURE);
          }
        }
        {
          ufin->get(target.sub("DX"), DX);
          ufin->get(target.sub("RHO"), RHO);
          ufin->get(target.sub("ETA"), ETA);
          ufin->get(target.sub("kBT"), kBT);
          ufin->get(target.sub("alpha_v"), alpha_v);
          ufin->get(target.sub("alpha_o"), alpha_o);
        }
        Shear_strain_realized = 0.0;
        { Srate_depend_LJ_cap = DBL_MAX; }
        {
          ufout->put(target.sub("DX"), DX);
          ufout->put(target.sub("RHO"), RHO);
          ufout->put(target.sub("ETA"), ETA);
          ufout->put(target.sub("kBT"), kBT);
          ufout->put(target.sub("alpha_v"), alpha_v);
          ufout->put(target.sub("alpha_o"), alpha_o);
        }
        {
          ufres->put(target.sub("DX"), DX);
          ufres->put(target.sub("RHO"), RHO);
          ufres->put(target.sub("ETA"), ETA);
          ufres->put(target.sub("kBT"), kBT);
          ufres->put(target.sub("alpha_v"), alpha_v);
          ufres->put(target.sub("alpha_o"), alpha_o);
        }
        {
          Location target("constitutive_eq.Shear_Navier_Stokes_Lees_Edwards_FDM.External_field");
          ufin->get(target.sub("type"), str);
          ufout->put(target.sub("type"), str);
          ufres->put(target.sub("type"), str);
          if (str == "DC") {
            target.down("DC");
            Shear_AC = 0;
            ufin->get(target.sub("Shear_rate"), Shear_rate);
            ufout->put(target.sub("Shear_rate"), Shear_rate);
            ufres->put(target.sub("Shear_rate"), Shear_rate);
            fprintf(stderr, "# DC steady shear: shear rate %f \n", Shear_rate);
          }
          if (str == "AC") {  // in near future, someone will extend this section.
                              // AC by otomura
            target.down("AC");
            Shear_AC = 1;
            ufin->get(target.sub("Shear_rate"), Shear_rate);
            ufout->put(target.sub("Shear_rate"), Shear_rate);
            ufres->put(target.sub("Shear_rate"), Shear_rate);
            ufin->get(target.sub("Frequency"), Shear_frequency);
            ufout->put(target.sub("Frequency"), Shear_frequency);
            ufres->put(target.sub("Frequency"), Shear_frequency);
            fprintf(stderr,
                    "# AC oscillatory shear: (shear rate, frequency, the maximum amp of strain)= %f %f %f\n",
                    Shear_rate,
                    Shear_frequency,
                    Shear_rate / Shear_frequency);
          }
        }
      }
    } else if (str == EQ_name[Shear_NS_LE_CH_FDM]) {
      SW_EQ            = Shear_NS_LE_CH_FDM;
      PHASE_SEPARATION = 1;
      {
        target.down(EQ_name[SW_EQ]);
        ufin->get(target.sub("NS_solver.type"), str);
        ufout->put(target.sub("NS_solver.type"), str);
        ufres->put(target.sub("NS_solver.type"), str);
        {
          if (str == NS_SOLVERTYPE_name[explicit_scheme]) {
            SW_NSST = explicit_scheme;
          } else if (str == NS_SOLVERTYPE_name[implicit_scheme]) {
            SW_NSST = implicit_scheme;
            target.down("NS_solver.implicit_scheme");

            ufin->get(target.sub("tolerance"), eps_ns);
            ufout->put(target.sub("tolerance"), eps_ns);
            ufres->put(target.sub("tolerance"), eps_ns);

            ufin->get(target.sub("maximum_iteration"), maxiter_ns);
            ufout->put(target.sub("maximum_iteration"), maxiter_ns);
            ufres->put(target.sub("maximum_iteration"), maxiter_ns);

            ufin->get(target.sub("viscosity_change"), str);
            ufout->put(target.sub("viscosity_change"), str);
            ufres->put(target.sub("viscosity_change"), str);
            {
              if (str == "OFF") {
                VISCOSITY_CHANGE = 0;
              } else if (str == "ON") {
                VISCOSITY_CHANGE = 1;
                target.down("ON");
                ufin->get(target.sub("ETA_A"), ETA_A);
                ufout->put(target.sub("ETA_A"), ETA_A);
                ufres->put(target.sub("ETA_A"), ETA_A);

                ufin->get(target.sub("ETA_B"), ETA_B);
                ufout->put(target.sub("ETA_B"), ETA_B);
                ufres->put(target.sub("ETA_B"), ETA_B);
                target.up();
              } else {
                fprintf(stderr, "invalid viscosity change slection \n");
                exit_job(EXIT_FAILURE);
              }
            }

            target.up();
            target.up();
          } else {
            fprintf(stderr, "invalid NS SOLVER TYPE\n");
            exit_job(EXIT_FAILURE);
          }
        }
        ufin->get(target.sub("CH_solver.type"), str);
        ufout->put(target.sub("CH_solver.type"), str);
        ufres->put(target.sub("CH_solver.type"), str);
        {
          if (str == CH_SOLVERTYPE_name[explicit_scheme]) {
            SW_CHST = explicit_scheme;
          } else if (str == CH_SOLVERTYPE_name[implicit_scheme]) {
            SW_CHST = implicit_scheme;
            target.down("CH_solver.implicit_scheme");

            ufin->get(target.sub("tolerance"), eps_ch);
            ufout->put(target.sub("tolerance"), eps_ch);
            ufres->put(target.sub("tolerance"), eps_ch);

            ufin->get(target.sub("maximum_iteration"), maxiter_ch);
            ufout->put(target.sub("maximum_iteration"), maxiter_ch);
            ufres->put(target.sub("maximum_iteration"), maxiter_ch);

            target.up();
            target.up();
          } else {
            fprintf(stderr, "invalid CH SOLVER TYPE\n");
            exit_job(EXIT_FAILURE);
          }
        }
        {
          ufin->get(target.sub("DX"), DX);
          ufin->get(target.sub("RHO"), RHO);
          ufin->get(target.sub("ETA"), ETA);
          ufin->get(target.sub("kBT"), kBT);
          ufin->get(target.sub("alpha_v"), alpha_v);
          ufin->get(target.sub("alpha_o"), alpha_o);
        }
        {
          Location target("constitutive_eq.Shear_NS_LE_CH_FDM.Potential");
          ufin->get(target.sub("type"), str);
          ufout->put(target.sub("type"), str);
          ufres->put(target.sub("type"), str);

          if (str == "Landau") {
            SW_POTENTIAL = Landau;
            target.down("Landau");
            ufin->get(target.sub("composition_ratio"), ps.ratio);
            ufout->put(target.sub("composition_ratio"), ps.ratio);
            ufres->put(target.sub("composition_ratio"), ps.ratio);

            ufin->get(target.sub("initial_fluctuation"), ps.init_fluct);
            ufout->put(target.sub("initial_fluctuation"), ps.init_fluct);
            ufres->put(target.sub("initial_fluctuation"), ps.init_fluct);

            ufin->get(target.sub("a"), gl.a);
            ufout->put(target.sub("a"), gl.a);
            ufres->put(target.sub("a"), gl.a);

            ufin->get(target.sub("b"), gl.b);
            ufout->put(target.sub("b"), gl.b);
            ufres->put(target.sub("b"), gl.b);

            ufin->get(target.sub("d"), ps.d);
            ufout->put(target.sub("d"), ps.d);
            ufres->put(target.sub("d"), ps.d);

            ufin->get(target.sub("w"), ps.w);
            ufout->put(target.sub("w"), ps.w);
            ufres->put(target.sub("w"), ps.w);

            ufin->get(target.sub("alpha"), ps.alpha);
            ufout->put(target.sub("alpha"), ps.alpha);
            ufres->put(target.sub("alpha"), ps.alpha);

            ufin->get(target.sub("kappa"), ps.kappa);
            ufout->put(target.sub("kappa"), ps.kappa);
            ufres->put(target.sub("kappa"), ps.kappa);

          } else if (str == "Flory_Huggins") {
            SW_POTENTIAL = Flory_Huggins;
            target.down("Flory_Huggins");
            ufin->get(target.sub("composition_ratio"), ps.ratio);
            ufout->put(target.sub("composition_ratio"), ps.ratio);
            ufres->put(target.sub("composition_ratio"), ps.ratio);

            ufin->get(target.sub("initial_fluctuation"), ps.init_fluct);
            ufout->put(target.sub("initial_fluctuation"), ps.init_fluct);
            ufres->put(target.sub("initial_fluctuation"), ps.init_fluct);

            ufin->get(target.sub("na"), fh.na);
            ufout->put(target.sub("na"), fh.na);
            ufres->put(target.sub("na"), fh.na);

            ufin->get(target.sub("nb"), fh.nb);
            ufout->put(target.sub("nb"), fh.nb);
            ufres->put(target.sub("nb"), fh.nb);

            ufin->get(target.sub("chi"), fh.chi);
            ufout->put(target.sub("chi"), fh.chi);
            ufres->put(target.sub("chi"), fh.chi);

            ufin->get(target.sub("d"), ps.d);
            ufout->put(target.sub("d"), ps.d);
            ufres->put(target.sub("d"), ps.d);

            ufin->get(target.sub("w"), ps.w);
            ufout->put(target.sub("w"), ps.w);
            ufres->put(target.sub("w"), ps.w);

            ufin->get(target.sub("alpha"), ps.alpha);
            ufout->put(target.sub("alpha"), ps.alpha);
            ufres->put(target.sub("alpha"), ps.alpha);

            ufin->get(target.sub("kappa"), ps.kappa);
            ufout->put(target.sub("kappa"), ps.kappa);
            ufres->put(target.sub("kappa"), ps.kappa);
          } else {
            fprintf(stderr, "invalid potential_type\n");
            exit_job(EXIT_FAILURE);
          }
          target.up();
        }
        Shear_strain_realized = 0.0;
        { Srate_depend_LJ_cap = DBL_MAX; }
        {
          ufout->put(target.sub("DX"), DX);
          ufout->put(target.sub("RHO"), RHO);
          ufout->put(target.sub("ETA"), ETA);
          ufout->put(target.sub("kBT"), kBT);
          ufout->put(target.sub("alpha_v"), alpha_v);
          ufout->put(target.sub("alpha_o"), alpha_o);
        }
        {
          ufres->put(target.sub("DX"), DX);
          ufres->put(target.sub("RHO"), RHO);
          ufres->put(target.sub("ETA"), ETA);
          ufres->put(target.sub("kBT"), kBT);
          ufres->put(target.sub("alpha_v"), alpha_v);
          ufres->put(target.sub("alpha_o"), alpha_o);
        }
        {
          Location target("constitutive_eq.Shear_NS_LE_CH_FDM.External_field");
          ufin->get(target.sub("type"), str);
          ufout->put(target.sub("type"), str);
          ufres->put(target.sub("type"), str);
          if (str == "DC") {
            target.down("DC");
            Shear_AC = 0;
            ufin->get(target.sub("Shear_rate"), Shear_rate);
            ufout->put(target.sub("Shear_rate"), Shear_rate);
            ufres->put(target.sub("Shear_rate"), Shear_rate);
            fprintf(stderr, "# DC steady shear: shear rate %f \n", Shear_rate);
          }
          if (str == "AC") {  // in near future, someone will extend this section.
                              // AC by otomura
            target.down("AC");
            Shear_AC = 1;
            ufin->get(target.sub("Shear_rate"), Shear_rate);
            ufout->put(target.sub("Shear_rate"), Shear_rate);
            ufres->put(target.sub("Shear_rate"), Shear_rate);
            ufin->get(target.sub("Frequency"), Shear_frequency);
            ufout->put(target.sub("Frequency"), Shear_frequency);
            ufres->put(target.sub("Frequency"), Shear_frequency);
            fprintf(stderr,
                    "# AC oscillatory shear: (shear rate, frequency, the maximum amp of strain)= %f %f %f\n",
                    Shear_rate,
                    Shear_frequency,
                    Shear_rate / Shear_frequency);
          }
        }
      }
    } else {
      fprintf(stderr, "invalid constitutive_eq\n");
      exit_job(EXIT_FAILURE);
    }
    fprintf(stderr, "#\n# %s eq. selected.\n", EQ_name[SW_EQ]);
  }

  /////// 計算条件の設定
  {
    Location target("object_type");
    string   str;
    ufin->get(target.sub("type"), str);
    ufout->put(target.sub("type"), str);
    ufres->put(target.sub("type"), str);
    if (str == PT_name[spherical_particle]) {
      SW_PT = spherical_particle;
      {
        Component_Number = ufin->size("object_type.spherical_particle.Particle_spec[]");
        // fprintf(stderr, "# %s %d\n",str, Component_Number);
        {
          MASS_RATIOS      = alloc_1d_double(Component_Number);
          Particle_Numbers = alloc_1d_int(Component_Number);
          RHO_particle     = alloc_1d_double(Component_Number);
          MASS             = alloc_1d_double(Component_Number);
          IMASS            = alloc_1d_double(Component_Number);
          IMASS_RATIOS     = alloc_1d_double(Component_Number);
          MOI              = alloc_1d_double(Component_Number);
          IMOI             = alloc_1d_double(Component_Number);

          RADII = alloc_1d_double(Component_Number);

          S_surfaces = alloc_1d_double(Component_Number);
          W_surfaces = alloc_1d_double(Component_Number);

          Surface_charge   = alloc_1d_double(Component_Number);
          Surface_charge_e = alloc_1d_double(Component_Number);

          int num_pairs = Component_Number * Component_Number;
          EPSILON       = alloc_1d_double(num_pairs);
          A_R_cutoff    = alloc_1d_double(num_pairs);
          LJ_dia        = alloc_1d_double(num_pairs);
          LJ_truncate   = alloc_1d_int(num_pairs);
          LJ_powers     = alloc_1d_int(num_pairs);
          SIGMA         = alloc_1d_double(num_paris);

          PATCHY_POWER      = alloc_1d_int(num_pairs);
          PATCHY_LAMBDA     = alloc_1d_double(num_pairs);
          PATCHY_EPSILON    = alloc_1d_double(num_pairs);
          PATCHY_A_R_cutoff = alloc_1d_double(num_pairs);

          janus_axis       = (JAX *)malloc(sizeof(JAX) * Component_Number);
          janus_propulsion = (JP *)malloc(sizeof(JP) * Component_Number);
          janus_force      = alloc_2d_double(Component_Number, DIM);
          janus_torque     = alloc_2d_double(Component_Number, DIM);
          janus_slip_vel   = alloc_1d_double(Component_Number);
          janus_slip_mode  = alloc_1d_double(Component_Number);
        }
      }
    } else if (str == PT_name[chain]) {
      SW_PT            = chain;
      Component_Number = ufin->size("object_type.chain.Chain_spec[]");
      {
        MASS_RATIOS      = alloc_1d_double(Component_Number);
        Particle_Numbers = alloc_1d_int(Component_Number);
        Beads_Numbers    = alloc_1d_int(Component_Number);
        Chain_Numbers    = alloc_1d_int(Component_Number);
        RHO_particle     = alloc_1d_double(Component_Number);
        MASS             = alloc_1d_double(Component_Number);
        IMASS            = alloc_1d_double(Component_Number);
        IMASS_RATIOS     = alloc_1d_double(Component_Number);
        MOI              = alloc_1d_double(Component_Number);
        IMOI             = alloc_1d_double(Component_Number);

        RADII = alloc_1d_double(Component_Number);

        S_surfaces = alloc_1d_double(Component_Number);
        W_surfaces = alloc_1d_double(Component_Number);

        Surface_charge   = alloc_1d_double(Component_Number);
        Surface_charge_e = alloc_1d_double(Component_Number);

        int num_pairs = Component_Number * Component_Number;
        EPSILON       = alloc_1d_double(num_pairs);
        A_R_cutoff    = alloc_1d_double(num_pairs);
        LJ_dia        = alloc_1d_double(num_pairs);
        LJ_truncate   = alloc_1d_int(num_pairs);
        LJ_powers     = alloc_1d_int(num_pairs);
        SIGMA         = alloc_1d_double(num_pairs);

        PATCHY_POWER      = alloc_1d_int(num_pairs);
        PATCHY_LAMBDA     = alloc_1d_double(num_pairs);
        PATCHY_EPSILON    = alloc_1d_double(num_pairs);
        PATCHY_A_R_cutoff = alloc_1d_double(num_pairs);

        janus_axis       = (JAX *)malloc(sizeof(JAX) * Component_Number);
        janus_propulsion = (JP *)malloc(sizeof(JP) * Component_Number);
        janus_force      = NULL;
        janus_torque     = NULL;
        janus_slip_vel   = NULL;
        janus_slip_mode  = NULL;
      }
    } else if (str == PT_name[rigid]) {
      SW_PT            = rigid;
      Component_Number = ufin->size("object_type.rigid.Rigid_spec[]");
      {
        MASS_RATIOS      = alloc_1d_double(Component_Number);
        Particle_Numbers = alloc_1d_int(Component_Number);
        Beads_Numbers    = alloc_1d_int(Component_Number);
        Chain_Numbers    = alloc_1d_int(Component_Number);
        RHO_particle     = alloc_1d_double(Component_Number);
        MASS             = alloc_1d_double(Component_Number);
        IMASS            = alloc_1d_double(Component_Number);
        IMASS_RATIOS     = alloc_1d_double(Component_Number);
        MOI              = alloc_1d_double(Component_Number);
        IMOI             = alloc_1d_double(Component_Number);

        RADII = alloc_1d_double(Component_Number);

        S_surfaces = alloc_1d_double(Component_Number);
        W_surfaces = alloc_1d_double(Component_Number);

        Surface_charge   = alloc_1d_double(Component_Number);
        Surface_charge_e = alloc_1d_double(Component_Number);

        Rigid_Motions_vel   = alloc_2d_int(Component_Number, DIM);
        Rigid_Motions_omega = alloc_2d_int(Component_Number, DIM);
        Rigid_Velocities    = alloc_2d_double(Component_Number, DIM);
        Rigid_Omegas        = alloc_2d_double(Component_Number, DIM);

        int num_pairs = Component_Number * Component_Number;
        EPSILON       = alloc_1d_double(num_pairs);
        A_R_cutoff    = alloc_1d_double(num_pairs);
        LJ_dia        = alloc_1d_double(num_pairs);
        LJ_truncate   = alloc_1d_int(num_pairs);
        LJ_powers     = alloc_1d_int(num_pairs);
        SIGMA         = alloc_1d_double(num_pairs);

        PATCHY_POWER      = alloc_1d_int(num_pairs);
        PATCHY_LAMBDA     = alloc_1d_double(num_pairs);
        PATCHY_EPSILON    = alloc_1d_double(num_pairs);
        PATCHY_A_R_cutoff = alloc_1d_double(num_pairs);

        janus_axis       = (JAX *)malloc(sizeof(JAX) * Component_Number);
        janus_propulsion = (JP *)malloc(sizeof(JP) * Component_Number);
        janus_force      = NULL;
        janus_torque     = NULL;
        janus_slip_vel   = NULL;
        janus_slip_mode  = NULL;
      }
    }
  }

  // section Janus_setting
  {
    {
      fprintf(stderr, "#\n");
      if (SW_PT == spherical_particle) {
        int d = 1;
        fprintf(stderr, "#%d:species", d++);
        fprintf(stderr, " %d:number_of_particle[i]", d++);
        fprintf(stderr, " %d:mass_density_ratio[i]", d++);
        if (SW_EQ == Electrolyte) {
          fprintf(stderr, " %d:Surface_charge[i]", d++);
        }
        fprintf(stderr, " %d:janus_axis[i]", d++);
        fprintf(stderr, " %d:janus_mode[i]", d++);
        fprintf(stderr, " %d:janus_frc_x[i]", d++);
        fprintf(stderr, " %d:janus_frc_y[i]", d++);
        fprintf(stderr, " %d:janus_frc_z[i]", d++);
        fprintf(stderr, " %d:janus_trq_x[i]", d++);
        fprintf(stderr, " %d:janus_trq_y[i]", d++);
        fprintf(stderr, " %d:janus_trq_z[i]", d++);
        fprintf(stderr, " %d:squirm_b1[i]", d++);
        fprintf(stderr, " %d:squirm_b2[i]", d++);
      } else if (SW_PT == chain) {
        int d = 1;
        fprintf(stderr, "#%d:species", d++);
        fprintf(stderr, " %d:total_number_of_particle[i]", d++);
        fprintf(stderr, " %d:number_of_beads[i]", d++);
        fprintf(stderr, " %d:number_of_chain[i]", d++);
        fprintf(stderr, " %d:mass_density_ratio[i]", d++);
        if (SW_EQ == Electrolyte) {
          fprintf(stderr, " %d:Surface_charge[i]", d++);
        }
        fprintf(stderr, "%d:janus_axis[i]", d++);
      } else if (SW_PT == rigid) {
        int d = 1;
        fprintf(stderr, "#%d:species", d++);
        fprintf(stderr, " %d:total_number_of_particle[i]", d++);
        fprintf(stderr, " %d:number_of_beads[i]", d++);
        fprintf(stderr, " %d:number_of_chain[i]", d++);
        fprintf(stderr, " %d:mass_density_ratio[i]", d++);
        if (SW_EQ == Electrolyte) {
          fprintf(stderr, " %d:Surface_charge[i]", d++);
        }
        fprintf(stderr, " %d:Rigid_velocity[i]", d++);
        fprintf(stderr, " %d:Rigid_omega[i]", d++);
      }
      fprintf(stderr, "\n");
    }

    SW_JANUS       = 0;
    SW_JANUS_MOTOR = 0;
    SW_JANUS_SLIP  = 0;
    if (SW_PT == spherical_particle) {
      for (int i = 0; i < Component_Number; i++) {
        char str[256];
        sprintf(str, "object_type.spherical_particle.Particle_spec[%d]", i);
        Location target(str);
        {
          string str_in;
          ufin->get(target.sub("Particle_number"), Particle_Numbers[i]);
          ufin->get(target.sub("MASS_RATIO"), MASS_RATIOS[i]);
          ufin->get(target.sub("Surface_charge"), Surface_charge[i]);

          ufin->get(target.sub("janus_axis"), str_in);
          if (str_in == JAX_name[no_axis]) {
            janus_axis[i] = no_axis;
          } else if (str_in == JAX_name[x_axis]) {
            janus_axis[i] = x_axis;
            SW_JANUS      = 1;
          } else if (str_in == JAX_name[y_axis]) {
            janus_axis[i] = y_axis;
            SW_JANUS      = 1;
          } else if (str_in == JAX_name[z_axis]) {
            janus_axis[i] = z_axis;
            SW_JANUS      = 1;
          } else {
            fprintf(stderr, "ERROR: Unknown axis specification\n");
            exit_job(EXIT_FAILURE);
          }

          ufin->get(target.sub("janus_propulsion"), str_in);
          if (str_in == JP_name[no_propulsion]) {
            janus_propulsion[i] = no_propulsion;
          } else if (str_in == JP_name[obstacle]) {
            janus_propulsion[i] = obstacle;
          } else if (str_in == JP_name[motor]) {
            janus_propulsion[i] = motor;
            SW_JANUS_MOTOR      = 1;
          } else if (str_in == JP_name[slip]) {
            janus_propulsion[i] = slip;
            SW_JANUS_SLIP       = 1;
          } else {
            fprintf(stderr, "ERROR: Unknown propulsion mechanism\n");
            exit_job(EXIT_FAILURE);
          }

          // self-force/torque in body coordinates
          if (janus_propulsion[i] == motor) {
            ufin->get(target.sub("janus_force.x"), janus_force[i][0]);
            ufin->get(target.sub("janus_force.y"), janus_force[i][1]);
            ufin->get(target.sub("janus_force.z"), janus_force[i][2]);

            ufin->get(target.sub("janus_torque.x"), janus_torque[i][0]);
            ufin->get(target.sub("janus_torque.y"), janus_torque[i][1]);
            ufin->get(target.sub("janus_torque.z"), janus_torque[i][2]);
          } else {
            for (int d = 0; d < DIM; d++) {
              janus_force[i][d]  = 0.0;
              janus_torque[i][d] = 0.0;
            }
          }

          // squirmer with surface slip velocity
          if (janus_propulsion[i] == slip) {
            ufin->get(target.sub("janus_slip_vel"), janus_slip_vel[i]);    // B1 coeff
            ufin->get(target.sub("janus_slip_mode"), janus_slip_mode[i]);  // alpha=B2/B1
            assert(janus_slip_vel[i] > 0);
          } else {
            janus_slip_vel[i]  = 0.0;
            janus_slip_mode[i] = 0.0;
          }
        }
        {
          ufout->put(target.sub("Particle_number"), Particle_Numbers[i]);
          ufout->put(target.sub("MASS_RATIO"), MASS_RATIOS[i]);
          ufout->put(target.sub("Surface_charge"), Surface_charge[i]);
          ufout->put(target.sub("janus_axis"), JAX_name[janus_axis[i]]);
          ufout->put(target.sub("janus_propulsion"), JP_name[janus_propulsion[i]]);
          ufout->put(target.sub("janus_force.x"), janus_force[i][0]);
          ufout->put(target.sub("janus_force.y"), janus_force[i][1]);
          ufout->put(target.sub("janus_force.z"), janus_force[i][2]);
          ufout->put(target.sub("janus_torque.x"), janus_torque[i][0]);
          ufout->put(target.sub("janus_torque.y"), janus_torque[i][1]);
          ufout->put(target.sub("janus_torque.z"), janus_torque[i][2]);
          ufout->put(target.sub("janus_slip_vel"), janus_slip_vel[i]);
          ufout->put(target.sub("janus_slip_mode"), janus_slip_mode[i]);
        }
        {
          ufres->put(target.sub("Particle_number"), Particle_Numbers[i]);
          ufres->put(target.sub("MASS_RATIO"), MASS_RATIOS[i]);
          ufres->put(target.sub("Surface_charge"), Surface_charge[i]);
          ufres->put(target.sub("janus_axis"), JAX_name[janus_axis[i]]);
          ufres->put(target.sub("janus_propulsion"), JP_name[janus_propulsion[i]]);
          ufres->put(target.sub("janus_force.x"), janus_force[i][0]);
          ufres->put(target.sub("janus_force.y"), janus_force[i][1]);
          ufres->put(target.sub("janus_force.z"), janus_force[i][2]);
          ufres->put(target.sub("janus_torque.x"), janus_torque[i][0]);
          ufres->put(target.sub("janus_torque.y"), janus_torque[i][1]);
          ufres->put(target.sub("janus_torque.z"), janus_torque[i][2]);
          ufres->put(target.sub("janus_slip_vel"), janus_slip_vel[i]);
          ufres->put(target.sub("janus_slip_mode"), janus_slip_mode[i]);
        }
        if (SW_EQ == Electrolyte) {
          fprintf(stderr,
                  "#%d %d %g %g %s %s %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",
                  i,
                  Particle_Numbers[i],
                  MASS_RATIOS[i],
                  Surface_charge[i],
                  JAX_name[janus_axis[i]],
                  JP_name[janus_propulsion[i]],
                  janus_force[i][0],
                  janus_force[i][1],
                  janus_force[i][2],
                  janus_torque[i][0],
                  janus_torque[i][1],
                  janus_torque[i][2],
                  janus_slip_vel[i],
                  janus_slip_mode[i]);
        } else {
          fprintf(stderr,
                  "#%d %d %g %s %s %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",
                  i,
                  Particle_Numbers[i],
                  MASS_RATIOS[i],
                  JAX_name[janus_axis[i]],
                  JP_name[janus_propulsion[i]],
                  janus_force[i][0],
                  janus_force[i][1],
                  janus_force[i][2],
                  janus_torque[i][0],
                  janus_torque[i][1],
                  janus_torque[i][2],
                  janus_slip_vel[i],
                  janus_slip_mode[i]);
        }

        fprintf(stderr, "#\n");
        fprintf(stderr, "# Spherical Particles selected.\n");
      }  // components
      if (SW_EQ != Navier_Stokes && (SW_JANUS_MOTOR == 1 || SW_JANUS_SLIP == 1)) {
        fprintf(stderr, "# Janus particles only implemented for Navier-Stokes solver...\n");
        exit_job(EXIT_FAILURE);
      }
    } else if (SW_PT == chain) {
      for (int i = 0; i < Component_Number; i++) {
        char str[256];
        sprintf(str, "object_type.chain.Chain_spec[%d]", i);
        Location target(str);
        {
          ufin->get(target.sub("Beads_number"), Beads_Numbers[i]);
          ufin->get(target.sub("Chain_number"), Chain_Numbers[i]);
          ufin->get(target.sub("MASS_RATIO"), MASS_RATIOS[i]);
          ufin->get(target.sub("Surface_charge"), Surface_charge[i]);

          string str_in;
          ufin->get(target.sub("janus_axis"), str_in);
          if (str_in == JAX_name[no_axis]) {
            janus_axis[i] = no_axis;
          } else if (str_in == JAX_name[x_axis]) {
            janus_axis[i] = x_axis;
            SW_JANUS      = 1;
          } else if (str_in == JAX_name[y_axis]) {
            janus_axis[i] = y_axis;
            SW_JANUS      = 1;
          } else if (str_in == JAX_name[z_axis]) {
            janus_axis[i] = z_axis;
            SW_JANUS      = 1;
          } else {
            fprintf(stderr, "ERROR: Unknown axis specification\n");
            exit_job(EXIT_FAILURE);
          }
          janus_propulsion[i] = no_propulsion;
        }
        {
          ufout->put(target.sub("Beads_number"), Beads_Numbers[i]);
          ufout->put(target.sub("Chain_number"), Chain_Numbers[i]);
          ufout->put(target.sub("MASS_RATIO"), MASS_RATIOS[i]);
          ufout->put(target.sub("Surface_charge"), Surface_charge[i]);

          ufout->put(target.sub("janus_axis"), JAX_name[janus_axis[i]]);
          ufout->put(target.sub("janus_propulsion"), JP_name[janus_propulsion[i]]);
          ufout->put(target.sub("janus_force.x"), 0.0);
          ufout->put(target.sub("janus_force.y"), 0.0);
          ufout->put(target.sub("janus_force.z"), 0.0);
          ufout->put(target.sub("janus_torque.x"), 0.0);
          ufout->put(target.sub("janus_torque.y"), 0.0);
          ufout->put(target.sub("janus_torque.z"), 0.0);
          ufout->put(target.sub("janus_slip_vel"), 0.0);
          ufout->put(target.sub("janus_slip_mode"), 0.0);
        }
        {
          ufres->put(target.sub("Beads_number"), Beads_Numbers[i]);
          ufres->put(target.sub("Chain_number"), Chain_Numbers[i]);
          ufres->put(target.sub("MASS_RATIO"), MASS_RATIOS[i]);
          ufres->put(target.sub("Surface_charge"), Surface_charge[i]);

          ufres->put(target.sub("janus_axis"), JAX_name[janus_axis[i]]);
          ufres->put(target.sub("janus_propulsion"), JP_name[janus_propulsion[i]]);
          ufres->put(target.sub("janus_force.x"), 0.0);
          ufres->put(target.sub("janus_force.y"), 0.0);
          ufres->put(target.sub("janus_force.z"), 0.0);
          ufres->put(target.sub("janus_torque.x"), 0.0);
          ufres->put(target.sub("janus_torque.y"), 0.0);
          ufres->put(target.sub("janus_torque.z"), 0.0);
          ufres->put(target.sub("janus_slip_vel"), 0.0);
          ufres->put(target.sub("janus_slip_mode"), 0.0);
        }

        Particle_Numbers[i] = Beads_Numbers[i] * Chain_Numbers[i];

        if (SW_EQ == Electrolyte) {
          fprintf(stderr,
                  "#%d %d %d %d %g %g %s\n",
                  i,
                  Particle_Numbers[i],
                  Beads_Numbers[i],
                  Chain_Numbers[i],
                  MASS_RATIOS[i],
                  Surface_charge[i],
                  JAX_name[janus_axis[i]]);
        } else {
          fprintf(stderr,
                  "#%d %d %d %d %f %s\n",
                  i,
                  Particle_Numbers[i],
                  Beads_Numbers[i],
                  Chain_Numbers[i],
                  MASS_RATIOS[i],
                  JAX_name[janus_axis[i]]);
        }
      }
      fprintf(stderr, "#\n");
      fprintf(stderr, "# Flexible chains selected.\n");
    } else if (SW_PT == rigid) {
      for (int i = 0; i < Component_Number; i++) {
        char   str[256];
        string rigid_str;
        sprintf(str, "object_type.rigid.Rigid_spec[%d]", i);
        Location target(str);
        {
          ufin->get(target.sub("Beads_number"), Beads_Numbers[i]);
          ufin->get(target.sub("Chain_number"), Chain_Numbers[i]);
          ufin->get(target.sub("MASS_RATIO"), MASS_RATIOS[i]);
          ufin->get(target.sub("Surface_charge"), Surface_charge[i]);
          ufin->get(target.sub("Rigid_motion"), rigid_str);
          ufin->get(target.sub("Rigid_velocity.x"), Rigid_Velocities[i][0]);
          ufin->get(target.sub("Rigid_velocity.y"), Rigid_Velocities[i][1]);
          ufin->get(target.sub("Rigid_velocity.z"), Rigid_Velocities[i][2]);
          ufin->get(target.sub("Rigid_omega.x"), Rigid_Omegas[i][0]);
          ufin->get(target.sub("Rigid_omega.y"), Rigid_Omegas[i][1]);
          ufin->get(target.sub("Rigid_omega.z"), Rigid_Omegas[i][2]);
        }
        {
          ufout->put(target.sub("Beads_number"), Beads_Numbers[i]);
          ufout->put(target.sub("Chain_number"), Chain_Numbers[i]);
          ufout->put(target.sub("MASS_RATIO"), MASS_RATIOS[i]);
          ufout->put(target.sub("Surface_charge"), Surface_charge[i]);
          ufout->put(target.sub("Rigid_motion"), rigid_str);
          ufout->put(target.sub("Rigid_velocity.x"), Rigid_Velocities[i][0]);
          ufout->put(target.sub("Rigid_velocity.y"), Rigid_Velocities[i][1]);
          ufout->put(target.sub("Rigid_velocity.z"), Rigid_Velocities[i][2]);
          ufout->put(target.sub("Rigid_omega.x"), Rigid_Omegas[i][0]);
          ufout->put(target.sub("Rigid_omega.y"), Rigid_Omegas[i][1]);
          ufout->put(target.sub("Rigid_omega.z"), Rigid_Omegas[i][2]);
        }
        {
          ufres->put(target.sub("Beads_number"), Beads_Numbers[i]);
          ufres->put(target.sub("Chain_number"), Chain_Numbers[i]);
          ufres->put(target.sub("MASS_RATIO"), MASS_RATIOS[i]);
          ufres->put(target.sub("Surface_charge"), Surface_charge[i]);
          ufres->put(target.sub("Rigid_motion"), rigid_str);
          ufres->put(target.sub("Rigid_velocity.x"), Rigid_Velocities[i][0]);
          ufres->put(target.sub("Rigid_velocity.y"), Rigid_Velocities[i][1]);
          ufres->put(target.sub("Rigid_velocity.z"), Rigid_Velocities[i][2]);
          ufres->put(target.sub("Rigid_omega.x"), Rigid_Omegas[i][0]);
          ufres->put(target.sub("Rigid_omega.y"), Rigid_Omegas[i][1]);
          ufres->put(target.sub("Rigid_omega.z"), Rigid_Omegas[i][2]);
        }

        if (rigid_str == "fix") {
          Rigid_Motions_vel[i][0] = Rigid_Motions_omega[i][0] = 0;
          Rigid_Motions_vel[i][1] = Rigid_Motions_omega[i][1] = 0;
          Rigid_Motions_vel[i][2] = Rigid_Motions_omega[i][2] = 0;
        } else if (rigid_str == "free") {
          Rigid_Motions_vel[i][0] = Rigid_Motions_omega[i][0] = 1;
          Rigid_Motions_vel[i][1] = Rigid_Motions_omega[i][1] = 1;
          Rigid_Motions_vel[i][2] = Rigid_Motions_omega[i][2] = 1;
        } else {
          fprintf(stderr, "invalid Rigid_motion\n");
          exit_job(EXIT_FAILURE);
        }

        janus_axis[i]       = no_axis;
        janus_propulsion[i] = no_propulsion;

        Particle_Numbers[i] = Beads_Numbers[i] * Chain_Numbers[i];

        if (SW_EQ == Electrolyte) {
          fprintf(stderr,
                  "#%d %d %d %d %g %g (%g, %g, %g) (%g, %g, %g)\n",
                  i,
                  Particle_Numbers[i],
                  Beads_Numbers[i],
                  Chain_Numbers[i],
                  MASS_RATIOS[i],
                  Surface_charge[i],
                  Rigid_Velocities[i][0],
                  Rigid_Velocities[i][1],
                  Rigid_Velocities[i][2],
                  Rigid_Omegas[i][0],
                  Rigid_Omegas[i][1],
                  Rigid_Omegas[i][2]);
        } else {
          fprintf(stderr,
                  "#%d %d %d %d %f (%f, %f, %f) (%f, %f, %f)\n",
                  i,
                  Particle_Numbers[i],
                  Beads_Numbers[i],
                  Chain_Numbers[i],
                  MASS_RATIOS[i],
                  Rigid_Velocities[i][0],
                  Rigid_Velocities[i][1],
                  Rigid_Velocities[i][2],
                  Rigid_Omegas[i][0],
                  Rigid_Omegas[i][1],
                  Rigid_Omegas[i][2]);
        }
      }  // close for

      Rigid_Number = 0;
      for (int rigid_i = 0; rigid_i < Component_Number; rigid_i++) {
        Rigid_Number += (Particle_Numbers[rigid_i] > 0 ? Chain_Numbers[rigid_i] : 0);
      }

      // allocation (using Rigid_Number)
      xGs                    = alloc_2d_double(Rigid_Number, DIM);
      xGs_previous           = alloc_2d_double(Rigid_Number, DIM);
      xGs_nopbc              = alloc_2d_double(Rigid_Number, DIM);
      RigidID_Components     = alloc_1d_int(Rigid_Number);
      Rigid_Particle_Numbers = alloc_1d_int(Rigid_Number);
      Rigid_Particle_Cumul   = alloc_1d_int(Rigid_Number + 1);
      Rigid_Masses           = alloc_1d_double(Rigid_Number);
      Rigid_IMasses          = alloc_1d_double(Rigid_Number);
      Rigid_Moments          = alloc_3d_double(Rigid_Number, DIM, DIM);
      Rigid_IMoments         = alloc_3d_double(Rigid_Number, DIM, DIM);
      Rigid_Moments_body     = alloc_3d_double(Rigid_Number, DIM, DIM);

      velocityGs         = alloc_2d_double(Rigid_Number, DIM);
      omegaGs            = alloc_2d_double(Rigid_Number, DIM);
      forceGs            = alloc_2d_double(Rigid_Number, DIM);
      forceGrs           = alloc_2d_double(Rigid_Number, DIM);
      torqueGs           = alloc_2d_double(Rigid_Number, DIM);
      torqueGrs          = alloc_2d_double(Rigid_Number, DIM);
      velocityGs_old     = alloc_2d_double(Rigid_Number, DIM);
      omegaGs_old        = alloc_2d_double(Rigid_Number, DIM);
      forceGs_previous   = alloc_2d_double(Rigid_Number, DIM);
      forceGrs_previous  = alloc_2d_double(Rigid_Number, DIM);
      torqueGs_previous  = alloc_2d_double(Rigid_Number, DIM);
      torqueGrs_previous = alloc_2d_double(Rigid_Number, DIM);
      // initialize
      for (int rigidID = 0; rigidID < Rigid_Number; rigidID++) {
        for (int d = 0; d < DIM; d++) {
          forceGs[rigidID][d]            = 0.0;
          forceGrs[rigidID][d]           = 0.0;
          torqueGs[rigidID][d]           = 0.0;
          torqueGrs[rigidID][d]          = 0.0;
          forceGs_previous[rigidID][d]   = 0.0;
          forceGrs_previous[rigidID][d]  = 0.0;
          torqueGs_previous[rigidID][d]  = 0.0;
          torqueGrs_previous[rigidID][d] = 0.0;
        }
      }
      fprintf(stderr, "#\n");
      fprintf(stderr, "# Rigid chains selected.\n");
    }
  }
  fprintf(stderr, "#\n");

  // section surface_thickness
  {
    ufin->get("A_XI", A_XI);
    ufout->put("A_XI", A_XI);
    ufres->put("A_XI", A_XI);
    XI = A_XI * DX;  // surface thickness
    ufin->get("A", dmy_A);
    ufout->put("A", dmy_A);
    ufres->put("A", dmy_A);
    dmy_RADIUS = dmy_A * DX;
    // dmy_SIGMA = 2.0 * dmy_RADIUS;

    for (int i = 0; i < Component_Number; i++) {
      RADII[i] = dmy_RADIUS;
      // SIGMAS[i] = dmy_SIGMA;
    }
  }

  // section polydisperse
  {
    Location target("polydisperse");
    if (ufin->seek(target)) {
      string str;
      ufin->get(target.sub("type"), str);
      ufout->put(target.sub("type"), str);
      ufres->put(target.sub("type"), str);
      if (str == "ON") {
        Location target_radii = Location(target.sub("ON.radii[]"));
        int      radii_num    = ufin->size(target_radii);
        int      dmy_spec;
        double   dmy_radius;
        for (int i = 0; i < radii_num && Component_Number > 0; i++) {
          target_radii.next();

          ufin->get(target_radii.sub("spec"), dmy_spec);
          ufout->put(target_radii.sub("spec"), dmy_spec);
          ufres->put(target_radii.sub("spec"), dmy_spec);

          ufin->get(target_radii.sub("A"), dmy_radius);
          ufout->put(target_radii.sub("A"), dmy_radius);
          ufres->put(target_radii.sub("A"), dmy_radius);
          if (dmy_spec >= 0 && dmy_spec < Component_Number) {
            RADII[dmy_spec] = dmy_radius * DX;
            // SIGMAS[dmy_spec] = 2.0*RADII[dmy_spec];
          } else {
            fprintf(stderr, "# Warning: Unable to set radius of species %d\n", dmy_spec);
          }
        }
      }
    }
  }

  // section gravity
  {
    Location target("gravity");
    ufin->get(target.sub("G"), G);
    string str;
    ufin->get(target.sub("G_direction"), str);
    if (str == "-X") {
      G_direction = 0;
    } else if (str == "-Y") {
      G_direction = 1;
    } else if (str == "-Z") {
      G_direction = 2;
    } else {
      fprintf(stderr, "invalid G_direction\n");
      exit_job(EXIT_FAILURE);
    }
    ufout->put(target.sub("G"), G);
    ufout->put(target.sub("G_direction"), str);

    ufres->put(target.sub("G"), G);
    ufres->put(target.sub("G_direction"), str);
  }

  // section LJ_powers
  {
    int    dmy_powers;
    double dmy_epsilon;
    string str;
    ufin->get("EPSILON", dmy_epsilon);
    ufout->put("EPSILON", dmy_epsilon);
    ufres->put("EPSILON", dmy_epsilon);
    if (dmy_epsilon < 0.0) {
      fprintf(stderr, "invalide EPSILON\n");
      exit_job(EXIT_FAILURE);
    }

    ufin->get("LJ_powers", str);
    ufout->put("LJ_powers", str);
    ufres->put("LJ_powers", str);
    if (str == "12:6") {
      dmy_powers = 0;
    } else if (str == "24:12") {
      dmy_powers = 1;
    } else if (str == "36:18") {
      dmy_powers = 2;
    } else {
      fprintf(stderr, "invalid LJ_powers\n");
      exit_job(EXIT_FAILURE);
    }

    for (int i = 0; i < Component_Number; i++) {
      int im, im2;
      im                 = i * Component_Number + i;
      EPSILON[im]        = dmy_epsilon;
      LJ_powers[im]      = dmy_powers;
      SIGMA[im]          = 2.0 * RADII[i];
      PATCHY_POWER[im]   = dmy_patchy_powers;
      PATCHY_LAMBDA[im]  = dmy_patchy_lambda;
      PATCHY_EPSILON[im] = dmy_patchy_epsilon;
      for (int j = i + 1; j < Component_Number; j++) {
        im          = i * Component_Number + j;
        im2         = j * Component_Number + i;
        EPSILON[im] = EPSILON[im2] = dmy_epsilon;
        LJ_powers[im] = LJ_powers[im2] = dmy_powers;
        SIGMA[im] = SIGMA[im2] = (RADII[i] + RADII[j]);
        PATCHY_POWER[im] = PATCHY_POWER[im2] = dmy_patchy_powers;
        PATCHY_LAMBDA[im] = PATCHY_LAMBDA[im2] = dmy_patchy_lambda;
        PATCHY_EPSILON[im] = PATCHY_EPSILON[im2] = dmy_patchy_epsilon;
      }
    }
  }

  // section mesh
  {
    int      np[DIM];
    Location target("mesh");
    ufin->get(target.sub("NPX"), np[0]);
    ufin->get(target.sub("NPY"), np[1]);
    ufin->get(target.sub("NPZ"), np[2]);

    ufout->put(target.sub("NPX"), np[0]);
    ufout->put(target.sub("NPY"), np[1]);
    ufout->put(target.sub("NPZ"), np[2]);

    ufres->put(target.sub("NPX"), np[0]);
    ufres->put(target.sub("NPY"), np[1]);
    ufres->put(target.sub("NPZ"), np[2]);

    NX          = 1 << np[0];
    Ns_shear[0] = NX;
    NY          = 1 << np[1];
    Ns_shear[1] = NY;
    NZ          = 1 << np[2];
    Ns_shear[2] = NZ;
    Nmax        = MAX(NX, MAX(NY, NZ));
    Nmin        = MIN(NX, MIN(NY, NZ));
  }

  // section time_increment
  {
    Location target("time_increment");
    string   str;
    ufin->get(target.sub("type"), str);
    ufout->put(target.sub("type"), str);
    ufres->put(target.sub("type"), str);
    if (str == "auto") {
      SW_TIME = AUTO;
      ufin->get(target.sub("auto.factor"), Axel);
      ufout->put(target.sub("auto.factor"), Axel);
      ufres->put(target.sub("auto.factor"), Axel);
    } else if (str == "manual") {
      SW_TIME = MANUAL;
      ufin->get(target.sub("manual.delta_t"), DT);
      ufout->put(target.sub("manual.delta_t"), DT);
      ufres->put(target.sub("manual.delta_t"), DT);
    } else {
      fprintf(stderr, "invalid time_increment\n");
      exit_job(EXIT_FAILURE);
    }
  }

  // section switch
  {
    Location target("switch");
    string   str;

    // section rotation
    ufin->get(target.sub("ROTATION"), str);
    ufout->put(target.sub("ROTATION"), str);
    ufres->put(target.sub("ROTATION"), str);
    {
      if (str == "OFF") {
        ROTATION = 0;
        if (SW_JANUS_SLIP == 1 || SW_JANUS_MOTOR == 1) {
          fprintf(stderr, "ROTATION must be turned on for JANUS particles !!!\n");
          exit_job(EXIT_FAILURE);
        }
      } else if (str == "ON") {
        ROTATION = 1;
      } else {
        fprintf(stderr, "invalid ROTATION\n");
        exit_job(EXIT_FAILURE);
      }
    }

    // section LJ truncation
    {  // Set default LJ truncation
      int dmy_truncate;
      ufin->get(target.sub("LJ_truncate"), str);
      ufout->put(target.sub("LJ_truncate"), str);
      ufres->put(target.sub("LJ_truncate"), str);
      if (str == "ON") {
        dmy_truncate = 1;
      } else if (str == "OFF") {
        dmy_truncate = 0;
      } else if (str == "NONE") {
        dmy_truncate = -1;
      } else {
        fprintf(stderr, "invalid LJ_truncate\n");
        exit_job(EXIT_FAILURE);
      }
      for (int i = 0; i < Component_Number; i++) {
        int im, im2;
        im              = i * Component_Number + i;
        LJ_truncate[im] = dmy_truncate;
        for (int j = i + 1; j < Component_Number; j++) {
          im              = i * Component_Number + j;
          im2             = j * Component_Number + i;
          LJ_truncate[im] = LJ_truncate[im2] = dmy_truncate;
        }
      }
    }

    {  // set specific LJ parameters for given species pair interactions
      if (ufin->get(target.sub("LJ_params"), str)) {
        ufout->put(target.sub("LJ_params"), str);
        ufres->put(target.sub("LJ_params"), str);
        if (str == "LJ_spec") {
          Location target_lj = Location(target.sub("LJ_spec.LJ_ij[]"));

          int    spec_i, spec_j, truncate_ij, powers_ij, pair_id, pair_id2;
          double epsilon_ij;
          string truncate_str, powers_str;
          int    N_PAIRS    = ufin->size(target_lj);
          bool   invalid_ij = false;
          for (int npair = 0; npair < N_PAIRS && Component_Number > 0; npair++) {
            target_lj.next();

            ufin->get(target_lj.sub("spec_i"), spec_i);
            ufout->put(target_lj.sub("spec_i"), spec_i);
            ufres->put(target_lj.sub("spec_i"), spec_i);

            ufin->get(target_lj.sub("spec_j"), spec_j);
            ufout->put(target_lj.sub("spec_j"), spec_j);
            ufres->put(target_lj.sub("spec_j"), spec_j);

            ufin->get(target_lj.sub("epsilon"), epsilon_ij);
            ufout->put(target_lj.sub("epsilon"), epsilon_ij);
            ufres->put(target_lj.sub("epsilon"), epsilon_ij);

            ufin->get(target_lj.sub("powers"), powers_str);
            ufout->put(target_lj.sub("powers"), powers_str);
            ufres->put(target_lj.sub("powers"), powers_str);

            ufin->get(target_lj.sub("truncate"), truncate_str);
            ufout->put(target_lj.sub("truncate"), truncate_str);
            ufres->put(target_lj.sub("truncate"), truncate_str);

            // validate input
            if (spec_i < 0 || spec_j < 0 || spec_i >= Component_Number || spec_j >= Component_Number) {
              fprintf(stderr, "# Error: invalid particle species in LJ_spec:\n");
              invalid_ij = true;
            }
            if (epsilon_ij < 0.0) {
              fprintf(stderr, "# Error: invalid epsilon in LJ_spec:\n");
              invalid_ij = true;
            }
            if (powers_str == "12:6") {
              powers_ij = 0;
            } else if (powers_str == "24:12") {
              powers_ij = 1;
            } else if (powers_str == "36:18") {
              powers_ij = 2;
            } else {
              fprintf(stderr, "# Error: invalid powers in LJ_spec:\n");
              invalid_ij = true;
            }

            if (truncate_str == "ON") {
              truncate_ij = 1;
            } else if (truncate_str == "OFF") {
              truncate_ij = 0;
            } else if (truncate_str == "NONE") {
              truncate_ij = -1;
            } else {
              invalid_ij = true;
            }
            if (invalid_ij) {
              fprintf(stderr,
                      "#        spec_i=%d spec_j=%d epsilon=%.3f powers=%s truncate=%s\n",
                      spec_i,
                      spec_j,
                      epsilon_ij,
                      powers_str.c_str(),
                      truncate_str.c_str());
              exit_job(EXIT_FAILURE);
            }

            // reset i-j interaction parameters
            pair_id          = spec_i * Component_Number + spec_j;
            pair_id2         = spec_j * Component_Number + spec_i;
            EPSILON[pair_id] = EPSILON[pair_id2] = epsilon_ij;
            LJ_powers[pair_id] = LJ_powers[pair_id2] = powers_ij;
            LJ_truncate[pair_id] = LJ_truncate[pair_id2] = truncate_ij;
          }
        }
      }

      {
        // default truncation
        // LJ_TRUNCATE = -1 : no need to compte ANY interactions
        // LJ_TRUNCATE = 0  : need to compute at least one pair of interactions
        LJ_TRUNCATE = -1;
        for (int pair_id = 0; pair_id < SQ(Component_Number); pair_id++)
          LJ_TRUNCATE = MAX(LJ_TRUNCATE, MIN(LJ_truncate[pair_id], 0));
      }
    }

    // section INIT_distribution
    {
      target.down("INIT_distribution");
      ufin->get(target.sub("type"), str);
      ufout->put(target.sub("type"), str);
      ufres->put(target.sub("type"), str);

      if (str == "NONE") {
        DISTRIBUTION = None;
      } else if (str == "uniform_random") {
        DISTRIBUTION = uniform_random;
      } else if (str == "random_walk") {
        DISTRIBUTION = random_walk;
        ufin->get(target.sub("random_walk.iteration"), N_iteration_init_distribution);
        ufout->put(target.sub("random_walk.iteration"), N_iteration_init_distribution);
        ufres->put(target.sub("random_walk.iteration"), N_iteration_init_distribution);
      } else if (str == "FCC") {
        DISTRIBUTION = FCC;
      } else if (str == "BCC") {
        DISTRIBUTION = BCC;
      } else if (str == "user_specify") {
        DISTRIBUTION = user_specify;
      } else {
        cerr << str << endl;
        fprintf(stderr, "invalid DISTRIBUTION\n");
        exit_job(EXIT_FAILURE);
      }
      target.up();
    }

    // section INIT_orientation tolerance and iteration number for squirmers
    {
      ufin->get(target.sub("INIT_orientation"), str);
      ufout->put(target.sub("INIT_orientation"), str);
      ufres->put(target.sub("INIT_orientation"), str);
      if (str == "user_specify") {
        ORIENTATION = user_dir;
      } else if (str == "random") {
        ORIENTATION = random_dir;
      } else if (str == "space_align") {
        ORIENTATION = space_dir;
      } else {
        cerr << str << endl;
        fprintf(stderr, "invalid ORIENTATION\n");
        exit_job(EXIT_FAILURE);
      }

      ufin->get(target.sub("SLIP_tol"), MAX_SLIP_TOL);
      ufout->put(target.sub("SLIP_tol"), MAX_SLIP_TOL);
      ufres->put(target.sub("SLIP_tol"), MAX_SLIP_TOL);
      assert(MAX_SLIP_TOL >= 0.0);

      ufin->get(target.sub("SLIP_iter"), MAX_SLIP_ITER);
      ufout->put(target.sub("SLIP_iter"), MAX_SLIP_ITER);
      ufres->put(target.sub("SLIP_iter"), MAX_SLIP_ITER);
      assert(MAX_SLIP_ITER >= 1);
    }

    // section FIX_CELL
    {
      target.down("FIX_CELL");
      {
        const char *xyz[DIM] = {"x", "y", "z"};
        for (int d = 0; d < DIM; d++) {
          ufin->get(target.sub(xyz[d]), str);
          ufout->put(target.sub(xyz[d]), str);
          ufres->put(target.sub(xyz[d]), str);
          if (str == "OFF") {
            FIX_CELLxyz[d] = 0;
          } else if (str == "ON") {
            FIX_CELLxyz[d] = 1;
          } else {
            fprintf(stderr, "invalid FIX_CELL%s\n", xyz[d]);
            exit_job(EXIT_FAILURE);
          }
        }
      }
      if ((SW_EQ == Shear_Navier_Stokes) || (SW_EQ == Shear_Navier_Stokes_Lees_Edwards) ||
          (SW_EQ == Shear_Navier_Stokes_Lees_Edwards_FDM) || (SW_EQ == Shear_NS_LE_CH_FDM)) {
        FIX_CELLxyz[1] = 1;
      }
      FIX_CELL = (FIX_CELLxyz[0] | FIX_CELLxyz[1] | FIX_CELLxyz[2]);
      target.up();
    }

    // section PINNING
    {
      Location target("switch.pin");
      ufin->get(target.sub("type"), str);
      ufout->put(target.sub("type"), str);
      ufres->put(target.sub("type"), str);

      if (str == "YES") {
        PINNING = 1;
      } else if (str == "NO") {
        PINNING = 0;
      }

      if (PINNING) {
        if (SW_PT == rigid) {
          fprintf(stderr, "Error: pinning not yet supported for rigid particles...\n");
          exit_job(EXIT_FAILURE);
        }
        N_PIN = ufin->size("switch.pin.YES.pin[]");
        {
          Pinning_Numbers = alloc_1d_int(N_PIN);
          for (int i = 0; i < N_PIN; i++) {
            int  pin_target;
            char str[256];
            sprintf(str, "switch.pin.YES.pin[%d]", i);
            Location target_pin(str);
            ufin->get(target_pin, pin_target);
            ufout->put(target_pin, pin_target);
            ufres->put(target_pin, pin_target);

            Pinning_Numbers[i] = pin_target;
            fprintf(stderr, "#PINNING %d %d\n", i, pin_target);
          }
        }

        N_PIN_ROT = ufin->size("switch.pin.YES.pin_rot[]");
        {
          Pinning_ROT_Numbers = alloc_1d_int(N_PIN_ROT);
          for (int i = 0; i < N_PIN_ROT; i++) {
            int  pin_rot_target;
            char str_rot[256];
            sprintf(str_rot, "switch.pin.YES.pin_rot[%d]", i);
            Location target_pin_rot(str_rot);
            ufin->get(target_pin_rot, pin_rot_target);
            ufout->put(target_pin_rot, pin_rot_target);
            ufres->put(target_pin_rot, pin_rot_target);

            Pinning_ROT_Numbers[i] = pin_rot_target;
            fprintf(stderr, "#PINNING ROT %d %d\n", i, pin_rot_target);
          }
        }
      }
    }

    // section Rigid Body Degree of Freedom
    {
      if (SW_PT == rigid) {
        fprintf(stderr, "#\n");
        fprintf(stderr, "# Rigid Body Degrees of Freedom DOF: 0 (fix) or 1 (free) :\n");
        fprintf(stderr, "# [spec_id] vx vy vz wx wy wz\n");
        for (int i = 0; i < Component_Number; i++) {
          fprintf(stderr,
                  "# [%d] %d %d %d %d %d %d\n",
                  i,
                  Rigid_Motions_vel[i][0],
                  Rigid_Motions_vel[i][1],
                  Rigid_Motions_vel[i][2],
                  Rigid_Motions_omega[i][0],
                  Rigid_Motions_omega[i][1],
                  Rigid_Motions_omega[i][2]);
        }
      }

      Location target("switch.free_rigid");
      if (ufin->get(target.sub("type"), str)) {
        ufout->put(target.sub("type"), str);
        ufres->put(target.sub("type"), str);

        if (str == "YES") {
          if (SW_PT == rigid) fprintf(stderr, "# WARNING: Switching individual Rigid Body DOF !\n");
          const char *vflag[DIM] = {"vel.x", "vel.y", "vel.z"};
          const char *wflag[DIM] = {"omega.x", "omega.y", "omega.z"};

          int    N_DOF = ufin->size("switch.free_rigid.YES.DOF[]");
          int    target_spec;
          char   buffer[256];
          string flag;

          for (int i = 0; i < N_DOF; i++) {
            sprintf(buffer, "switch.free_rigid.YES.DOF[%d]", i);
            Location target_flag(buffer);
            ufin->get(target_flag.sub("spec_id"), target_spec);
            ufout->put(target_flag.sub("spec_id"), target_spec);
            ufres->put(target_flag.sub("spec_id"), target_spec);
            if (target_spec < 0 || target_spec >= Component_Number) {
              fprintf(stderr, "# Error: species id out of bounds in %s !\n", buffer);
              exit_job(EXIT_FAILURE);
            }

            // switch velocity components on/off
            for (int d = 0; d < DIM; d++) {
              ufin->get(target_flag.sub(vflag[d]), flag);
              ufout->put(target_flag.sub(vflag[d]), flag);
              ufres->put(target_flag.sub(vflag[d]), flag);

              if (SW_PT == rigid) Rigid_Motions_vel[target_spec][d] = (flag == "YES" ? 1 : 0);
            }

            // switch omega components on/off
            for (int d = 0; d < DIM; d++) {
              ufin->get(target_flag.sub(wflag[d]), flag);
              ufout->put(target_flag.sub(wflag[d]), flag);
              ufres->put(target_flag.sub(wflag[d]), flag);

              if (SW_PT == rigid) Rigid_Motions_omega[target_spec][d] = (flag == "YES" ? 1 : 0);
            }

            if (SW_PT == rigid) {
              fprintf(stderr,
                      "# [%d] %d %d %d %d %d %d (new values)\n",
                      target_spec,
                      Rigid_Motions_vel[i][0],
                      Rigid_Motions_vel[i][1],
                      Rigid_Motions_vel[i][2],
                      Rigid_Motions_omega[i][0],
                      Rigid_Motions_omega[i][1],
                      Rigid_Motions_omega[i][2]);
            }
          }
        }
      }
    }

    // section ns_solver
    {
      Location target("switch.ns_solver");
      string   str;

      // default values
      SW_OBL_INT = linear_int;

      if (ufin->get(target.sub("OBL_INT"), str)) {
        ufout->put(target.sub("OBL_INT"), str);
        ufres->put(target.sub("OBL_INT"), str);
        if (str == OBL_INT_name[linear_int]) {
          SW_OBL_INT = linear_int;
        } else if (str == OBL_INT_name[spline_int]) {
          SW_OBL_INT = spline_int;
        } else {
          exit_job(EXIT_FAILURE);
        }
      }
      if (SW_EQ == Shear_Navier_Stokes_Lees_Edwards || SW_EQ == Shear_Navier_Stokes_Lees_Edwards_FDM ||
          SW_EQ == Shear_NS_LE_CH_FDM) {
        if (SW_OBL_INT == linear_int) {
          fprintf(stderr, "# OBL/RCT transform. scheme: linear\n");
        } else if (SW_OBL_INT == spline_int) {
          fprintf(stderr, "# OBL/RCT transform. scheme: periodic spline\n");
        } else {
          exit_job(EXIT_FAILURE);
        }
      }
    }
  }

  // section output
  {  // output;
    string   str;
    Location target("output");
    ufin->get(target.sub("GTS"), GTS);
    ufin->get(target.sub("Num_snap"), Num_snap);

    ufout->put(target.sub("GTS"), GTS);
    ufout->put(target.sub("Num_snap"), Num_snap);

    ufres->put(target.sub("GTS"), GTS);
    ufres->put(target.sub("Num_snap"), Num_snap);
    {  // AVS
      ufin->get(target.sub("AVS"), str);
      ufout->put(target.sub("AVS"), str);
      ufres->put(target.sub("AVS"), str);
      SW_OUTFORMAT = OUT_NONE;
      SW_EXTFORMAT = EXT_OUT_HDF5;

      // default field print flags
      print_field.none     = false;
      print_field.vel      = true;   // print velocity field
      print_field.phi      = true;   // print phi field
      print_field.charge   = true;   // print rho field      (if electrolyte)
      print_field.pressure = false;  // print pressure field (not implemented yet)
      print_field.tau      = true;   // print stress field

      for (int d = 0; d < DIM; d++) {
        print_field_crop.start[d]  = 0;
        print_field_crop.count[d]  = Ns[d];
        print_field_crop.stride[d] = 1;
      }

      if (str == "ON") {
        target.down("ON");
        {
          ufin->get(target.sub("Out_dir"), str);
          ufout->put(target.sub("Out_dir"), str);
          ufres->put(target.sub("Out_dir"), str);
          strcpy(Out_dir, str.c_str());
          dircheckmake(Out_dir);

          ufin->get(target.sub("Out_name"), str);
          ufout->put(target.sub("Out_name"), str);
          ufres->put(target.sub("Out_name"), str);
          strcpy(Out_name, str.c_str());

          ufin->get(target.sub("FileType"), str);
          ufout->put(target.sub("FileType"), str);
          ufres->put(target.sub("FileType"), str);
          if (str == OUTFORMAT_name[OUT_AVS_BINARY]) {
            SW_OUTFORMAT = OUT_AVS_BINARY;
          } else if (str == OUTFORMAT_name[OUT_AVS_ASCII]) {
            SW_OUTFORMAT = OUT_AVS_ASCII;
          } else if (str == OUTFORMAT_name[OUT_EXT]) {
#ifndef WITH_EXTOUT
            fprintf(stderr, "Error: Kapsel compiled without EXTENDED output support\n");
            exit_job(EXIT_FAILURE);
#endif
            SW_OUTFORMAT = OUT_EXT;
            target.down("EXTENDED");
            {
              // extended output options
              target.down("Driver");
              {
                ufin->get(target.sub("Format"), str);
                ufout->put(target.sub("Format"), str);
                ufres->put(target.sub("Format"), str);
                if (str == EXTFORMAT_name[EXT_OUT_HDF5]) {
                  SW_EXTFORMAT = EXT_OUT_HDF5;
                } else {
                  fprintf(stderr, "#%s : Unrecognized exteded format\n", str.c_str());
                  exit_job(EXIT_FAILURE);
                }
              }
              target.up();

              target.down("Print_field");
              {  // Print flags for field data
                ufin->get(target.sub("Crop"), str);
                ufout->put(target.sub("Crop"), str);
                ufres->put(target.sub("Crop"), str);
                if (str == "YES") {
                  target.down("YES");

                  const char *slab_name[DIM] = {"Slab_x", "Slab_y", "Slab_z"};
                  for (int d = 0; d < DIM; d++) {
                    target.down(slab_name[d]);

                    ufin->get(target.sub("start"), print_field_crop.start[d]);
                    ufin->get(target.sub("count"), print_field_crop.count[d]);
                    ufin->get(target.sub("stride"), print_field_crop.stride[d]);

                    ufout->put(target.sub("start"), print_field_crop.start[d]);
                    ufout->put(target.sub("count"), print_field_crop.count[d]);
                    ufout->put(target.sub("stride"), print_field_crop.stride[d]);

                    ufres->put(target.sub("start"), print_field_crop.start[d]);
                    ufres->put(target.sub("count"), print_field_crop.count[d]);
                    ufres->put(target.sub("stride"), print_field_crop.stride[d]);

                    target.up();
                  }

                  for (int d = 0; d < DIM; d++) {
                    if (print_field_crop.start[d] < 0 || print_field_crop.start[d] >= Ns[d]) {
                      fprintf(stderr, "# Error: field output start value for %d-dim out of bounds\n", d);
                      exit_job(EXIT_FAILURE);
                    }
                    if (print_field_crop.stride[d] <= 0 || print_field_crop.stride[d] >= Ns[d]) {
                      fprintf(stderr, "# Error: field output stride value for %d-dim out of bounds\n", d);
                      exit_job(EXIT_FAILURE);
                    }
                    if (print_field_crop.count[d] <= 0 || print_field_crop.count[d] > Ns[d]) {
                      fprintf(stderr, "# Error: field output count value for %d-dim out of bounds\n", d);
                      exit_job(EXIT_FAILURE);
                    }

                    int dmy_crop_end =
                        print_field_crop.start[d] + (print_field_crop.count[d] - 1) * print_field_crop.stride[d];
                    if (dmy_crop_end < 0 || dmy_crop_end >= Ns[d] || dmy_crop_end < print_field_crop.start[d]) {
                      fprintf(stderr, "# Error: invalid field output range for %d-dim\n", d);
                      exit_job(EXIT_FAILURE);
                    }
                  }

                  target.up();  // Crop YES
                } else {        // Crop NO
                  for (int d = 0; d < DIM; d++) {
                    print_field_crop.start[d]  = 0;
                    print_field_crop.count[d]  = Ns[d];
                    print_field_crop.stride[d] = 1;
                  }
                }

                ufin->get(target.sub("Vel"), str);
                ufout->put(target.sub("Vel"), str);
                ufres->put(target.sub("Vel"), str);
                print_field.vel = (str == "YES" ? true : false);

                ufin->get(target.sub("Phi"), str);
                ufout->put(target.sub("Phi"), str);
                ufres->put(target.sub("Phi"), str);
                print_field.phi = (str == "YES" ? true : false);

                ufin->get(target.sub("Charge"), str);
                ufout->put(target.sub("Charge"), str);
                ufres->put(target.sub("Charge"), str);
                print_field.charge = (str == "YES" ? true : false);

                ufin->get(target.sub("Pressure"), str);
                ufout->put(target.sub("Pressure"), str);
                ufres->put(target.sub("Pressure"), str);
                print_field.pressure = (str == "YES" ? true : false);

                ufin->get(target.sub("Tau"), str);
                ufout->put(target.sub("Tau"), str);
                ufres->put(target.sub("Tau"), str);
                print_field.tau = (str == "YES" ? true : false);
              }
              target.up();
            }
            target.up();
          } else {
            fprintf(stderr, "invalid FileType %s\n", str.c_str());
            exit_job(EXIT_FAILURE);
          }
        }
        target.up();
      } else if (str == "OFF") {
        SW_OUTFORMAT = OUT_NONE;
      } else {
        fprintf(stderr, "invalid switch for AVS\n");
        exit_job(EXIT_FAILURE);
      }
    }
    {  ////// UDF
      ufin->get(target.sub("UDF"), str);
      ufout->put(target.sub("UDF"), str);
      ufres->put(target.sub("UDF"), str);
      if (str == "ON") {
        SW_UDF = 1;
      } else if (str == "OFF") {
        SW_UDF = 0;
      } else {
        fprintf(stderr, "invalid switch for UDF\n");
        exit_job(EXIT_FAILURE);
      }
    }
  }

  if ((!RESUMED) && (DISTRIBUTION != user_specify)) {
    delete ufin;
  }

  Set_global_parameters();

  if (SW_PT == rigid) {
    // allocation (using Particle_Number)
    GRvecs      = alloc_2d_double(Particle_Number, DIM);
    GRvecs_body = alloc_2d_double(Particle_Number, DIM);

    // set Particle_RigidID
    int rigid_n1     = 0;
    int rigid_n2     = 0;
    int rigidID      = 0;
    Particle_RigidID = alloc_1d_int(Particle_Number);
    for (int rigid_i = 0; rigid_i < Component_Number; rigid_i++) {
      for (int rigid_j = 0; rigid_j < Chain_Numbers[rigid_i]; rigid_j++) {
        rigid_n2 += Beads_Numbers[rigid_i];
        for (int n = rigid_n1; n < rigid_n2; n++) {
          Particle_RigidID[n] = rigidID;
        }
        rigidID += 1;
        rigid_n1 = rigid_n2;
      }
    }
    // set RigidID_Components
    rigidID = 0;
    for (int rigid_i = 0; rigid_i < Component_Number; rigid_i++) {
      for (int rigid_j = 0; rigid_j < Chain_Numbers[rigid_i]; rigid_j++) {
        RigidID_Components[rigidID] = rigid_i;
        rigidID += 1;
      }
    }

    // initialize Rigid_Particle_Numbers[]
    for (int rigidID = 0; rigidID < Rigid_Number; rigidID++) {
      Rigid_Particle_Numbers[rigidID] = 0;
    }

    // set Rigid_Particle_Numbers[]
    for (int n = 0; n < Particle_Number; n++) {
      Rigid_Particle_Numbers[Particle_RigidID[n]] += 1;
    }
    // set Rigid_Particle_Cumul[]
    // loop over beads of rigid I: cumul[I] <= i < cumul[I+1]
    Rigid_Particle_Cumul[0] = 0;
    for (int rigidID = 0; rigidID < Rigid_Number; rigidID++) {
      Rigid_Particle_Cumul[rigidID + 1] = Rigid_Particle_Cumul[rigidID] + Rigid_Particle_Numbers[rigidID];
    }
    if (rigid_n1 != Particle_Number || Rigid_Particle_Cumul[Rigid_Number] != Particle_Number) {  // for debug
      fprintf(stderr, "error: set Particle_RigidID\n");
      exit_job(EXIT_FAILURE);
    }
    for (int rigidID = 0; rigidID < Rigid_Number; rigidID++) {
      for (int n = Rigid_Particle_Cumul[rigidID]; n < Rigid_Particle_Cumul[rigidID + 1]; n++) {
        if (Particle_RigidID[n] != rigidID) {
          fprintf(stderr, "error: set Particle_Cumul\n");
          exit_job(EXIT_FAILURE);
        }
      }
    }

    // debug output
    /*
    for(int n=0; n<Particle_Number; n++){
            fprintf(stderr, "# debug: Particle_RigidID[%d] = %d\n", n, Particle_RigidID[n]);
    }
    for(int rigidID=0; rigidID<Rigid_Number; rigidID++){
            fprintf(stderr, "# debug: RigidID_Components[%d] = %d\n", rigidID, RigidID_Components[rigidID]);
    }
    for(int rigidID=0; rigidID<Rigid_Number; rigidID++){
            fprintf(stderr, "# debug: Rigid_Particle_Numbers[%d] = %d (%d -> %d)\n",
                    rigidID, Rigid_Particle_Numbers[rigidID],
                    Rigid_Particle_Cumul[rigidID], Rigid_Particle_Cumul[rigidID+1] - 1);
    }
    */

    // initialize velocityGs and omegaGs and
    int rigid_component;
    for (int rigidID = 0; rigidID < Rigid_Number; rigidID++) {
      rigid_component = RigidID_Components[rigidID];
      for (int d = 0; d < DIM; d++) {
        velocityGs[rigidID][d]     = Rigid_Velocities[rigid_component][d];
        omegaGs[rigidID][d]        = Rigid_Omegas[rigid_component][d];
        velocityGs_old[rigidID][d] = velocityGs[rigidID][d];
        omegaGs_old[rigidID][d]    = omegaGs[rigidID][d];
      }
    }
  }
}

char *In_udf, *Sum_udf, *Out_udf, *Def_udf, *Ctrl_udf, *Res_udf;

// GOURMET上で与えられたファイル名を取得します

void file_get(const int argc, char *argv[]) {
  const int Number_of_reuired_arguments = 5;

  if (argc < Number_of_reuired_arguments) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "> %s -I[input UDF] -O[output UDF] -D[define UDF] -R[restart UDF]\n", argv[0]);
    fprintf(stderr, "\n");
    exit_job(EXIT_FAILURE);
  }
  int R_selected = 0;
  In_udf = Sum_udf = Out_udf = Def_udf = Ctrl_udf = Def_udf = Res_udf = NULL;
  for (int i = 1; i < argc; i++) {
    char  c = ' ';
    char *p = argv[i];
    if (*p == '-' && *++p) c = *p++;
    switch (c) {
      case 'I':  //インプットUDF
        In_udf = p;
        fprintf(stderr, "#using %s as input\n", p);
        break;
      case 'S':  //計算途中経過出力UDF
        Sum_udf = p;
        fprintf(stderr, "#using %s as summary\n", p);
        break;
      case 'O':  //アウトプットUDF
        Out_udf = p;
        fprintf(stderr, "#using %s as output\n", p);
        break;
      case 'D':  //定義UDF
        Def_udf = p;
        fprintf(stderr, "#using %s as definition\n", p);
        break;
      case 'M':  //制御用ファイル
        Ctrl_udf = p;
        fprintf(stderr, "#using %s as control\n", p);
        break;
      case 'R':  // リスタートUDF
        Res_udf = p;
        fprintf(stderr, "#using %s as restart\n", p);
        R_selected = 1;
        break;
      default:
        break;
    }
  }
  if ((In_udf == NULL) || (Out_udf == NULL) || (Def_udf == NULL) || (Res_udf == NULL)) {
    fprintf(stderr, "Program stopped because required udf file(s) is not given.\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "> %s -I[input UDF] -O[output UDF] -D[define UDF] -R[restart UDF]\n", argv[0]);
    fprintf(stderr, "\n");
    exit_job(EXIT_FAILURE);
  }

  if (0) {  // input.udf を restart.udf にコピーしとく
            // if(R_selected){ // input.udf を restart.udf にコピーしとく
    FILE *fin, *fout;
    char  s[256];
    if ((fin = fopen(In_udf, "r")) == NULL) {
      printf("cannot open file\n");
      exit(0);
    }
    if ((fout = fopen(Res_udf, "w")) == NULL) {
      printf("cannot open file\n");
      exit(0);
    }
    while (fgets(s, 256, fin) != NULL) {
      fputs(s, fout);
    }
    fclose(fin);
    fclose(fout);
  }
}
