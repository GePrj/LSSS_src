#include "udf.h"
#include "math.h"
#include "mem.h"
#include "stdio.h"

#include "functions.h"

real _max_coolant_temperature;
real _max_fuel_temperature;


DEFINE_INIT(initialization,d)
{
	real _powerLevel;

	if(RP_Variable_Exists_P("total_power"))
	{
		Message("The variable total_power is already defined as %f\n",\
						RP_Get_Real("total_power"));
		_powerLevel = RP_Get_Real("total_power");
	}
	else
	{
		Message("The variable total_power is not defined !");
		Message("So the default power Level is defined as 2MW");
		_powerLevel = 2.0;
		/* The default power is 2MW. */
	}

	_power_initialization(d, _powerLevel);
}

DEFINE_ADJUST(myAdjust,d)
{

	_max_coolant_temperature = _max_coolant_temperature_calc();
	_max_fuel_temperature    = _max_fuel_temperature_calc();
}

DEFINE_SOURCE(energy_source,c,t,dS,eqn)
{
	return C_UDMI(c,t,0);
}

DEFINE_PROPERTY(viscosity,c,t)
{
	real T=C_T(c,t);
	real vis;
	vis=0.000116*exp(3755/T);
	return vis;
}


DEFINE_ON_DEMAND(hehe)
{
	Domain *domain;
	Thread *t;
	cell_t  c;
	int zone_ID;
	int i;
	real testValue;
	real testValue1;
	real testValue2;

	i = 0;
	testValue  = 0;
	testValue1 = 0;
	testValue2 = 0;

	domain=Get_Domain(1);
	zone_ID = 17;
	t = Lookup_Thread(domain, zone_ID);

	begin_c_loop(c,t)
	{
		testValue  += C_VOLUME(c,t);
		testValue1 += C_UDMI(c,t,0);
		testValue2 += C_VOLUME(c,t)*C_UDMI(c,t,0);
	}end_c_loop(c,t)

	Message("%f\n%f\n%f\n",testValue,testValue1,testValue2);

	if(RP_Variable_Exists_P("gejian"))
	{
		Message("%f\n",RP_Get_Real("gejian"));
	}
	else
	{
		Message("Variable gejian is not exist !\n");
	}

}













