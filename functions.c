#include "udf.h"
#include "math.h"
#include "mem.h"
#include "stdio.h"

#include "functions.h"


void _power_initialization(Domain *domain, real powerLevel)
{
	Thread *t;
	cell_t  c;

	t = Lookup_Thread(domain, 17);

	begin_c_loop(c,t)
	{
		_set_power_for_each_cell(c,t);
	}end_c_loop(c,t)

	_power_density_correction(c, t, powerLevel);
}

void _set_power_for_each_cell(cell_t c, Thread *t)
{
	real coordinate[ND_ND];
	real y;
	C_CENTROID(coordinate, c, t);
	y = coordinate[1];

	if      ( (y>0.0000) && (y<0.0339))
		C_UDMI(c,t,0)=46795440.460;
	else if ( (y>0.0339) && (y<0.1017))
		C_UDMI(c,t,0)=82950740.756;
	else if ( (y>0.1017) && (y<0.1696))
		C_UDMI(c,t,0)=80196480.218;
	else if ( (y>0.1696) && (y<0.2374))
		C_UDMI(c,t,0)=81374510.932;
	else if ( (y>0.2374) && (y<0.3052))
		C_UDMI(c,t,0)=84347800.635;
	else if ( (y>0.3052) && (y<0.3730))
		C_UDMI(c,t,0)=87577730.481;
	else if ( (y>0.3730) && (y<0.4409))
		C_UDMI(c,t,0)=90477760.658;
	else if ( (y>0.4409) && (y<0.5087))
		C_UDMI(c,t,0)=93000850.623;
	else if ( (y>0.5087) && (y<0.5765))
		C_UDMI(c,t,0)=94753360.010;
	else if ( (y>0.5765) && (y<0.6443))
		C_UDMI(c,t,0)=95680370.810;
	else if ( (y>0.6443) && (y<0.7121))
		C_UDMI(c,t,0)=96099570.014;
	else if ( (y>0.7121) && (y<0.7800))
		C_UDMI(c,t,0)=95338820.339;
	else if ( (y>0.7800) && (y<0.8478))
		C_UDMI(c,t,0)=93930350.465;
	else if ( (y>0.8478) && (y<0.9156))
		C_UDMI(c,t,0)=91791230.500;
	else if ( (y>0.9156) && (y<0.9834))
		C_UDMI(c,t,0)=88800410.963;
	else if ( (y>0.9834) && (y<1.0513))
		C_UDMI(c,t,0)=85178330.576;
	else if ( (y>1.0513) && (y<1.1191))
		C_UDMI(c,t,0)=81439580.917;
	else if ( (y>1.1191) && (y<1.1869))
		C_UDMI(c,t,0)=77516050.083;
	else if ( (y>1.1869) && (y<1.2547))
		C_UDMI(c,t,0)=74179380.135;
	else if ( (y>1.2547) && (y<1.3226))
		C_UDMI(c,t,0)=73328920.584;
	else
		C_UDMI(c,t,0)=58907060.580;
}

void _power_density_correction(cell_t c, Thread *t, real powerLevel)
{
	real _totalPower;
	_totalPower = 0;

	begin_c_loop(c,t)
	{
		_totalPower += C_UDMI(c,t,0)*C_VOLUME(c,t);
	}end_c_loop(c,t)

	begin_c_loop(c,t)
	{
		C_UDMI(c,t,0) = C_UDMI(c,t,0)/_totalPower*powerLevel*1.0e6;
	}end_c_loop(c,t)
}


real _max_coolant_temperature_calc(Domain *domain)
{
	Thread *t;
	cell_t c;
	real _max_coolant_temperature;
	real _fuel_surface_temperature;
	real _heat_transfer_coefficient;
	real _volume_power_rate;
	real _fuel_ball_power;
	real _porosity;
	real _volume_fuel_ball;
	real _coolant_velocity;

	t = Lookup_Thread(domain, 17);
	_max_coolant_temperature = 0.;
	_fuel_surface_temperature = 0.;
	_heat_transfer_coefficient = 0.;
	_volume_power_rate = 0.;
	_fuel_ball_power = 0.;
	_porosity = 0.3194;
	_volume_fuel_ball = 4./3.*3.1415926*pow(0.03,3);
	_coolant_velocity = 0.;

	begin_c_loop(c,t)
	{
		_volume_power_rate = C_UDMI(c,t,0);
		_fuel_ball_power = _volume_power_rate*_volume_fuel_ball/(1.-_porosity);
		_coolant_velocity = pow(C_U(c,t),2.) + pow(C_V(c,t),2.) + pow(C_W(c,t),2.);
		_coolant_velocity = pow(_coolant_velocity, 0.5);
		_heat_transfer_coefficient = \
			_heat_transfer_coefficient_calc(C_T(c,t), _coolant_velocity);

		_fuel_surface_temperature = C_T(c,t) + \
			_fuel_ball_power/(3.14159*0.06*0.06*_heat_transfer_coefficient);

		if(_fuel_surface_temperature > _max_coolant_temperature)
			_max_coolant_temperature = _fuel_surface_temperature;
	}end_c_loop(c,t)

	return _max_coolant_temperature;
}

real _max_fuel_temperature_calc()
{

	return 0;
}


real _coolant_HeatCapacity(real T)
{
	real HeatCapacity;
	T = T-273.15;         /* Transfer to Celsius Degree */
	HeatCapacity = 2414.0;
	return HeatCapacity;  /* The unit is J*kg^-1*K^-1 */
}

real _coolant_Density(real T)
{
	real Density;
	T = T-273.15;         /* Transfer to Celsius Degree */
	Density = 2413.-0.4884*T;
	return Density;       /* The unit is kg*m^-3 */
}

real _coolant_Viscosity(real T)
{
	real Viscosity;
	Viscosity = 1.16e-4*exp(3755.0/T);
	return Viscosity;     /* The unit is Pa*s */
}

real _coolant_Conductivity(real T)
{
	real Conductivity;
	T = T-273.15;         /* Transfer to Celsius Degree */
	Conductivity = 1.0;
	return Conductivity;  /* The unit is W*m^-1*K^-1 */
}

real _graphite_Conductivity(real T)
{
	real Conductivity;
	real DOSIS;
	T = T-273.15;         /* Transfer to Celsius Degree */
	DOSIS = 1021.;
	Conductivity = (-0.3906e-4*T+0.06829)/(DOSIS+1.931e-4*T);
	Conductivity = Conductivity + 1.228e-4*T + 0.042;
	Conductivity = Conductivity * 1.2768e2;
	return Conductivity;
}

real _heat_transfer_coefficient_calc(real T, real v)
{
	real _Re;
	real _Pr;
	real _length;
	real _coefficient;
	real _capacity;
	real _density;
	real _viscosity;
	real _conductivity;

	_length = 0.06;
	_capacity     = _coolant_HeatCapacity(T);
	_density      = _coolant_Density(T);
	_viscosity    = _coolant_Viscosity(T);
	_conductivity = _coolant_Conductivity(T);

	_Re = _density*v*_length/_viscosity;
	_Pr = _viscosity*_capacity/_conductivity;

	_coefficient = (2+1.1*pow(_Re,0.6)*pow(_Pr,0.333))*_conductivity/_length;

	return _coefficient;
}





