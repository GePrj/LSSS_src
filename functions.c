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


real _max_coolant_temperature_calc()
{

}

real _max_fuel_temperature_calc()
{

}




