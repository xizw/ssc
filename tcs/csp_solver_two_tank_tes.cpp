#include "csp_solver_two_tank_tes.h"
#include "csp_solver_util.h"

C_heat_exchanger::C_heat_exchanger()
{
	m_m_dot_des_ave = m_eff_des = m_UA_des = std::numeric_limits<double>::quiet_NaN();
}

void C_heat_exchanger::init(HTFProperties &fluid_field, HTFProperties &fluid_store, double q_transfer_des /*W*/,
	double dt_des, double T_h_in_des /*K*/, double T_h_out_des /*K*/)
{
	// Counter flow heat exchanger

		// Design should provide field/pc side design temperatures
	double T_ave = (T_h_in_des + T_h_out_des) / 2.0;		//[K] Average hot side temperature
	double c_h = mc_field_htfProps.Cp(T_ave)*1000.0;		//[J/kg-K] Spec heat of hot side fluid at hot side average temperature, convert from [kJ/kg-K]
	double c_c = mc_store_htfProps.Cp(T_ave)*1000.0;		//[J/kg-K] Spec heat of cold side fluid at hot side average temperature (estimate, but should be close)
		// HX inlet and outlet temperatures
	double T_c_out = T_h_in_des - dt_des;
	double T_c_in = T_h_out_des - dt_des;
		// Mass flow rates
	double m_dot_h = q_transfer_des/(c_h*(T_h_in_des - T_h_out_des));	//[kg/s]
	double m_dot_c = q_transfer_des/(c_c*(T_c_out - T_c_in));			//[kg/s]
	m_m_dot_des_ave = 0.5*(m_dot_h + m_dot_c);		//[kg/s]
		// Capacitance rates
	double c_dot_h = m_dot_h * c_h;				//[W/K]
	double c_dot_c = m_dot_c * c_c;				//[W/K]
	double c_dot_max = fmax(c_dot_h, c_dot_c);	//[W/K]
	double c_dot_min = fmin(c_dot_h, c_dot_c);	//[W/K]
	double cr = c_dot_min / c_dot_max;			//[-]
		// Maximum possible energy flow rate
	double q_max = c_dot_min * (T_h_in_des - T_c_in);	//[W]
		// Effectiveness
	m_eff_des = q_transfer_des / q_max;

	// Check for realistic conditions
	if(cr > 1.0 || cr < 0.0)
	{
		throw(C_csp_exception("Heat exchanger design calculations failed", ""));
	}

	double NTU = std::numeric_limits<double>::quiet_NaN();
	if(cr < 1.0)
		NTU = log((1. - m_eff_des*cr) / (1. - m_eff_des)) / (1. - cr);
	else
		NTU = m_eff_des / (1. - m_eff_des);

	m_UA_des = NTU * c_dot_min;		//[W/K]
}

void C_heat_exchanger::hx_charge_mdot_tes(double T_cold_tes, double m_dot_tes, double T_hot_field,
	double &eff, double &T_hot_tes, double &T_cold_field, double &q_trans, double &m_dot_field)
{
	hx_performance(false, true, T_hot_field, m_dot_tes, T_cold_tes,
		eff, T_hot_tes, T_cold_field, q_trans, m_dot_field);
}

void C_heat_exchanger::hx_discharge_mdot_tes(double T_hot_tes, double m_dot_tes, double T_cold_field,
	double &eff, double &T_cold_tes, double &T_hot_field, double &q_trans, double &m_dot_field)
{
	hx_performance(true, true, T_hot_tes, m_dot_tes, T_cold_field,
		eff, T_hot_field, T_cold_tes, q_trans, m_dot_field);
}

void C_heat_exchanger::hx_charge_mdot_field(double T_hot_field, double m_dot_field, double T_cold_tes,
	double &eff, double &T_cold_field, double &T_hot_tes, double &q_trans, double &m_dot_tes)
{
	hx_performance(true, false, T_hot_field, m_dot_field, T_cold_tes,
		eff, T_hot_tes, T_cold_field, q_trans, m_dot_tes);
}

