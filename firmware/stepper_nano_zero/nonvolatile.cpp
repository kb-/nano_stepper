/**********************************************************************
	Copyright (C) 2018  MisfitTech LLC,  All rights reserved.

 	MisfitTech uses a dual license model that allows the software to be used under
	a standard GPL open source license, or a commercial license.  The standard GPL
	license  requires that all software statically linked with MisfitTec Code is
	also distributed under the same GPL V2 license terms.  Details of both license
	options follow:

	- Open source licensing -
	MisfitTech is a free download and may be used, modified, evaluated and
	distributed without charge provided the user adheres to version two of the GNU
	General Public License (GPL) and does not remove the copyright notice or this
	text.  The GPL V2 text is available on the gnu.org web site

	- Commercial licensing -
	Businesses and individuals that for commercial or other reasons cannot comply
	with the terms of the GPL V2 license must obtain a low cost commercial license
	before incorporating MisfitTech code into proprietary software for distribution in
	any form.  Commercial licenses can be purchased from www.misfittech.net
	and do not require any source files to be changed.


	This code is distributed in the hope that it will be useful.  You cannot
	use MisfitTech's code unless you agree that you use the software 'as is'.
	MisfitTech's code is provided WITHOUT ANY WARRANTY; without even the implied
	warranties of NON-INFRINGEMENT, MERCHANTABILITY or FITNESS FOR A PARTICULAR
	PURPOSE. MisfitTech LLC disclaims all conditions and terms, be they
	implied, expressed, or statutory.


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
//X 41026,41353,41677,41990,42317,42643,42969,43286,43610,43937,44264,44579,44909,45239,45567,45888,46219,46553,46884,47207,47542,47881,48215,48543,48878,49217,49555,49884,50223,50562,50901,51229,51562,51903,52236,52562,52894,53227,53556,53880,54208,54537,54863,55181,55505,55830,56152,56463,56782,57106,57425,57737,58056,58378,58699,59012,59329,59654,59974,60288,60611,60935,61260,61575,61900,62228,62553,62871,63200,63533,63862,64187,64519,64855,65189,65517,317,657,993,1321,1658,1999,2337,2669,3005,3347,3684,4016,4353,4693,5029,5358,5694,6035,6369,6696,7030,7364,7698,8023,8354,8690,9023,9346,9679,10012,10344,10666,10997,11329,11659,11980,12310,12642,12973,13295,13622,13956,14288,14611,14939,15275,15606,15929,16262,16599,16931,17258,17591,17928,18259,18587,18918,19254,19586,19908,20236,20569,20896,21215,21539,21868,22190,22505,22824,23147,23467,23778,24095,24413,24731,25038,25353,25671,25986,26292,26605,26924,27237,27543,27858,28179,28495,28803,29122,29446,29767,30082,30403,30735,31061,31380,31710,32047,32379,32707,33041,33379,33719,34047,34385,34729,35068,35400,35738,36080,36416,36748,37083,37421,37756,38083,38415,38750,39080,39403,39731,40062,40390,40709,
//Y 23602,23939,24273,24611,24944,25283,25615,25948,26279,26615,26942,27274,27599,27933,28259,28588,28911,29244,29566,29892,30215,30543,30867,31191,31514,31844,32166,32491,32816,33145,33471,33799,34127,34460,34785,35115,35446,35783,36112,36446,36777,37114,37445,37774,38103,38439,38766,39095,39423,39758,40085,40413,40739,41076,41399,41726,42050,42382,42704,43027,43350,43676,43996,44320,44640,44965,45284,45604,45923,46247,46566,46887,47205,47530,47849,48169,48490,48815,49138,49460,49780,50110,50430,50757,51077,51406,51727,52053,52378,52710,53032,53362,53688,54019,54345,54673,54999,55331,55656,55985,56307,56635,56958,57280,57600,57923,58243,58561,58878,59200,59516,59832,60145,60466,60780,61097,61412,61735,62050,62373,62691,63019,63337,63659,63979,64304,64625,64946,65270,63,383,710,1032,1363,1686,2013,2340,2677,3006,3341,3676,4017,4355,4695,5031,5378,5716,6056,6392,6735,7067,7403,7730,8070,8395,8727,9050,9385,9707,10036,10357,10688,11009,11335,11656,11988,12313,12641,12962,13299,13621,13946,14269,14597,14921,15248,15568,15901,16222,16554,16877,17210,17538,17867,18195,18532,18863,19199,19532,19874,20209,20551,20888,21230,21569,21907,22242,22585,22920,23259,

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

