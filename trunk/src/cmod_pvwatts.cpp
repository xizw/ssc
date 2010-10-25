#include "core.h"

#include "lib_pvwatts.h"

static var_info _cm_vtab_pvwatts[] = {

/*   VARTYPE           DATATYPE         NAME                         LABEL                              UNITS     META                      GROUP          REQUIRED_IF                 CONSTRAINTS                      UI_HINTS*/
	{ SSC_INPUT,        SSC_NUMBER,      "year",                       "Year (defaults to 1990)",        "",       "",                      "Weather",      "?=1990",                  "INTEGER,MIN=1950",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,      "lat",                        "Latitude",                       "deg",    "",                      "Weather",      "*",                       "MIN=-90,MAX=90",                           "" },
	{ SSC_INPUT,        SSC_NUMBER,      "lon",                        "Longitude",                      "deg",    "",                      "Weather",      "*",                       "MIN=-180,MAX=180",                         "" },
	{ SSC_INPUT,        SSC_NUMBER,      "tz",                         "Time zone rel. GMT",             "hrs",    "",                      "Weather",      "*",                       "",                                         "" },
	
	{ SSC_INPUT,        SSC_ARRAY,       "dn",                         "Direct normal radiation",        "W/m2",   "",                      "Weather",      "*",                       "",                                         "" },
	{ SSC_INPUT,        SSC_ARRAY,       "df",                         "Diffuse radiation",              "W/m2",   "",                      "Weather",      "*",                       "LENGTH_EQUAL=dn",                          "" },
	{ SSC_INPUT,        SSC_ARRAY,       "tdry",                       "Dry bulb temp",                  "'C",     "",                      "Weather",      "*",                       "LENGTH_EQUAL=dn",                          "" },
	{ SSC_INPUT,        SSC_ARRAY,       "wspd",                       "Wind speed",                     "m/s",    "",                      "Weather",      "*",                       "LENGTH_EQUAL=dn",                          "" },
	
	{ SSC_INPUT,        SSC_NUMBER,      "system_size",                "Nameplate capacity",             "kW",     "",                      "PVWatts",      "*",                       "MIN=0.5,MAX=100000",                       "" },
	{ SSC_INPUT,        SSC_NUMBER,      "derate",                     "System derate value",            "frac",   "",                      "PVWatts",      "*",                       "MIN=0,MAX=1",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "track_mode",                 "Tracking mode",                  "0/1/2",  "Fixed,1Axis,2Axis",     "PVWatts",      "*",                       "MIN=0,MAX=2,INTEGER",                      "" }, 
	{ SSC_INPUT,        SSC_NUMBER,      "tilt",                       "Tilt angle",                     "deg",    "E=90,S=180,W=270",      "PVWatts",      "*",                       "MIN=0,MAX=360",                            "" },
	{ SSC_INPUT,        SSC_NUMBER,      "azimuth",                    "Azimuth angle",                  "deg",    "H=0,V=90",              "PVWatts",      "*",                       "MIN=0,MAX=90",                             "" },
	{ SSC_INPUT,        SSC_NUMBER,      "tilt_eq_lat",                "Tilt=latitude override",         "0/1",    "",                      "PVWatts",      "?",                       "BOOLEAN",                                  "" },

	{ SSC_OUTPUT,       SSC_ARRAY,       "poa",                        "Plane of array radiation",       "W/m2",   "",                      "PVWatts",      "*",                       "LENGTH_EQUAL=dn",                          "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "tmod",                       "Module temperature",             "'C",     "",                      "PVWatts",      "*",                       "LENGTH_EQUAL=dn",                          "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "dc",                         "DC array output",                "kWhdc",  "",                      "PVWatts",      "*",                       "LENGTH_EQUAL=dn",                          "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "ac",                         "AC system output",               "kWhac",  "",                      "PVWatts",      "*",                       "LENGTH_EQUAL=dn",                          "" },

var_info_invalid };

static param_info _cm_param_tab[] = {
	/* TYPE,      NAME,       DEFAULT_VALUE,    DESCRIPTION */
	{ SSC_NUMBER, "t_start",  "0",              "Simulation start time (hours), 0 = Jan 1st, 12am" },
	{ SSC_NUMBER, "t_end",    "8760",           "Simulation end time (hours), 8759 = Dec 31st 11pm" },
	{ SSC_NUMBER, "t_step",   "1",              "Simulation time step (hours), must represent integer number of minutes" },
	{ SSC_INVALID, NULL }  };
	
class cm_pvwatts : public compute_module
{
private:
public:
	cm_pvwatts()
	{
		add_var_info( _cm_vtab_pvwatts );
		set_param_info( _cm_param_tab );
	}

	int month_of(float hour)
	{
		if (hour < 0) return 0;
		if (hour < 744) return 1;
		if (hour < 1416) return 2;
		if (hour < 2160) return 3;
		if (hour < 2880) return 4;
		if (hour < 3624) return 5;
		if (hour < 4344) return 6;
		if (hour < 5088) return 7;
		if (hour < 5832) return 8;
		if (hour < 6552) return 9;
		if (hour < 7296) return 10;
		if (hour < 8016) return 11;
		if (hour < 8760) return 12;
		return 0;
	}

	bool exec( ) throw( general_error )
	{
		float t_start = (float)param_number("t_start");
		float t_end = (float)param_number("t_end");
		float t_step = (float)param_number("t_step");

		size_t data_values = check_timestep( t_start, t_end, t_step );

		size_t arr_len;
		ssc_number_t *p_dn = as_array( "dn", &arr_len );      if (arr_len != data_values) throw mismatch_error( (int)data_values, (int)arr_len, "direct normal radiation");
 		ssc_number_t *p_df = as_array( "df", &arr_len );      if (arr_len != data_values) throw mismatch_error( (int)data_values, (int)arr_len, "diffuse radiation");
		ssc_number_t *p_tdry = as_array( "tdry", &arr_len );  if (arr_len != data_values) throw mismatch_error( (int)data_values, (int)arr_len, "dry bulb temperature");
		ssc_number_t *p_wspd = as_array( "wspd", &arr_len );  if (arr_len != data_values) throw mismatch_error( (int)data_values, (int)arr_len, "wind speed");
		
		int year = as_integer("year");
		double lat = as_double("lat");
		double lon = as_double("lon");
		double tz = as_double("tz");

		double watt_spec = 1000.0 * as_double("system_size");
		double derate = as_double("derate");
		int track_mode = as_integer("track_mode"); // 0, 1, 2
		double tilt = as_double("tilt");
		double azimuth = as_double("azimuth");
		if ( as_boolean("tilt_eq_lat") ) tilt = lat; // override tilt angle

		ssc_number_t *p_poa = allocate("poa", data_values);
		ssc_number_t *p_tmod = allocate("tmod", data_values);
		ssc_number_t *p_dc = allocate("dc", data_values);
		ssc_number_t *p_ac = allocate("ac", data_values);

		
		/* PV RELATED SPECIFICATIONS */
		double inoct = 45.0 + 273.15;        /* Installed normal operating cell temperature (deg K) */
		double height = 5.0;                 /* Average array height (meters) */
		double reftem = 25.0;                /* Reference module temperature (deg C) */
		double pwrdgr = -0.005;              /* Power degradation due to temperature (decimal fraction), si approx -0.004 */
		double efffp = 0.92;                 /* Efficiency of inverter at rated output (decimal fraction) */
		double tmloss = 1.0 - derate/efffp;  /* All losses except inverter,decimal */
		double rot_limit = 45.0;             /* +/- rotation in degrees permitted by physical constraint of tracker */
		double albedo = 0.2;                 /* surface albedo, decimal fraction */

		/* storage for calculations */
		double angle[3];
		double sun[8];

		float time = t_start;
		size_t idx = 0;
		while ( time < t_end && idx < data_values )
		{
			// calculate month, day, hour, minute time
			float tmid = time + t_step/2;
			int month = month_of(time);              // month goes 1-12
			int day =  ( ((int)(time/24.0)) + 1 );   // day goes 1-365
			int hour = (int)(time)%24;		         // hour goes 0-23
			int minute = (int)(time-floor(time))*60;      // minute goes 0-59
			
			// calculate solar position
			solarpos( year, month, day, hour, minute, lat, lon, tz, sun );

			double poa, pvt, dc, ac;

			if (sun[2] > 0.0087)
			{
				/* sun elevation > 0.5 degrees */
				double dn = p_dn[idx];
				double df = p_df[idx];
				double wind = p_wspd[idx];
				double ambt = p_tdry[idx];

				incident2( track_mode, tilt, azimuth, rot_limit, sun[1], sun[0], angle );
				poa = perez( dn, df, albedo, angle[0], angle[1], sun[1] );

				double tpoa = 0;
				if (dn > 0)	
					tpoa = transpoa( poa, dn, angle[0] );  /* have valid poa and dn, calculate transmitted through glass cover */
				else
					tpoa = poa; /* default to dn 0 or bad value - assume no glass cover on module */
				
				pvt = celltemp(inoct, height, poa, wind, ambt );
				dc = dcpowr( reftem, watt_spec, pwrdgr, tmloss, tpoa, pvt );
				ac = dctoac( watt_spec, efffp, dc );
			}
			else
			{
				/* night time */
				poa = 0.0;
				pvt = 999.9;
				dc = 0.0;
				ac = 0.0;
			}

			p_poa[idx] = (ssc_number_t)poa;
			p_tmod[idx] = (ssc_number_t)pvt;
			p_dc[idx] = (ssc_number_t)dc;
			p_ac[idx] = (ssc_number_t)ac;

			time += t_step;
			idx++;
		}
		
		return true;
	}
};

DEFINE_MODULE_ENTRY( pvwatts, "Integrated PV system simulator.", 1 )