void C_heat_exchanger::hx_discharge_mdot_field(double T_cold_field, double m_dot_field, double T_hot_tes,
	double &eff, double &T_hot_field, double &T_cold_tes, double &q_trans, double &m_dot_tes)
{
	hx_performance(false, false, T_hot_tes, m_dot_field, T_cold_field,
		eff, T_hot_field, T_cold_tes, q_trans, m_dot_tes);
}

void C_heat_exchanger::hx_performance(bool is_hot_side_mdot, bool is_storage_side, double T_hot_in, double m_dot_known, double T_cold_in,
	double &eff, double &T_hot_out, double &T_cold_out, double &q_trans, double &m_dot_solved)
{
	// UA fixed

	// Subroutine for storage heat exchanger performance
	// Pass a flag to determine whether mass flow rate is cold side or hot side. Return mass flow rate will be the other
	// Also pass a flag to determine whether storage or field side mass flow rate is known. Return mass flow rate will be the other
	// Inputs: hot side mass flow rate [kg/s], hot side inlet temp [K], cold side inlet temp [K]
	// Outputs: HX effectiveness [-], hot side outlet temp [K], cold side outlet temp [K], 
	//				Heat transfer between fluids [MWt], cold side mass flow rate [kg/s]

	double m_dot_hot, m_dot_cold, c_hot, c_cold, c_dot;

	double T_ave = (T_hot_in + T_cold_in) / 2.0;		//[K]

	if( is_hot_side_mdot )		// know hot side mass flow rate - assuming always know storage side and solving for field
	{
		if( is_storage_side )
		{
			c_cold = mc_field_htfProps.Cp(T_ave)*1000.0;		//[J/kg-K]
			c_hot = mc_store_htfProps.Cp(T_ave)*1000.0;			//[J/kg-K]
		}
		else
		{
			c_hot = mc_field_htfProps.Cp(T_ave)*1000.0;			//[J/kg-K]
			c_cold = mc_store_htfProps.Cp(T_ave)*1000.0;		//[J/kg-K]
		}
	
		m_dot_hot = m_dot_known;
		// Calculate flow capacitance of hot stream
		double c_dot_hot = m_dot_hot*c_hot;			//[W/K]
		c_dot = c_dot_hot;
		// Choose a cold stream mass flow rate that results in c_dot_h = c_dot_c
		m_dot_cold = c_dot_hot / c_cold;
		m_dot_solved = m_dot_cold;
	}
	else
	{
		if( is_storage_side )
		{
			c_hot = mc_field_htfProps.Cp(T_ave)*1000.0;		//[J/kg-K]
			c_cold = mc_store_htfProps.Cp(T_ave)*1000.0;	//[J/kg-K]
		}
		else
		{
			c_cold = mc_field_htfProps.Cp(T_ave)*1000.0;	//[J/kg-K]
			c_hot = mc_store_htfProps.Cp(T_ave)*1000.0;		//[J/kg-K]
		}

		m_dot_cold = m_dot_known;
		// Calculate flow capacitance of cold stream
		double c_dot_cold = m_dot_cold*c_cold;
		c_dot = c_dot_cold;
		// Choose a cold stream mass flow rate that results in c_dot_c = c_dot_h
		m_dot_hot = c_dot_cold / c_hot;
		m_dot_solved = m_dot_hot;
	}

	// Scale UA
	double m_dot_od = 0.5*(m_dot_cold + m_dot_hot);
	double UA = m_UA_des*pow(m_dot_od / m_m_dot_des_ave, 0.8);

	// Calculate effectiveness
	double NTU = UA / c_dot;
	eff = NTU / (1.0 + NTU);

	// Calculate heat transfer in HX
	double q_dot_max = c_dot*(T_hot_in - T_cold_in);
	q_trans = eff*q_dot_max;

	T_hot_out = T_hot_in - q_trans / c_dot;
	T_cold_out = T_cold_in + q_trans / c_dot;

	q_trans *= 1.E-6;		//[MWt]

	if( eff <= 0.0 || eff > 1.0 )
	{
		throw(C_csp_exception("Off design heat exchanger failed", ""));
	}
}

C_storage_tank::C_storage_tank()
{
	m_V_prev = m_T_prev = m_m_prev =
		
	m_V_total = m_V_active = m_V_inactive = m_UA = 
	
	m_T_htr = m_max_q_htr = std::numeric_limits<double>::quiet_NaN();
}

