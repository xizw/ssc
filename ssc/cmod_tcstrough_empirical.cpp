#include "core.h"
#include "tckernel.h"

static var_info _cm_vtab_tcstrough_empirical[] = {
/*   VARTYPE            DATATYPE          NAME                 LABEL                                                            UNITS           META            GROUP            REQUIRED_IF                 CONSTRAINTS             UI_HINTS  */
    { SSC_INPUT,        SSC_STRING,      "file_name",         "local weather file path",                                        "",             "",            "Weather",        "*",                       "LOCAL_FILE",            "" },
    { SSC_INPUT,        SSC_NUMBER,      "track_mode",        "Tracking mode",                                                  "",             "",            "Weather",        "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "tilt",              "Tilt angle of surface/axis",                                     "",             "",            "Weather",        "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "azimuth",           "Azimuth angle of surface/axis",                                  "",             "",            "Weather",        "*",                       "",                      "" }, 

    // general
    { SSC_INPUT,        SSC_NUMBER,      "TOUPeriod",         "Time of Use Period",                                             "",             "",            "other",          "*",                       "INTEGER",               "" }, 

    // solar field
    { SSC_INPUT,        SSC_NUMBER,      "Site_Lat",          "Latitude of Solar Plant Site",                                   "deg",          "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "Site_LongD",        "Longitude of Solar Plant Site",                                  "deg",          "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SHIFT",             "Longitude of Standard Meridian",                                 "deg",          "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "LU_Fl",             "Fluid property file logical unit",                               "",             "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "LuFlEr",            "Fluid property error file logical unit",                         "",             "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "i_SfTi",            "Solar Field HTF inlet Temperature (if -999, calculated)",        "C",            "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "ColType",           "Collector Type",                                                 "",             "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SfPipeHl300",       "Solar field piping heat loss at design",                         "W/m2",         "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SfPipeHl1",         "Solar field piping heat loss at reduced temp. - linear term",    "C^(-1)",       "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SfPipeHl2",         "Solar field piping heat loss at reduced temp. - quadratic term", "C^(-2)",       "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SfPipeHl3",         "Solar field piping heat loss at reduced temp. - cubic term",     "C^(-3)",       "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "Stow_Angle",        "Night-Time Trough Stow Angle",                                   "deg",          "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "DepAngle",          "Deployment Angle",                                               "deg",          "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "Distance_SCA",      "Distance between SCAs in Row",                                   "m",            "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "Row_Distance",      "Distance between Rows of SCAs",                                  "m",            "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "NumScas",           "Number of SCAs per Row",                                         "",             "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "Solar_Field_Area",  "Solar Field Area",                                               "m2",           "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "Solar_Field_Mult",  "Solar Field Multiple",                                           "",             "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SfInTempD",         "Solar Field Design Inlet Temperature",                           "C",            "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SfOutTempD",        "Solar Field Design Outlet Temperature",                          "C",            "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "MinHtfTemp",        "Minimum Heat Transfer Fluid Temperature",                        "C",            "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HtfGalArea",        "HTF Fluids in Gallons per Field Area",                           "gal/m2",       "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "ColTilt",           "Collector Axis Tilt",                                            "deg",          "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "ColAz",             "Azimuthal Angle of Collector Axis",                              "deg",          "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SFTempInit",        "Solar Field Initial Temperature",                                "C",            "",            "solarfield",     "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HTFFluid",          "Type of Heat Transfer Fluid used",                               "",             "",            "solarfield",     "*",                       "INTEGER",               "" }, 

    // SCA
    { SSC_INPUT,        SSC_NUMBER,      "IamF0",             "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "IamF1",             "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "IamF2",             "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "Ave_Focal_Length",  "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "ScaLen",            "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SCA_aper",          "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SfAvail",           "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TrkTwstErr",        "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "GeoAcc",            "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "MirRef",            "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "MirCln",            "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "ConcFac",           "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" }, 

    // HCE
    { SSC_INPUT,        SSC_NUMBER,      "NumHCETypes",       "Number of HCE types",                                            "",             "",            "hce",            "*",                       "INTEGER",               "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCEtype",           "Number indicating the receiver type",                            "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCEFrac",           "Fraction of field that is this type of HCE",                     "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCEdust",           "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCEBelShad",        "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCEEnvTrans",       "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCEabs",            "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCEmisc",           "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "PerfFac",           "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "RefMirrAper",       "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCE_A0",            "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCE_A1",            "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCE_A2",            "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCE_A3",            "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCE_A4",            "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCE_A5",            "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "HCE_A6",            "label",                                                          "",             "",            "hce",            "*",                       "",                      "" }, 

    // powerblock
    { SSC_INPUT,        SSC_NUMBER,      "TurbOutG",          "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TurbEffG",          "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "PTTMAX",            "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "PTTMIN",            "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "MaxGrOut",          "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "MinGrOut",          "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TurSUE",            "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "T2EPLF0",           "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "T2EPLF1",           "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "T2EPLF2",           "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "T2EPLF3",           "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "T2EPLF4",           "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "E2TPLF0",           "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "E2TPLF1",           "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "E2TPLF2",           "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "E2TPLF3",           "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "E2TPLF4",           "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TempCorrF",         "Temp Correction Mode (1=wetbulb 2=drybulb basis)",               "",             "",            "pwrb",           "*",                       "INTEGER",               "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TempCorr0",         "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TempCorr1",         "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TempCorr2",         "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TempCorr3",         "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TempCorr4",         "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "LHVBoilEff",        "Label",                                                          "",             "",            "pwrb",           "*",                       "",                      "" }, 
                                                          
    // thermal energy storage                             
    { SSC_INPUT,        SSC_NUMBER,      "TurTesEffAdj",      "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TurTesOutAdj",      "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TnkHL",             "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "PTSmax",            "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "PFSmax",            "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "TSHOURS",           "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "NUMTOU",            "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_MATRIX,      "TSLogic",           "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_ARRAY,       "FossilFill",        "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" }, 
  //{ SSC_INPUT,        SSC_NUMBER,      "TimeOfDay",         "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" }, 
                                                            
    // parasitics
    { SSC_INPUT,        SSC_NUMBER,      "SfPar",             "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "SfParPF",           "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "ChtfPar",           "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "ChtfParPF",         "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "CHTFParF0",         "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "CHTFParF1",         "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "CHTFParF2",         "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "AntiFrPar",         "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "BOPPar",            "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "BOPParPF",          "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "BOPParF0",          "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "BOPParF1",          "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "BOPParF2",          "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "CtOpF",             "Label",                                                          "",             "",            "parasitic",      "*",                       "INTEGER",               "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "CtPar",             "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "CtParPF",           "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "CtParF0",           "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "CtParF1",           "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "CtParF2",           "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HtrPar",            "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HtrParPF",          "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HtrParF0",          "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HtrParF1",          "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HtrParF2",          "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HhtfPar",           "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HhtfParPF",         "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HhtfParF0",         "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HhtfParF1",         "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "HhtfParF2",         "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_NUMBER,      "PbFixPar",          "Label",                                                          "",             "",            "parasitic",      "*",                       "",                      "" }, 

    // OUTPUTS
	// The names of the output variables should match the parameter names for the TCS units in order to signal to the TCS kernel to store the values by timestep

	// VARTYPE          DATATYPE          NAME                 LABEL                                                            UNITS           META            GROUP            REQUIRED_IF                 CONSTRAINTS             UI_HINTS
	// weather file reader
    { SSC_OUTPUT,       SSC_ARRAY,       "month",             "Month",                                                          "",             "",            "weather",        "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "solazi",            "Solar Azimuth",                                                  "deg",          "",            "weather",        "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "solzen",            "Solar Zenith",                                                   "deg",          "",            "weather",        "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "beam",              "Beam normal irradiance",                                         "W/m2",         "",            "weather",        "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "tdry",              "Dry bulb temperature",                                           "C",            "",            "weather",        "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "wspd",              "Wind Speed",                                                     "m/s",          "",            "weather",        "*",                       "LENGTH=8760",           "" },

	// type 805 - solar field
    { SSC_OUTPUT,       SSC_ARRAY,       "Theta",             "Angle between aperture plane normal & incident radiation",       "deg",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "CosTheta",          "Multiplying term that scales incident rad due to angular loss",  "",             "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "TrackAngle",        "Collector tracking angle",                                       "deg",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "IAM",               "Incidence Angle Modifier (average over the timestep)",           "",             "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "RowShadow",         "Fraction of energy lost from row-to-row shadowing",              "",             "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "EndLoss",           "End loss effect, hourly",                                        "",             "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "QnipCosTh",         "Incident radiation scaled by cosine loss: effective radiation",  "W/m2",         "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Qsfnipcosth",       "Total inc. radiation scaled by the cosine loss",                 "MWt",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "ColOptEff",         "Collector Optical Efficiency",                                   "",             "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Qdni",              "Total incident irradiation on the field before any losses",      "MWt",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "QsfAbs",            "Total energy absorbed by the solar field before thermal losses", "MWt",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Qabs",              "Energy absorbed by solar field before th. loss, per unit area",  "W/m2",         "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Qsf",               "Thermal energy available from the solar field",                  "MWt",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Qcol",              "Energy delivered by the solar field, per unit area",             "W/m2",         "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "RecHl",             "Total receiver heat loss",                                       "kJ/hr-m2",     "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "QsfHceHL",          "Total energy lost by the receivers",                             "MWt",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "QsfPipeHL",         "Total energy lost by the field piping",                          "MWt",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "o_SfTi",            "Solar Field HTF inlet Temp (if -999, calculated)",               "C",            "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "SfTo",              "Final Outlet temperature of the solar field",                    "C",            "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "SfMassFlow",        "Solar field mass flow rate",                                     "kg/hr",        "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "AveSfTemp",         "Average Solar Field Temperature during the timestep",            "C",            "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "QsfWarmup",         "Power required or has contributed to warming the solar field",   "MWt",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "QhtfFreezeProt",    "Total energy contributing to solar field freeze protection",     "MWt",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },

	// type 806 - thermal energy storage
	{ SSC_OUTPUT,       SSC_ARRAY,       "QhtfFpTES",         "Thermal energy storage freeze protection energy",                "MWt",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "QhtfFpHtr",         "Freeze protection provided by auxiliary heater",                 "MWt",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Qtts",              "Heat to Thermal Storage",                                        "MWt",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Qfts",              "Heat from Thermal Storage",                                      "MWt",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Ets",               "Energy in Thermal Storage",                                      "MWt.hr",       "",            "type_806",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "QTsHl",             "Energy losses from Thermal Storage",                             "MWt",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "QTsFull",           "Energy dumped because the thermal storage is full",              "MWt",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Qtpb",              "Heat to Power Block (output from TS/Dispatch type)",             "MWt",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "QTurSu",            "The energy needed to startup the turbine",                       "MWt",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "Qmin",              "Energy dumped due to minimum load requirement",                  "MWt",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Qdump",             "Amount of energy dumped (more than turbine & storage)",          "MWt",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },

	// type 807 - power plant
	{ SSC_OUTPUT,       SSC_ARRAY,       "EgrSol",            "Gross electric production from the solar resource",              "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EgrFos",            "Gross electric production from the fossil resource",             "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "Qgas",              "Gas Thermal Energy Input",                                       "MW",           "",            "type_807",       "*",                       "LENGTH=8760",           "" },
    { SSC_OUTPUT,       SSC_ARRAY,       "Egr",               "Gross electricity produced, before parasitic loss",              "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "Enet",              "Net electricity produced, after parasitic loss",                 "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },

	// parasitics
	{ SSC_OUTPUT,       SSC_ARRAY,       "Epar",              "Total Parasitics for entire system",                             "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EparSf",            "Parasitics associated with solar field tracking and drives",     "MWe",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EparCHTF",          "Cold HTF Pump Parasitics (HTF flow to Solar Field)",             "MWe",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EparHhtf",          "Hot HTF pump parasitics",                                        "MWe",          "",            "type_806",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EparAnti",          "Antifreeze pumping parasitics",                                  "MWe",          "",            "type_805",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EparHtr",           "Auxiliary heater parasitic load",                                "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EparPB",            "Fixed Power Block Parasitics",                                   "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EparBOP",           "Balance of Plant Parasitics",                                    "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EparCT",            "Cooling Tower Parasitic Load",                                   "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EparOffLine",       "Parasitics incurred while plant is not producing electricity",   "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "EparOnLine",        "Parasitics incurred while plant is producing electricity",       "MWe",          "",            "type_807",       "*",                       "LENGTH=8760",           "" },

	// other outputs
	{ SSC_OUTPUT,       SSC_ARRAY,       "Ftrack",            "Fraction of time period that the field is tracking",             "",             "",            "other",          "*",                       "LENGTH=8760",           "" },
  //{ SSC_OUTPUT,       SSC_ARRAY,       "TSLogic",           "Dispatch logic w/o solar(1),with solar(2),turbine load(3)",      "",             "",            "other",          "*",                       "LENGTH=8760",           "" },

    var_info_invalid };
class cm_tcstrough_empirical : public tcKernel
{
public:
	
	cm_tcstrough_empirical(tcstypeprovider *prov)
	:tcKernel(prov)
	{
		add_var_info( _cm_vtab_tcstrough_empirical );
		//set_store_all_parameters(true); // default is 'false' = only store TCS parameters that match the SSC_OUTPUT variables above
	}

	void exec( ) throw( general_error )
	{
		bool debug_mode = false;
		//Add weather file reader unit
		int weather = 0;
		if(debug_mode) weather = add_unit("trnsys_weatherreader", "TRNSYS weather reader");
		else weather = add_unit("weatherreader", "TCS weather reader");
		//Add Empirical Solar Field Model
		int	u1 = add_unit( "sam_trough_model_type805", "Test Trough" );
		//Add Empirical Storage Model
		int u2 = add_unit( "sam_trough_storage_type806", "Test Storage" );
		//Add Empirical Power Block Model
		int u3 = add_unit( "sam_trough_plant_type807", "Test Plant" );

		if(debug_mode)
		{
			set_unit_value( weather, "file_name", "C:/svn_NREL/main/ssc/tcs/typelib/TRNSYS_weather_outputs/tucson_trnsys_weather.out" );
			set_unit_value( weather, "i_hour", "TIME" );
			set_unit_value( weather, "i_month", "month" );
			set_unit_value( weather, "i_day", "day" );
			set_unit_value( weather, "i_global", "GlobalHorizontal" );
			set_unit_value( weather, "i_beam", "DNI" );
			set_unit_value( weather, "i_diff", "DiffuseHorizontal" );
			set_unit_value( weather, "i_tdry", "T_dry" );
			set_unit_value( weather, "i_twet", "T_wet" );
			set_unit_value( weather, "i_tdew", "T_dew" );
			set_unit_value( weather, "i_wspd", "WindSpeed" );
			set_unit_value( weather, "i_wdir", "WindDir" );
			set_unit_value( weather, "i_rhum", "RelHum" );
			set_unit_value( weather, "i_pres", "AtmPres" );
			set_unit_value( weather, "i_snow", "SnowCover" );
			set_unit_value( weather, "i_albedo", "GroundAlbedo" );
			set_unit_value( weather, "i_poa", "POA" );
			set_unit_value( weather, "i_solazi", "Azimuth" );
			set_unit_value( weather, "i_solzen", "Zenith" );
			set_unit_value( weather, "i_lat", "Latitude" );
			set_unit_value( weather, "i_lon", "Longitude" );
			set_unit_value( weather, "i_shift", "Shift" );
		}
		else
		{
			//Set weatherreader parameters
			set_unit_value_ssc_string( weather, "file_name" );
			set_unit_value_ssc_double( weather, "track_mode" );    //, 0 ); SET TO 3 IN TRNSYS FILE, no user input !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			set_unit_value_ssc_double( weather, "tilt" );          //, 0 );
			set_unit_value_ssc_double( weather, "azimuth" );       //, 0 );
		}

		//Set Solar Field Parameters
		set_unit_value_ssc_double( u1, "Site_Lat" );           //, 32.116667 );
		set_unit_value_ssc_double( u1, "Site_LongD" );         //, -110.933333 );
		set_unit_value_ssc_double( u1, "SHIFT" );              //, -7 );
		
		set_unit_value_ssc_double( u1, "Solar_Field_Area" );   //, 877580 ); csp.tr.solf.dp.fieldarea
		set_unit_value_ssc_double( u1, "Solar_Field_Mult" );   //, 2 ); csp.tr.solf.dp.solarmultiple
		set_unit_value_ssc_double( u1, "HTFFluid" );           //, 21 ); TranslateHTFType( IVal("csp.tr.solf.fieldhtftype")
		set_unit_value_ssc_double( u1, "NumHCETypes" );        //, 4 );
        set_unit_value_ssc_array( u1, "HCEtype" );      //                      {1,1,1,1},
        set_unit_value_ssc_array( u1, "HCEFrac" );      //                      {0.985,0.01,0.005,0},
        set_unit_value_ssc_array( u1, "HCEdust" );      //, t[2], 4 );       // {0.98,0.98,0.98,0.98},
        set_unit_value_ssc_array( u1, "HCEBelShad" );   //, t[3], 4  );      // {0.963,0.963,0.963,0.963},
        set_unit_value_ssc_array( u1, "HCEEnvTrans" );  //, t[4], 4  );      // {0.963,0.963,1,0.963},
        set_unit_value_ssc_array( u1, "HCEabs" );       //, t[5], 4  );      // {0.96,0.96,0.8,0.96},  
        set_unit_value_ssc_array( u1, "HCEmisc" );      //, t[6], 4  );      // {1,1,1,1},   
        set_unit_value_ssc_array( u1, "PerfFac" );      //, t[7], 4 );       // {1,1,1,1},                
        set_unit_value_ssc_array( u1, "RefMirrAper" );  //, t[8], 4  );      // {5,5,5,5},   
        set_unit_value_ssc_array( u1, "HCE_A0" );       //, t[9], 4  );      // {4.05,      50.8,      -9.95,     11.8},  
        set_unit_value_ssc_array( u1, "HCE_A1" );       //, t[10], 4  );     // {0.247,     0.904,      0.465,    1.35},   
        set_unit_value_ssc_array( u1, "HCE_A2" );       //, t[11], 4 );      // {-0.00146,  0.000579,  -0.000854, 0.0075} ,  
        set_unit_value_ssc_array( u1, "HCE_A3" );       //, t[12], 4 );      // {5.65e-6,   1.13e-5,    1.85e-5,  4.07e-6},  
        set_unit_value_ssc_array( u1, "HCE_A4" );       //, t[13], 4 );      // {7.62e-8,   1.73e-7,    6.89e-7,  5.85e-8} ,  
        set_unit_value_ssc_array( u1, "HCE_A5" );       //, t[14], 4 );      // {-1.7,     -43.2,       24.7,     4.48},  
        set_unit_value_ssc_array( u1, "HCE_A6" );       //, t[15], 4 );      // {0.0125,    0.524,      3.37,     0.285}      
		set_unit_value_ssc_double( u1, "LU_Fl" );       //,             21.0 );   // necessary?      
		set_unit_value_ssc_double( u1, "LuFlEr" );       //,            0.0 );    // necessary?
		set_unit_value_ssc_double( u1, "i_SfTi" );       //,           -999 );           
		set_unit_value_ssc_double( u1, "Stow_Angle" );       //, 	    170);     // csp.tr.solf.stowangle
		set_unit_value_ssc_double( u1, "DepAngle" );       //, 	        10);      // csp.tr.solf.deployangle
		set_unit_value_ssc_double( u1, "IamF0" );       //, 	        1);       // csp.tr.sca.iamc1
		set_unit_value_ssc_double( u1, "IamF1" );       //, 	        0.0506);  // csp.tr.sca.iamc2
		set_unit_value_ssc_double( u1, "IamF2" );       //, 	        -0.1763); // csp.tr.sca.iamc3
		set_unit_value_ssc_double( u1, "Ave_Focal_Length" );       //,  1.8);     // csp.tr.sca.avg_focal_length
		set_unit_value_ssc_double( u1, "Distance_SCA" );       //, 	    1);       // csp.tr.solf.distscas
		set_unit_value_ssc_double( u1, "Row_Distance" );       //, 	    15);      // csp.tr.solf.distrows
		set_unit_value_ssc_double( u1, "SCA_aper" );       //, 	        5);       // csp.tr.sca.aperture
		set_unit_value_ssc_double( u1, "SfAvail" );       //, 	        0.99);    // csp.tr.sca.availability 
		set_unit_value_ssc_double( u1, "ColTilt" );       //, 	        0.0);     // csp.tr.solf.tilt
		set_unit_value_ssc_double( u1, "ColAz" );       //, 	        0.0);     // csp.tr.solf.azimuth
		set_unit_value_ssc_double( u1, "NumScas" );       //, 	        4);       // csp.tr.solf.nscasperloop
		set_unit_value_ssc_double( u1, "ScaLen" );       //, 	        100);     // csp.tr.sca.length
		set_unit_value_ssc_double( u1, "MinHtfTemp" );       //, 	    50);      // csp.tr.solf.htfmintemp
		set_unit_value_ssc_double( u1, "HtfGalArea" );       //, 	    0.614);   // csp.tr.solf.htfgallonsperarea
		set_unit_value_ssc_double( u1, "SfPar" );       //, 	        0.233436);// csp.tr.par.sf.total
		set_unit_value_ssc_double( u1, "SfParPF" );       //, 	        1);       // csp.tr.par.sf.partload
		set_unit_value_ssc_double( u1, "ChtfPar" );       //, 	        9.23214); // csp.tr.par.htfpump.total
		set_unit_value_ssc_double( u1, "ChtfParPF" );       //, 	    1);       // csp.tr.par.htfpump.partload
		set_unit_value_ssc_double( u1, "CHTFParF0" );       //, 	    -0.036);  // csp.tr.par.htfpump.f0
		set_unit_value_ssc_double( u1, "CHTFParF1" );       //, 	    0.242);   // csp.tr.par.htfpump.f1
		set_unit_value_ssc_double( u1, "CHTFParF2" );       //, 	    0.794);   // csp.tr.par.htfpump.f2
		set_unit_value_ssc_double( u1, "AntiFrPar" );       //, 	    0.923214);// csp.tr.par.antifreeze.total
		set_unit_value_ssc_double( u1, "TurbOutG" );       //, 	       111);      // csp.tr.pwrb.design_gross_output
		set_unit_value_ssc_double( u1, "TurbEffG" );       //, 	       0.3774);   // csp.tr.pwrb.effdesign
		set_unit_value_ssc_double( u1, "SfInTempD" );       //, 	   293);      // csp.tr.solf.htfinlettemp
		set_unit_value_ssc_double( u1, "SfOutTempD" );       //, 	   391);      // csp.tr.solf.htfoutlettemp
		set_unit_value_ssc_double( u1, "ColType" );       //, 	       1);
		set_unit_value_ssc_double( u1, "TrkTwstErr" );       //, 	   0.994);     // csp.tr.sca.track_twist_error
		set_unit_value_ssc_double( u1, "GeoAcc" );       //, 	       0.98);      // csp.tr.sca.geometric_accuracy
		set_unit_value_ssc_double( u1, "MirRef" );       //, 	       0.935);     // csp.tr.sca.reflectivity
		set_unit_value_ssc_double( u1, "MirCln" );       //, 	       0.95);      // csp.tr.sca.cleanliness
		set_unit_value_ssc_double( u1, "ConcFac" );       //, 	       1);         // csp.tr.sca.concentrator_factor
		set_unit_value_ssc_double( u1, "SfPipeHl300" );       //, 	   10);        // csp.tr.solf.pipingheatlossatdesign
		set_unit_value_ssc_double( u1, "SfPipeHl1" );       //, 	   0.001693);  // csp.tr.solf.pipingheatlosscoeff1
		set_unit_value_ssc_double( u1, "SfPipeHl2" );       //, 	   -1.683e-5); // csp.tr.solf.pipingheatlosscoeff2
		set_unit_value_ssc_double( u1, "SfPipeHl3" );       //, 	   6.78e-8);   // csp.tr.solf.pipingheatlosscoeff3
		set_unit_value_ssc_double( u1, "SFTempInit" );       //,       100);	   // csp.tr.solf.htfinittemp

		//Connect Solar Field Inputs
		bool bConnected = connect( weather, "solazi", u1, "SolarAz", 0.1, -1 );                
		bConnected &= connect( weather, "beam", u1, "Insol_Beam_Normal", 0.1, -1 );
		bConnected &= connect( weather, "tdry", u1, "AmbientTemperature", 0.1, -1 );
		bConnected &= connect( weather, "wspd", u1, "WndSpd", 0.1, -1 );
	
		//Set Storage Parameters
		set_unit_value_ssc_double(u2, "TSHOURS" );       //,6);          // csp.tr.tes.full_load_hours
		set_unit_value_ssc_double(u2, "NUMTOU" );       //, 9);
		set_unit_value_ssc_double(u2, "E2TPLF0" );       //, 0.03737);   // csp.tr.pwrb.tpl_tff0
		set_unit_value_ssc_double(u2, "E2TPLF1" );       //, 0.98823);   // csp.tr.pwrb.tpl_tff1
		set_unit_value_ssc_double(u2, "E2TPLF2" );       //, -0.064991); // csp.tr.pwrb.tpl_tff2
		set_unit_value_ssc_double(u2, "E2TPLF3" );       //, 0.039388);  // csp.tr.pwrb.tpl_tff3
		set_unit_value_ssc_double(u2, "E2TPLF4" );       //, 0.0);       // csp.tr.pwrb.tpl_tff4

		//double  t2[9][3] =  {{0.1,0.1,1.05},{0.1,0.1,1},{0.1,0.1,1},{0.1,0.1,1},{0.1,0.1,1},{0.1,0.1,1},{0.1,0.1,1},{0.1,0.1,1},{0.1,0.1,1}};
		//set_unit_value(u2, "TSLogic", &t2[0][0], 9, 3);  
		set_unit_value_ssc_matrix(u2, "TSLogic" ); // csp.tr.tes.dispX.solar, csp.tr.tes.dispX.nosolar, csp.tr.tes.dispX.turbout,  where X = 1 to 9

		set_unit_value_ssc_double(u2, "TOUPeriod" );       //, 1 );    
		set_unit_value_ssc_double(u2, "TnkHL" );        //, 	 0.97);            // csp.tr.tes.tank_heatloss
		set_unit_value_ssc_double(u2, "PTSmax" );       //,  294.118);	       // csp.tr.tes.max_to_power
		set_unit_value_ssc_double(u2, "PFSmax" );       //,  297.999);	       // csp.tr.tes.max_from_power
		set_unit_value_ssc_double(u2, "PTTMAX" );       //,  1.05);            // csp.tr.pwrb.maxoutput
		set_unit_value_ssc_double(u2, "PTTMIN" );       //,  0.25);	           // csp.tr.pwrb.minoutput
		set_unit_value_ssc_double(u2, "TurSUE" );       //,  0.2);	           // csp.tr.pwrb.startup_energy
		set_unit_value_ssc_double(u2, "HhtfPar" );       //, 	 2.22);        // csp.tr.par.tes.total
		set_unit_value_ssc_double(u2, "HhtfParPF" );       //,   1);           // csp.tr.par.tes.partload
		set_unit_value_ssc_double(u2, "HhtfParF0" );       //,   -0.036);      // csp.tr.par.tes.f0
		set_unit_value_ssc_double(u2, "HhtfParF1" );       //,    0.242);      // csp.tr.par.tes.f1
		set_unit_value_ssc_double(u2, "HhtfParF2" );       //,    0.794);      // csp.tr.par.tes.f2

		//Connect Storage Inputs
		bConnected &= connect( u1, "Qsf", u2, "Qsf", 0.1, -1);
		bConnected &= connect( u1, "Qdesign", u2, "Qdesign", 0.1, -1);
		bConnected &= connect( u1, "QhtfFreezeProt", u2, "QhtfFreezeProt", 0.1, -1);  	
	
		//Set Powerblock Parameters
		set_unit_value_ssc_double(u3,"T2EPLF0" );       //, -0.037726);	    // csp.tr.pwrb.tpl_tef0
		set_unit_value_ssc_double(u3,"T2EPLF1" );       //, 1.0062);	    // csp.tr.pwrb.tpl_tef1
		set_unit_value_ssc_double(u3,"T2EPLF2" );       //, 0.076316);      // csp.tr.pwrb.tpl_tef2
		set_unit_value_ssc_double(u3,"T2EPLF3" );       //, -0.044775);	    // csp.tr.pwrb.tpl_tef3
		set_unit_value_ssc_double(u3,"T2EPLF4" );       //, 0.0);           // csp.tr.pwrb.tpl_tef4
		set_unit_value_ssc_double(u3,"E2TPLF0" );       //, 0.03737);	    // csp.tr.pwrb.tpl_tff0
		set_unit_value_ssc_double(u3,"E2TPLF1" );       //, 0.98823);       // csp.tr.pwrb.tpl_tff1
		set_unit_value_ssc_double(u3,"E2TPLF2" );       //, -0.064991);     // csp.tr.pwrb.tpl_tff2
		set_unit_value_ssc_double(u3,"E2TPLF3" );       //, 0.039388);      // csp.tr.pwrb.tpl_tff3
		set_unit_value_ssc_double(u3,"E2TPLF4" );       //, 0.0);	        // csp.tr.pwrb.tpl_tff4
		set_unit_value_ssc_double(u3,"TempCorrF" );     //, 1);     // csp.tr.pwrb.temp_corr_mode + 1
		set_unit_value_ssc_double(u3,"TempCorr0" );       //, 1);           // csp.tr.pwrb.ctcf0
		set_unit_value_ssc_double(u3,"TempCorr1" );       //, 0.0);         // csp.tr.pwrb.ctcf1
		set_unit_value_ssc_double(u3,"TempCorr2" );       //, 0.0);         // csp.tr.pwrb.ctcf2
		set_unit_value_ssc_double(u3,"TempCorr3" );       //, 0.0);         // csp.tr.pwrb.ctcf3
		set_unit_value_ssc_double(u3,"TempCorr4" );       //, 0.0);         // csp.tr.pwrb.ctcf4
		set_unit_value_ssc_double(u3,"TurTesEffAdj" );       //, 0.985);    // csp.tr.tes.adj_eff
		set_unit_value_ssc_double(u3,"TurTesOutAdj" );       //, 0.998);    // csp.tr.tes.adj_output
		set_unit_value_ssc_double(u3,"MinGrOut" );       //, 0.25);         // csp.tr.pwrb.minoutput
		set_unit_value_ssc_double(u3,"MaxGrOut" );       //, 1.05);         // csp.tr.pwrb.maxoutput

		set_unit_value_ssc_double(u3,"NUMTOU" );       //, 9);              // csp.tr.tes.dispX.fossil, where X = 1 to 9
		//double t4[9] = {0,0,0,0,0,0,0,0,0};
		set_unit_value_ssc_array(u3, "FossilFill" ); //, t4, 9 );
		set_unit_value_ssc_double(u3,"PbFixPar" );       //, 0.6105);       // csp.tr.par.fixedblock.total
		set_unit_value_ssc_double(u3,"BOPPar" );       //, 	2.73837);       // csp.tr.par.bop.total
		set_unit_value_ssc_double(u3,"BOPParPF" );       //, 1);            // csp.tr.par.bop.partload
		set_unit_value_ssc_double(u3,"BOPParF0" );       //, 0.483);        // csp.tr.par.bop.f0
		set_unit_value_ssc_double(u3,"BOPParF1" );       //, 0.517);        // csp.tr.par.bop.f1
		set_unit_value_ssc_double(u3,"BOPParF2" );       //, 0.0);          // csp.tr.par.bop.f2
		set_unit_value_ssc_double(u3,"CtPar" );       //,  1.892);	        // csp.tr.par.ct0.total
		set_unit_value_ssc_double(u3,"CtParPF" );       //, 1); 	        // csp.tr.par.ct0.partload
		set_unit_value_ssc_double(u3,"CtParF0" );       //, -0.036);	    // csp.tr.par.ct0.f0
		set_unit_value_ssc_double(u3,"CtParF1" );       //, 0.242);	        // csp.tr.par.ct0.f1
		set_unit_value_ssc_double(u3,"CtParF2" );       //, 0.794);	        // csp.tr.par.ct0.f2
		set_unit_value_ssc_double(u3,"HtrPar" );       //, 2.52303);	    // csp.tr.par.hb.total
		set_unit_value_ssc_double(u3,"HtrParPF" );       //, 1);            // csp.tr.par.hb.partload
		set_unit_value_ssc_double(u3,"HtrParF0" );       //, 0.483);        // csp.tr.par.hb.f0
		set_unit_value_ssc_double(u3,"HtrParF1" );       //, 0.517);        // csp.tr.par.hb.f1
		set_unit_value_ssc_double(u3,"HtrParF2" );       //, 0.0);          // csp.tr.par.hb.f2
	
		set_unit_value_ssc_double(u3,"LHVBoilEff" );       //, 0.9);       // csp.tr.pwrb.boiler_lhv_eff
		set_unit_value_ssc_double(u3,"TOUPeriod" );       //, 1 );// uniform dispatch
		set_unit_value_ssc_double(u3,"CtOpF" );       //, 1);              // csp.tr.par.operation_mode

		bConnected &= connect(u1, "Qdesign", u3, "Qdesign", 0.1, -1);
		bConnected &= connect(u1, "Edesign", u3,"Edesign", 0.1, -1);
		bConnected &= connect(u2,"Qtpb", u3,"Qtpb", 0.1,-1);
		bConnected &= connect(u2,"Qfts",u3,"Qfts", 0.1,-1);	   
		bConnected &= connect(weather, "twet",u3,"Twetbulb", 0.1, -1 );    
		bConnected &= connect( weather, "tdry", u3,"Tdrybulb", 0.1, -1 );		
		bConnected &= connect(u1, "SFTotPar", u3,"SFTotPar", 0.1, -1 );  
		bConnected &= connect(u2, "EparHhtf", u3,"EparHhtf", 0.1, -1 );      	  

		// check if all connections worked
		if ( !bConnected )
			throw exec_error( "tcstrough_empirical", util::format("there was a problem connecting outputs of one unit to inputs of another for the simulation.") );

		// Run simulation
		size_t hours = 8760;
		if (0 > simulate(3600, hours*3600, 3600) )
			throw exec_error( "tcstrough_empirical", util::format("there was a problem simulating in tcstrough_empirical.") );

		// get the outputs
		if (!set_all_output_arrays() )
			throw exec_error( "tcstrough_empirical", util::format("there was a problem returning the results from the simulation.") );


		//set_output_array("i_SfTi",8760);
	}

};

DEFINE_TCS_MODULE_ENTRY( tcstrough_empirical, "CSP model using the emperical trough TCS types.", 4 )
