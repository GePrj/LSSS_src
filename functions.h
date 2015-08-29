#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "udf.h"
#include "math.h"
#include "mem.h"
#include "stdio.h"

void _power_initialization(Domain *domain, real powerLevel);

void _set_power_for_each_cell(cell_t c, Thread *t);

void _power_density_correction(cell_t c, Thread *t, real powerLevel);


real _max_coolant_temperature_calc();

real _max_fuel_temperature_calc();


real _coolant_HeatCapacity(real T);
real _coolant_Density(real T);
real _coolant_Viscosity(real T);
real _coolant_Conductivity(real T);
real _graphite_Conductivity(real T);


#endif