void C_storage_tank::init(HTFProperties htf_class_in, double V_tank_one_temp, double h_tank, double h_min, double u_tank, 
	double tank_pairs, double T_htr, double max_q_htr)
{
	mc_htf = htf_class_in;

	m_V_total = V_tank_one_temp;				//[m^3]

	m_V_inactive = m_V_total*h_min / h_tank;	//[m^3]

	m_V_active = m_V_total - m_V_inactive;		//[m^3]

	double A_cs = m_V_total / (h_tank*tank_pairs);		//[m^2] Cross-sectional area of a single tank

	double diameter = pow(A_cs / CSP::pi, 0.5)*2.0;		//[m] Diameter of a single tank

	// Calculate tank conductance
	m_UA = u_tank*(A_cs + CSP::pi*diameter*h_tank)*tank_pairs;

	m_T_htr = T_htr;
	m_max_q_htr = max_q_htr;
}

double C_storage_tank::calc_mass_at_prev()
{
	return m_V_prev*mc_htf.dens(m_T_prev, 1.0);	//[kg] 
}

double C_storage_tank::m_dot_available(double f_unavail, double timestep)
{
	double rho = mc_htf.dens(m_T_prev, 1.0);		//[kg/m^3]
	double V = m_m_prev / rho;						//[m^3] Volume available in tank (one temperature)
	double V_avail = fmax(V - m_V_inactive, 0.0);	//[m^3] Volume that is active - need to maintain minimum height (corresponding m_V_inactive)

	// "Unavailable" fraction now applied to one temperature tank volume, not total tank volume
	double m_dot_avail = fmax(V_avail - m_V_active*f_unavail, 0.0)*rho / timestep;		//[kg/s] Max mass flow rate available

	return m_dot_avail;		//[kg/s]
}

void C_storage_tank::converged()
{
	// Reset 'previous' timestep values to 'calculated' values
	m_V_prev = m_V_calc;		//[m^3]
	m_T_prev = m_T_calc;		//[K]
	m_m_prev = m_m_calc;		//[kg]
}

