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
    		6779,7106,7428,7747,8067,8388,8703,9021,9343,9669,9991,10316,10642,10975,11306,11638,11973,12310,12646,12984,13315,13652,13983,14314,14642,14971,15297,15622,15947,16277,16602,16930,17258,17588,17916,18241,18569,18900,19227,19555,19882,20216,20543,20877,21210,21543,21872,22205,22533,22863,23185,23511,23830,24153,24470,24791,25107,25432,25752,26075,26399,26728,27057,27387,27721,28064,28402,28741,29083,29425,29759,30094,30425,30757,31080,31408,31730,32059,32381,32707,33033,33362,33693,34022,34355,34692,35027,35364,35698,36040,36377,36716,37049,37388,37717,38046,38367,38693,39009,39328,39641,39959,40274,40590,40905,41226,41542,41863,42183,42508,42830,43158,43482,43814,44138,44469,44795,45124,45447,45773,46096,46420,46742,47063,47388,47715,48043,48371,48699,49034,49362,49693,50025,50358,50687,51020,51352,51688,52018,52355,52690,53029,53362,53698,54029,54359,54686,55011,55330,55651,55967,56285,56598,56918,57232,57552,57872,58193,58515,58841,59164,59500,59828,60160,60489,60821,61149,61476,61800,62128,62451,62775,63100,63433,63760,64093,64425,64761,65092,65425,223,561,887,1215,1540,1868,2188,2512,2833,3163,3488,3816,4144,4480,4813,5146,5478,5811,6142,6469,
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

