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
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
		 * Defining the total_power variable in FLUENT console
		 * (rp-var-define 'total_power 10 'real #f)
		 *
		 * Changing the total_power variable in FLUENT console
		 * (rpsetvar 'total_power 12)
		 *
		 * Accessing the total_power variable in FLUENT console
		 * (%rpgetvar 'total_power)
		 *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		Message("The variable total_power is already defined as %5.2fMW\n",\
						RP_Get_Real("total_power"));
		_powerLevel = RP_Get_Real("total_power");
	}
	else
	{
		Message("The variable total_power is not defined !\n");
		Message("So the default power Level is defined as 2MW\n");
		_powerLevel = 2.0;
		/* The default power is 2MW. */
	}

	_power_initialization(d, _powerLevel);
}

DEFINE_ADJUST(myAdjust,d)
{

	_max_coolant_temperature = _max_coolant_temperature_calc(d);
	_max_fuel_temperature    = _max_fuel_temperature_calc();

	Message("The max coolant temperature is %f\n",_max_coolant_temperature);
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

	Message("Total_Volume=%f\nTotal_PowerPerVolume=%f\nTotal_Power=%f\n\n",\
			testValue,testValue1,testValue2);

	if(RP_Variable_Exists_P("gejian"))
	{
		Message("%f\n",RP_Get_Real("gejian"));
	}
	else
	{
		Message("Variable gejian is not exist !\n");
	}

}