void C_storage_tank::energy_balance(double timestep /*s*/, double m_dot_in, double m_dot_out, double T_in /*K*/, double T_amb /*K*/, 
	double &T_ave /*K*/, double & q_heater /*MW*/, double & q_dot_loss /*MW*/)
{
	// Get properties from tank state at the end of last time step
	double rho = mc_htf.dens(m_T_prev, 1.0);	//[kg/m^3]
	double cp = mc_htf.Cp(m_T_prev)*1000.0;		//[J/kg-K] spec heat, convert from kJ/kg-K

	// Calculate ending volume levels
	m_m_calc = fmax(0.001, m_m_prev + timestep*(m_dot_in - m_dot_out));	//[kg] Available mass at the end of this timestep, limit to nonzero positive number
	//double m_ave = (m_m_prev + m_m_calc) / 2.0;	//[kg] Average mass in tank during timestep
	m_V_calc = m_m_calc / rho;					//[m^3] Available volume at end of timestep (using initial temperature...)		

	if( (m_dot_in - m_dot_out) != 0.0 )
	{
		double a_coef = m_dot_in*T_in + m_UA / cp*T_amb;
		double b_coef = m_dot_in + m_UA / cp;
		double c_coef = (m_dot_in - m_dot_out);

		m_T_calc = a_coef / b_coef + (m_T_prev - a_coef / b_coef)*pow((timestep*c_coef / m_m_prev + 1), -b_coef / c_coef);
		T_ave = a_coef / b_coef + m_m_prev*(m_T_prev - a_coef / b_coef) / ((c_coef - b_coef)*timestep)*(pow((timestep*c_coef / m_m_prev + 1.0), -b_coef / c_coef) - 1.0);
		q_dot_loss = m_UA*(T_ave - T_amb)/1.E6;		//[MW]

		if( m_T_calc < m_T_htr )
		{
				q_heater = b_coef*((m_T_htr - m_T_prev*pow((timestep*c_coef / m_m_prev + 1), -b_coef / c_coef)) /
					(-pow((timestep*c_coef / m_m_prev + 1), -b_coef / c_coef) + 1)) - a_coef;

				q_heater = q_heater*cp;

				q_heater /= 1.E6;
		}
		else
		{
			q_heater = 0.0;
			return;
		}

		if( q_heater > m_max_q_htr )
		{
			q_heater = m_max_q_htr;
		}

		a_coef += q_heater*1.E6 / cp;

		m_T_calc = a_coef / b_coef + (m_T_prev - a_coef / b_coef)*pow((timestep*c_coef / m_m_prev + 1), -b_coef / c_coef);
		T_ave = a_coef / b_coef + m_m_prev*(m_T_prev - a_coef / b_coef) / ((c_coef - b_coef)*timestep)*(pow((timestep*c_coef / m_m_prev + 1.0), -b_coef / c_coef) - 1.0);
		q_dot_loss = m_UA*(T_ave - T_amb)/1.E6;		//[MW]

	}
	else	// No mass flow rate, tank is idle
	{
		double b_coef = m_UA / (cp*m_m_prev);
		double c_coef = m_UA / (cp*m_m_prev) * T_amb;

		m_T_calc = c_coef / b_coef + (m_T_prev - c_coef / b_coef)*exp(-b_coef*timestep);
		T_ave = c_coef/b_coef - (m_T_prev - c_coef/b_coef)/(b_coef*timestep)*(exp(-b_coef*timestep)-1.0);
		q_dot_loss = m_UA*(T_ave - T_amb)/1.E6;

		if( m_T_calc < m_T_htr )
		{
			q_heater = (b_coef*(m_T_htr - m_T_prev*exp(-b_coef*timestep)) / (-exp(-b_coef*timestep) + 1.0) - c_coef)*cp*m_m_prev;
			q_heater /= 1.E6;	//[MW]
		}
		else
		{
			q_heater = 0.0;
			return;
		}

		if( q_heater > m_max_q_htr )
		{
			q_heater = m_max_q_htr;
		}

		c_coef += q_heater*1.E6 / (cp*m_m_prev);

		m_T_calc = c_coef / b_coef + (m_T_prev - c_coef / b_coef)*exp(-b_coef*timestep);
		T_ave = c_coef / b_coef - (m_T_prev - c_coef / b_coef) / (b_coef*timestep)*(exp(-b_coef*timestep) - 1.0);
		q_dot_loss = m_UA*(T_ave - T_amb)/1.E6;		//[MW]
	}
}

C_csp_two_tank_tes::C_csp_two_tank_tes()
{
	m_V_tank_active = std::numeric_limits<double>::quiet_NaN();

	m_m_dot_tes_dc_max = m_m_dot_tes_ch_max = std::numeric_limits<double>::quiet_NaN();
}

