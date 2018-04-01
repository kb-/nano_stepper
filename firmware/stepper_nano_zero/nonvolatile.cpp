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
    		41802,42135,42456,42781,43105,43434,43756,44081,44403,44729,45050,45371,45695,46018,46340,46661,46980,47306,47628,47950,48273,48600,48923,49248,49572,49899,50224,50545,50868,51194,51515,51838,52161,52487,52811,53133,53459,53789,54111,54437,54764,55093,55418,55744,56066,56391,56710,57037,57353,57676,57992,58313,58635,58954,59272,59589,59908,60227,60540,60860,61176,61497,61814,62133,62455,62779,63100,63421,63746,64073,64397,64720,65046,65375,162,488,812,1142,1468,1793,2123,2451,2781,3112,3440,3779,4114,4449,4786,5128,5467,5803,6140,6484,6815,7149,7478,7815,8140,8470,8795,9127,9452,9778,10101,10431,10755,11078,11401,11731,12058,12384,12712,13044,13371,13701,14025,14358,14684,15012,15335,15667,15992,16318,16644,16977,17304,17633,17959,18296,18626,18956,19289,19629,19961,20299,20636,20980,21317,21656,21992,22334,22671,23011,23343,23685,24018,24353,24687,25022,25355,25686,26014,26348,26676,27007,27334,27667,27994,28322,28653,28983,29308,29636,29965,30295,30620,30944,31270,31598,31923,32250,32575,32904,33231,33558,33887,34218,34547,34876,35205,35540,35869,36200,36531,36867,37198,37528,37858,38194,38523,38851,39180,39512,39841,40168,40494,40827,41153,41478,
#endif 

#if NZS_LABEL == 'X'
    		24010,24324,24642,24955,25275,25596,25915,26227,26547,26869,27189,27503,27826,28151,28473,28790,29119,29447,29773,30092,30423,30754,31083,31408,31742,32077,32410,32736,33073,33409,33747,34075,34413,34751,35091,35421,35761,36101,36439,36769,37105,37441,37776,38105,38438,38772,39102,39429,39765,40095,40425,40748,41084,41414,41751,42074,42408,42741,43075,43403,43739,44074,44408,44738,45073,45409,45743,46071,46407,46740,47076,47400,47737,48071,48399,48724,49057,49388,49717,50041,50370,50697,51024,51343,51670,51996,52320,52636,52961,53280,53600,53914,54236,54554,54871,55182,55501,55818,56136,56448,56767,57085,57403,57716,58037,58357,58677,58993,59315,59639,59962,60280,60606,60932,61258,61578,61905,62235,62564,62888,63215,63549,63877,64201,64535,64864,65197,65524,323,657,987,1313,1650,1983,2315,2639,2975,3306,3637,3963,4294,4624,4956,5280,5610,5942,6272,6595,6927,7257,7587,7909,8241,8572,8901,9225,9555,9888,10218,10544,10876,11207,11537,11864,12194,12527,12859,13188,13520,13852,14183,14511,14843,15176,15509,15836,16170,16504,16835,17162,17494,17828,18159,18485,18818,19151,19480,19805,20136,20465,20793,21113,21442,21770,22093,22410,22735,23057,23378,23693,
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

