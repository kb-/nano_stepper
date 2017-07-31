/**********************************************************************
 *      Author: tstern
 *
 *	Misfit Tech invests time and resources providing this open source code,
 *	please support Misfit Tech and open-source hardware by purchasing
 *	products from Misfit Tech, www.misifittech.net!
 *
 *	Written by Trampas Stern  for Misfit Tech.
 *	BSD license, check license.txt for more information
 *	All text above, must be included in any redistribution
 *********************************************************************/
#include "nonvolatile.h"
#include "Flash.h"  //thanks to Kent Larsen for pointing out the lower case error
#include <Arduino.h>




//we use this so we can hard code calibration table
// be sure to set the last word as status flag
// this save time calibrating each time we do a code build
#ifdef NZS_FAST_CAL
__attribute__((__aligned__(FLASH_ROW_SIZE))) const uint16_t NVM_flash[16640]={  //allocates 33280 bytes
#else
__attribute__((__aligned__(FLASH_ROW_SIZE))) const uint16_t NVM_flash[256]={  //allocates 512 bytes
#endif

#if NZS_LABEL == 'Y'
    		29516,29860,30197,30536,30869,31202,31528,31856,32180,32509,32835,33162,33492,33826,34157,34493,34829,35168,35507,35847,36185,36528,36864,37205,37538,37873,38198,38524,38844,39169,39486,39804,40116,40439,40753,41071,41387,41710,42029,42352,42674,43001,43324,43651,43979,44311,44638,44966,45293,45625,45954,46280,46607,46936,47259,47587,47912,48242,48565,48889,49213,49546,49873,50201,50531,50867,51199,51535,51868,52207,52539,52874,53203,53535,53860,54186,54506,54828,55145,55462,55776,56093,56407,56720,57036,57356,57669,57991,58309,58636,58959,59287,59610,59942,60265,60589,60913,61240,61557,61880,62200,62525,62842,63166,63494,63825,64153,64485,64818,65156,65488,290,623,959,1285,1611,1934,2260,2579,2898,3221,3547,3870,4196,4523,4855,5187,5517,5845,6181,6508,6835,7159,7489,7812,8133,8455,8782,9103,9426,9750,10078,10402,10725,11053,11384,11713,12045,12377,12715,13053,13392,13729,14069,14404,14738,15066,15398,15722,16045,16369,16693,17014,17338,17660,17988,18314,18640,18964,19293,19615,19942,20264,20591,20916,21243,21573,21907,22234,22568,22899,23231,23559,23887,24211,24537,24858,25180,25502,25826,26150,26474,26802,27136,27468,27802,28143,28487,28826,29172,
#endif 
        //insert the getcal command calibration data here
		0xFFFF
};



static_assert (sizeof(nvm_t)<sizeof(NVM_flash), "nvm_t structure larger than allocated memory");




//FLASH_ALLOCATE(NVM_flash, sizeof(nvm_t));


bool nvmWriteCalTable(void *ptrData, uint32_t size)
{
	bool x=true;
	flashWrite(&NVM->CalibrationTable,ptrData,size);
	return true;
}

bool nvmWrite_sPID(float Kp, float Ki, float Kd)
{
	PIDparams_t pid;

	pid.Kp=Kp;
	pid.Ki=Ki;
	pid.Kd=Kd;
	pid.parametersVaild=true;

	flashWrite((void *)&NVM->sPID,&pid,sizeof(pid));
	return true;
}

bool nvmWrite_vPID(float Kp, float Ki, float Kd)
{
	PIDparams_t pid;

	pid.Kp=Kp;
	pid.Ki=Ki;
	pid.Kd=Kd;
	pid.parametersVaild=true;

	flashWrite((void *)&NVM->vPID,&pid,sizeof(pid));
	return true;
}

bool nvmWrite_pPID(float Kp, float Ki, float Kd)
{
	PIDparams_t pid;

	pid.Kp=Kp;
	pid.Ki=Ki;
	pid.Kd=Kd;
	pid.parametersVaild=true;

	flashWrite((void *)&NVM->pPID,&pid,sizeof(pid));
	return true;
}

bool nvmWriteSystemParms(SystemParams_t &systemParams)
{
	systemParams.parametersVaild=true;

	flashWrite((void *)&NVM->SystemParams,&systemParams,sizeof(systemParams));
	return true;
}

bool nvmWriteMotorParms(MotorParams_t &motorParams)
{
	motorParams.parametersVaild=true;

	flashWrite((void *)&NVM->motorParams,&motorParams,sizeof(motorParams));
	return true;
}

bool nvmErase(void)
{
	bool data=false;
	uint16_t cs=0;

	flashWrite((void *)&NVM->CalibrationTable.status,&data,sizeof(data));
	flashWrite((void *)&NVM->sPID.parametersVaild ,&data,sizeof(data));
	flashWrite((void *)&NVM->vPID.parametersVaild ,&data,sizeof(data));
	flashWrite((void *)&NVM->pPID.parametersVaild ,&data,sizeof(data));
	flashWrite((void *)&NVM->motorParams.parametersVaild ,&data,sizeof(data));
	flashWrite((void *)&NVM->SystemParams.parametersVaild ,&data,sizeof(data));
#ifdef NZS_FAST_CAL
	flashWrite((void *)&NVM->FastCal.checkSum,&cs,sizeof(cs));
#endif
}