void C_csp_two_tank_tes::init()
{
	if( !(ms_params.m_ts_hours > 0.0) )
	{
		m_is_tes = false;
		return;		// No storage!
	}

	m_is_tes = true;

	// Declare instance of fluid class for FIELD fluid
	// Set fluid number and copy over fluid matrix if it makes sense
	if( ms_params.m_field_fl != HTFProperties::User_defined && ms_params.m_field_fl < HTFProperties::End_Library_Fluids )
	{
		if( !mc_field_htfProps.SetFluid(ms_params.m_field_fl) )
		{
			throw(C_csp_exception("Field HTF code is not recognized", "Two Tank TES Initialization"));
		}
	}
	else if( ms_params.m_field_fl == HTFProperties::User_defined )
	{
		int n_rows = ms_params.m_field_fl_props.nrows();
		int n_cols = ms_params.m_field_fl_props.ncols();
		if( n_rows > 2 && n_cols == 7 )
		{
			if( !mc_field_htfProps.SetUserDefinedFluid(ms_params.m_field_fl_props) )
			{
				error_msg = util::format(mc_field_htfProps.UserFluidErrMessage(), n_rows, n_cols);
				throw(C_csp_exception(error_msg, "Two Tank TES Initialization"));
			}
		}
		else
		{
			error_msg = util::format("The user defined field HTF table must contain at least 3 rows and exactly 7 columns. The current table contains %d row(s) and %d column(s)", n_rows, n_cols);
			throw(C_csp_exception(error_msg, "Two Tank TES Initialization"));
		}
	}
	else
	{
		throw(C_csp_exception("Field HTF code is not recognized", "Two Tank TES Initialization"));
	}


	// Declare instance of fluid class for STORAGE fluid.
	// Set fluid number and copy over fluid matrix if it makes sense.
	if( ms_params.m_tes_fl != HTFProperties::User_defined && ms_params.m_tes_fl_props < HTFProperties::End_Library_Fluids )
	{
		if( !mc_store_htfProps.SetFluid(ms_params.m_tes_fl) )
		{
			throw(C_csp_exception("Storage HTF code is not recognized", "Two Tank TES Initialization"));
		}	
	}
	else if( ms_params.m_tes_fl == HTFProperties::User_defined )
	{
		int n_rows = ms_params.m_tes_fl_props.nrows();
		int n_cols = ms_params.m_tes_fl_props.ncols();
		if( n_rows > 2 && n_cols == 7 )
		{
			if( !mc_store_htfProps.SetUserDefinedFluid(ms_params.m_tes_fl_props) )
			{
				error_msg = util::format(mc_store_htfProps.UserFluidErrMessage(), n_rows, n_cols);
				throw(C_csp_exception(error_msg, "Two Tank TES Initialization"));
			}
		}
		else
		{
			error_msg = util::format("The user defined storage HTF table must contain at least 3 rows and exactly 7 columns. The current table contains %d row(s) and %d column(s)", n_rows, n_cols);
			throw(C_csp_exception(error_msg, "Two Tank TES Initialization"));
		}
	}
	else
	{
		throw(C_csp_exception("Storage HTF code is not recognized", "Two Tank TES Initialization"));
	}

	bool is_hx_calc = true;

	if( ms_params.m_tes_fl != ms_params.m_field_fl )
		is_hx_calc = true;
	else if( ms_params.m_field_fl != HTFProperties::User_defined )
		is_hx_calc = false;
	else
	{
		is_hx_calc = !mc_field_htfProps.equals(&mc_store_htfProps);
	}

	if( ms_params.m_is_hx != is_hx_calc )
	{
		if( is_hx_calc )
			mc_csp_messages.add_message(C_csp_messages::NOTICE, "Input field and storage fluids are different, but the inputs did not specify a field-to-storage heat exchanger. The system was modeled assuming a heat exchanger.");
		else
			mc_csp_messages.add_message(C_csp_messages::NOTICE, "Input field and storage fluids are identical, but the inputs specified a field-to-storage heat exchanger. The system was modeled assuming no heat exchanger.");

		ms_params.m_is_hx = is_hx_calc;
	}

	// Convert parameter units
	ms_params.m_q_pb_design *= 1.E6;			//[W] convert from MW
	ms_params.m_hot_tank_Thtr += 273.15;		//[K] convert from C
	ms_params.m_cold_tank_Thtr += 273.15;		//[K] convert from C
	ms_params.m_T_field_in_des += 273.15;		//[K] convert from C
	ms_params.m_T_field_out_des += 273.15;		//[K] convert from C
	ms_params.m_T_tank_hot_ini += 273.15;		//[K] convert from C
	ms_params.m_T_tank_cold_ini += 273.15;		//[K] convert from C

	// 5.13.15, twn: also be sure that hx is sized such that it can supply full load to power cycle, in cases of low solar multiples
	double duty = ms_params.m_q_pb_design * fmax(1.0, ms_params.m_solarm);		//[W] Allow all energy from the field to go into storage at any time

	if( ms_params.m_ts_hours > 0.0 )
	{
		mc_hx.init(mc_field_htfProps, mc_store_htfProps, duty, ms_params.m_dt_hot, ms_params.m_T_field_out_des, ms_params.m_T_field_in_des);
	}

	// Do we need to define minimum and maximum thermal powers to/from storage?
	// The 'duty' definition should allow the tanks to accept whatever the field and/or power cycle can provide...

	// Set initial storage values
		// Initialize cold and hot tanks
	mc_hot_tank.init(mc_store_htfProps, ms_params.m_vol_tank, ms_params.m_h_tank, ms_params.m_h_tank_min, 
		ms_params.m_u_tank, ms_params.m_tank_pairs, ms_params.m_hot_tank_Thtr, ms_params.m_hot_tank_max_heat);
	mc_cold_tank.init(mc_store_htfProps, ms_params.m_vol_tank, ms_params.m_h_tank, ms_params.m_h_tank_min, 
		ms_params.m_u_tank, ms_params.m_tank_pairs, ms_params.m_cold_tank_Thtr, ms_params.m_cold_tank_max_heat);

	mc_hot_tank.m_V_prev = ms_params.m_V_tank_hot_ini;						//[m^3]
	mc_cold_tank.m_V_prev = ms_params.m_vol_tank - mc_hot_tank.m_V_prev;	//[m^3]

	mc_hot_tank.m_T_prev = ms_params.m_T_tank_hot_ini;		//[K]
	mc_cold_tank.m_T_prev = ms_params.m_T_tank_cold_ini;	//[K]

	mc_hot_tank.m_m_prev = mc_hot_tank.calc_mass_at_prev();
	mc_cold_tank.m_m_prev = mc_cold_tank.calc_mass_at_prev();

	m_V_tank_active = ms_params.m_vol_tank*(1.0 - 2.0*ms_params.m_h_tank_min / ms_params.m_h_tank);
}

