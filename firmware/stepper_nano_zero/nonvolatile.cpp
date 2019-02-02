/**********************************************************************
 *      Author: tstern
 *
	Copyright (C) 2018  MisfitTech,  All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Written by Trampas Stern for MisfitTech.

    Misfit Tech invests time and resources providing this open source code,
    please support MisfitTech and open-source hardware by purchasing
	products from MisfitTech, www.misifittech.net!
 *********************************************************************/
#include "nonvolatile.h"
#include "Flash.h"  //thanks to Kent Larsen for pointing out the lower case error
#include <Arduino.h>




//we use this so we can hard code calibration table
// be sure to set the last word as status flag
// this save time calibrating each time we do a code build
#ifdef NZS_FAST_CAL
__attribute__((__aligned__(FLASH_ROW_SIZE))) const uint16_t NVM_flash[16767]={  //allocates 33280 bytes
#else
__attribute__((__aligned__(FLASH_ROW_SIZE))) const uint16_t NVM_flash[256]={  //allocates 512 bytes
#endif
//52944,53262,53582,53896,54217,54537,54854,55167,55485,55803,56121,56435,56754,57073,57390,57703,58025,58347,58666,58981,59306,59627,59952,60268,60594,60919,61245,61565,61893,62222,62547,62872,63201,63532,63862,64188,64517,64851,65181,65508,307,642,971,1297,1633,1965,2298,2624,2957,3290,3622,3947,4280,4613,4944,5269,5603,5933,6264,6590,6923,7254,7586,7909,8243,8576,8905,9232,9565,9897,10228,10553,10887,11219,11550,11876,12209,12542,12872,13200,13534,13866,14198,14522,14857,15190,15521,15847,16178,16512,16842,17166,17503,17835,18166,18489,18824,19154,19484,19808,20138,20467,20795,21115,21443,21770,22092,22410,22735,23056,23378,23691,24013,24330,24651,24961,25284,25603,25920,26233,26556,26875,27196,27510,27834,28153,28479,28794,29121,29449,29774,30095,30423,30754,31080,31405,31739,32071,32403,32730,33064,33402,33737,34068,34406,34743,35080,35410,35751,36088,36423,36755,37092,37428,37763,38093,38427,38761,39093,39421,39756,40089,40421,40747,41081,41414,41746,42075,42412,42744,43078,43407,43743,44078,44412,44742,45079,45414,45747,46076,46411,46745,47077,47403,47737,48069,48396,48724,49056,49386,49713,50036,50365,50693,51018,51336,51663,51988,52309,52627,

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

