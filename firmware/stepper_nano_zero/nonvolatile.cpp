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
    		41311,41634,41953,42274,42595,42924,43246,43576,43903,44240,44570,44903,45236,45576,45907,46239,46570,46902,47232,47559,47885,48216,48538,48866,49192,49519,49843,50171,50500,50836,51166,51502,51840,52185,52524,52862,53201,53540,53870,54201,54526,54852,55170,55488,55804,56123,56434,56751,57064,57383,57704,58022,58346,58672,59000,59325,59657,59991,60316,60648,60974,61302,61622,61943,62265,62586,62904,63224,63550,63876,64202,64531,64860,65195,65527,325,657,993,1320,1647,1973,2303,2629,2955,3284,3617,3945,4274,4606,4940,5268,5596,5919,6246,6566,6886,7205,7524,7839,8156,8472,8793,9110,9432,9753,10082,10405,10733,11059,11393,11720,12048,12378,12711,13040,13368,13698,14034,14365,14700,15032,15366,15698,16026,16355,16686,17010,17337,17661,17987,18309,18633,18953,19279,19601,19925,20248,20578,20902,21230,21561,21894,22224,22555,22884,23219,23544,23871,24193,24519,24838,25161,25482,25809,26127,26454,26778,27109,27437,27770,28106,28450,28787,29129,29469,29813,30147,30485,30814,31147,31472,31799,32123,32451,32774,33104,33433,33764,34096,34430,34761,35098,35431,35767,36101,36441,36776,37109,37441,37775,38104,38428,38756,39081,39400,39719,40036,40359,40674,40993,
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