bool C_csp_two_tank_tes::does_tes_exist()
{
	return m_is_tes;
}

void C_csp_two_tank_tes::discharge_avail_est(double T_cold_K, double step_s, double &q_dot_dc_est, double &m_dot_field_est, double &T_hot_field_est) 
{
	double f_storage = 0.0;		// for now, hardcode such that storage always completely discharges

	double m_dot_tank_disch_avail = mc_hot_tank.m_dot_available(f_storage, step_s);

	if(ms_params.m_is_hx)
	{
		double eff, T_cold_tes;
		eff = T_cold_tes = std::numeric_limits<double>::quiet_NaN();
		mc_hx.hx_discharge_mdot_tes(mc_hot_tank.m_T_prev, m_dot_tank_disch_avail, T_cold_K, eff, T_cold_tes, T_hot_field_est, q_dot_dc_est, m_dot_field_est);
		
		// If above method fails, it will throw an exception, so if we don't want to break here, need to catch and handle it
	}
	else
	{
		double cp_T_avg = mc_store_htfProps.Cp(0.5*(T_cold_K + mc_hot_tank.m_T_prev));		//[kJ/kg-K] spec heat at average temperature during discharge from hot to cold
		
		q_dot_dc_est = m_dot_tank_disch_avail * cp_T_avg * (mc_hot_tank.m_T_prev - T_cold_K)*1.E-3;	//[MW]

		m_dot_field_est = m_dot_tank_disch_avail;

		T_hot_field_est = mc_hot_tank.m_T_prev;
	}

	m_m_dot_tes_dc_max = m_dot_tank_disch_avail;
}

void C_csp_two_tank_tes::charge_avail_est(double T_hot_K, double step_s, double &q_dot_ch_est, double &m_dot_field_est, double &T_cold_field_est)
{
	double f_ch_storage = 0.0;	// for now, hardcode such that storage always completely charges

	double m_dot_tank_charge_avail = mc_cold_tank.m_dot_available(f_ch_storage, step_s);

	if(ms_params.m_is_hx)
	{
		double eff, T_hot_tes;
		eff = T_hot_tes = std::numeric_limits<double>::quiet_NaN();
		mc_hx.hx_charge_mdot_tes(mc_cold_tank.m_T_prev, m_dot_tank_charge_avail, T_hot_K, eff, T_hot_tes, T_cold_field_est, q_dot_ch_est, m_dot_field_est);

		// If above method fails, it will throw an exception, so if we don't want to break here, need to catch and handle it
	}
	else
	{
		double cp_T_avg = mc_store_htfProps.Cp(0.5*(mc_cold_tank.m_T_prev, T_hot_K));	//[kJ/kg-K] spec heat at average temperature during charging from cold to hot

		q_dot_ch_est = m_dot_tank_charge_avail * cp_T_avg * (T_hot_K - mc_cold_tank.m_T_prev) *1.E-3;	//[MW]

		m_dot_field_est = m_dot_tank_charge_avail;

		T_cold_field_est = mc_cold_tank.m_T_prev;
	}

	m_m_dot_tes_ch_max = m_dot_tank_charge_avail;
}

bool C_csp_two_tank_tes::discharge(double m_dot_htf_in /*kg/s*/, double T_htf_cold_in, double & T_htf_hot_out /*K*/)
{
	// This method calculates the storage discharge temperature on the HX side (if applicable). If no heat exchanger (direct storage),
	// the discharge temperature is equal to the average (timestep) hot tank outlet temperature.

	// Inputs are:
	// 1) Required hot side mass flow rate on the HX side (if applicable). If no heat exchanger, then the mass flow rate
	//     is equal to the hot tank exit mass flow rate (and cold tank fill mass flow rate)
	// 2) inlet temperature on the HX side (if applicable). If no heat exchanger, the inlet temperature is the temperature
	//	   of HTF directly entering the cold tank. 

	// If no heat exchanger, no iteration is required between the heat exchanger and storage tank models

	if(!ms_params.m_is_hx)
	{
		
	}
	else
	{

	}


	// Guess average hot tank outlet temperature
	double T_hot_out_avg = mc_hot_tank.m_T_prev;		//[K]

	// Set up iteration on T_hot_out_avg
		// Upper bound, error, booleans
	double T_hot_out_upper = std::numeric_limits<double>::quiet_NaN();
	double y_T_hot_out_upper = std::numeric_limits<double>::quiet_NaN();
	bool is_upper_bound = false;
	bool is_upper_error = false;
		// Lower bound, error, booleans
	double T_hot_out_lower = std::numeric_limits<double>::quiet_NaN();
	double y_T_hot_out_lower = std::numeric_limits<double>::quiet_NaN();
	bool is_lower_bound = false;
	bool is_lower_error = false;

	double tol = 0.0005;		//[-]

	double diff_T_hot = 999.9*tol;	//[-]

	int iter_T_hot_out = 0;

	// Start iteration loop
	while(abs(diff_T_hot) > tol)
	{
		iter_T_hot_out++;		// First iteration = 1

		double m_dot_tes = std::numeric_limits<double>::quiet_NaN();
		// If there is a heat exchanger, solve for TES-side mass flow rate
		if(ms_params.m_is_hx)
		{
			
		}
		else	// No heat exchanger, the TES-side mass flow rate is equal to the input mass flow rate
		{
			m_dot_tes = m_dot_htf_in;
		}


	}

	return false;

}

void C_csp_two_tank_tes::idle(double timestep, double T_amb, C_csp_tes::S_csp_tes_outputs &outputs)
{
	double T_hot_ave, q_hot_heater, q_dot_hot_loss;
	T_hot_ave = q_hot_heater = q_dot_hot_loss = std::numeric_limits<double>::quiet_NaN();

	mc_hot_tank.energy_balance(timestep, 0.0, 0.0, mc_hot_tank.m_T_prev, T_amb, T_hot_ave, q_hot_heater, q_dot_hot_loss);

	double T_cold_ave, q_cold_heater, q_dot_cold_loss;
	T_cold_ave = q_cold_heater = q_dot_cold_loss = std::numeric_limits<double>::quiet_NaN();

	mc_cold_tank.energy_balance(timestep, 0.0, 0.0, mc_cold_tank.m_T_prev, T_amb, T_cold_ave, q_cold_heater, q_dot_cold_loss);

	outputs.m_q_heater = q_cold_heater + q_hot_heater;			//[MJ]
	outputs.m_q_dot_loss = q_dot_cold_loss + q_dot_hot_loss;	//[MW]
	outputs.m_T_hot_final = mc_hot_tank.m_T_calc;		//[K]
	outputs.m_T_cold_final = mc_cold_tank.m_T_calc;		//[K]
}

void C_csp_two_tank_tes::converged()
{
	mc_cold_tank.converged();
	mc_hot_tank.converged();
	
	// The max charge and discharge flow rates should be set at the beginning of each timestep
	//   during the q_dot_xx_avail_est calls
	m_m_dot_tes_dc_max = m_m_dot_tes_ch_max = std::numeric_limits<double>::quiet_NaN();
}