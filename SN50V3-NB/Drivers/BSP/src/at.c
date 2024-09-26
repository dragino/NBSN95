#include "at.h"
#include "nbInit.h"
#include "tiny_sscanf.h"

static uint8_t  keep = 0;
static uint32_t general_parameters[32]={0};
static uint32_t password_parameters[3]={0};
static uint32_t servaddr_parameters[32]={0};
static uint32_t mqtt_parameters_client[32]={0};
static uint32_t mqtt_parameters_uname[32]={0};
static uint32_t mqtt_parameters_pwd[50]={0};
static uint32_t mqtt_parameters_pubtopic[32]={0};
static uint32_t mqtt_parameters_subtopic[32]={0};
static uint32_t coap_parameters1[32]={0};
static uint32_t coap_parameters2[32]={0};
static uint32_t coap_parameters3[32]={0};
static uint32_t coap_parameters4[32]={0};
static uint8_t  noud_flags = 0;
uint8_t  mqtt_qos_flags = 0;
uint8_t  qband_flag = 0;
uint8_t mqtt_qos=0;
extern char MCU_pwd[20];
uint8_t getsensorvalue_flag=0;
extern bool at_sleep_flag;
extern bool sleep_status;
extern bool first_sample;
ATEerror_t ATInsPro(char* atdata)
{
	uint8_t i = 0;
	rtrim(atdata);
	if(strcmp(atdata,AT) == 0)
		return AT_OK;
	else if(strcmp(atdata,AT"?") == 0)
	{
		return at_que(atdata);
	}
	
	while(strstr(atdata,ATCommand[i].string) == NULL)
	{
		i++;
		if(i>(sizeof(ATCommand) / sizeof(struct ATCommand_s))-1)
		{
			return AT_ERROR;
		}
	}

	if(strstr(atdata,"=?"))
	{	
		 return ATCommand[i].get(atdata);
	}
	else if(strstr(atdata,"="))
	{
		ATEerror_t AT_State = ATCommand[i].set(atdata);
		if(AT_State == AT_OK)
			config_Set();
		else
			return AT_State;
	}
	else if(strstr(atdata,"?"))
	{
		user_main_printf("%s",ATCommand[i].help_string);
	}
	else 
	{
		ATCommand[i].run(atdata);
	}
	return AT_OK;
}
void space_fun(char *str)
{
	char *str_c=str;
	int i,j=0;
	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]!=' ')
			str_c[j++]=str[i];
	}
	str_c[j]='\0';
	str=str_c;	
}

/************** 			AT			 **************/
ATEerror_t at_return_error(const char *param)
{
  return AT_ERROR;
}

ATEerror_t at_que(const char *param)
{
	user_main_printf("\r\nAT+<CMD>?        : Help on <CMD>\r\n"
              "AT+<CMD>         : Run <CMD>\r\n"
              "AT+<CMD>=<value> : Set the value\r\n"
              "AT+<CMD>=?       : Get the value\r\n");
	for (int i = 0; i < (sizeof(ATCommand) / sizeof(struct ATCommand_s)); i++)
	{
		printf("%s\r\n",ATCommand[i].help_string);
	}	
	
  return AT_OK;
}

/************** 			ATZ			 **************/
ATEerror_t at_reset_run(const char *param)
{
	NVIC_SystemReset();
  return AT_OK;
}

/************** 			AT+MODEL			 **************/
ATEerror_t at_model_get(const char *param)
{
	if(keep)
		printf(AT MODEL"=");
#ifdef NB_1D	
	#if defined NB_NS
	printf("SN50V3-NS-1D,%s\r\n",AT_VERSION_STRING);
	#else	
	printf("SN50V3-NB-1D,%s\r\n",AT_VERSION_STRING);
	#endif
#else	
	#if defined NB_NS
	printf("SN50V3-NS-GE,%s\r\n",AT_VERSION_STRING);
	#else	
	printf("SN50V3-NB-GE,%s\r\n",AT_VERSION_STRING);
	#endif	
	#endif
	return AT_OK;
}

/************** 			AT+CFGMOD			 **************/
ATEerror_t at_mod_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t mod = atoi((param+(pos-param)+1));

	if(mod == model1)
	{
		printf("\r\nDefault mode\r\n");
	}
	else if(mod == model2)
	{
		printf("\r\nDistance mode\r\n");
	}
	else if(mod == model3)
	{
		printf("\r\n3xADC + I2C mode\r\n");
	}
	else if(mod == model4)
	{
		printf("\r\n3xDS18B20 mode\r\n");
	}
	else if(mod == model5)
	{
		printf("\r\nWeiget mode\r\n");
	}
	else if(mod == model6)
	{
		printf("\r\nCounting mode\r\n");
	}	
	else if(mod == model7)
	{
		printf("\r\n3xADC + 3xDS18B20 mode\r\n");
	}
	else if(mod == model8)
	{
		printf("\r\nThree interrupt contact mode\r\n");
	}
	else if(mod == model9)
	{
		printf("\r\nCount+SHT31 mode\r\n");
	}
	else if(mod == model10)
	{
		printf("\r\nUse Sensor is TMP117\r\n");
	}	
	else
	{
		return AT_PARAM_ERROR;
	}
  printf("Attention:Take effect after ATZ\r\n");			
	sys.mod = mod;
  return AT_OK;
}

ATEerror_t at_mod_get(const char *param)
{
	if(keep)
		printf("AT+CFGMOD=");
	printf("%d\r\n",sys.mod);
	
  return AT_OK;
}

/************** 			AT+PWORD			 **************/
ATEerror_t at_pword_set(const char *param)
{
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >8)
	{		
		return AT_PARAM_ERROR;
	}
	
	if((strlen(param) - (pos-param)-1==1) && pos[1]=='0')
	{
		sys.pwd_flag=2;
	}
	else
	{
		sys.pwd_flag=1;
	}
	
	memset(sys.pwd,0,8);
	memcpy(sys.pwd,&pos[1],strlen(param) - (pos-param)-1);

	password_parameters[0]=sys.pwd[0]<<24 | sys.pwd[1]<<16 	| sys.pwd[2]<<8 | sys.pwd[3];
	password_parameters[1]=sys.pwd[4]<<24 | sys.pwd[5]<<16 	| sys.pwd[6]<<8 | sys.pwd[7];	
	
	FLASH_erase(FLASH_USER_START_PASSWORD,(FLASH_USER_END - FLASH_USER_START_PASSWORD) / FLASH_PAGE_SIZE);
	FLASH_program(FLASH_USER_START_PASSWORD,password_parameters, sizeof(password_parameters)/4);

  return AT_OK;
}

ATEerror_t at_pword_get(const char *param)
{	
	if(keep)
		printf("AT+PWORD=");

	printf("%s\r\n",(char*)sys.pwd);
	return AT_OK;
}

/************** 			AT+DEUI		 **************/
ATEerror_t at_deui_get(const char *param)
{
	if(keep)
		printf(AT DEUI"=");
	printf("%s\r\n",user.deui);
  return AT_OK;
}

ATEerror_t at_deui_set(const char *param)
{
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 !=15 || hexDetection((char*)(param+(pos-param)+1)) == 0)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.deui,0,sizeof(user.deui));
	memcpy(user.deui,(param+(pos-param)+1),strlen((param+(pos-param)+1)));	
	
  return AT_OK;
}

/************** 			AT+FDR		 **************/
ATEerror_t at_fdr_run(const char *param)
{
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_USER_START_FDR_FLAG,0x01);//store fdr_flag
	HAL_FLASHEx_DATAEEPROM_Lock();
	FLASH_erase(FLASH_USER_START_PASSWORD,(FLASH_USER_END - FLASH_USER_START_PASSWORD) / FLASH_PAGE_SIZE);
	FLASH_erase(FLASH_USER_START_ADDR_CONFIG,(FLASH_USER_END_ADDR - FLASH_USER_START_ADDR_CONFIG) / FLASH_PAGE_SIZE);	
	FLASH_erase(FLASH_USER_COAP_URI1,(FLASH_USER_COAP_END - FLASH_USER_COAP_URI1) / FLASH_PAGE_SIZE);	
	DatalogClear();
	memset(general_parameters,0,sizeof(general_parameters));	
	sys.clock_switch=1;
	sys.strat_time=65535;
	qband_flag=1;	
  general_parameters[11]=qband_flag<<24;
	general_parameters[29]=sys.clock_switch<<24 | sys.strat_time<<8;		
	FLASH_program(FLASH_USER_START_ADDR_CONFIG,general_parameters, sizeof(general_parameters)/4);		
	NVIC_SystemReset();
  return AT_OK;
}

/************** 			AT+FDR1		 **************/
ATEerror_t at_fdr1_run(const char *param)
{
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_USER_START_FDR_FLAG,0x01);//store fdr_flag
	HAL_FLASHEx_DATAEEPROM_Lock();
	FLASH_erase(FLASH_USER_START_ADDR_CONFIG,(FLASH_USER_END_ADDR - FLASH_USER_START_ADDR_CONFIG) / FLASH_PAGE_SIZE);		
	FLASH_erase(FLASH_USER_COAP_URI1,(FLASH_USER_COAP_END - FLASH_USER_COAP_URI1) / FLASH_PAGE_SIZE);		
	DatalogClear();
	memset(general_parameters,0,sizeof(general_parameters));	
	sys.clock_switch=1;
	sys.strat_time=65535;
	qband_flag=1;	
  general_parameters[11]=qband_flag<<24;
	general_parameters[29]=sys.clock_switch<<24 | sys.strat_time<<8;		
	FLASH_program(FLASH_USER_START_ADDR_CONFIG,general_parameters, sizeof(general_parameters)/4);			
	NVIC_SystemReset();
  return AT_OK;
}
/************** 			AT+CFG		 **************/
ATEerror_t at_cfg_run(const char *param)
{
	keep = 1;
	for (int i = 0; i < (sizeof(ATCommand) / sizeof(struct ATCommand_s)); i++)
	{
		ATCommand[i].get(param);
	}	
	keep = 0;
	
  return AT_OK;
}

/************** 			AT+SERVADDR		 **************/
ATEerror_t at_servaddr_get(const char *param)
{
	if(keep)
		printf("AT+SERVADDR=");
	printf("%s\r\n",user.add);
	if(strlen((char*)user.add_ip)!=0 && strstr((char*)user.add_ip,"NULL") == NULL)
		printf("(%s)",user.add_ip);
	
	printf("\r\n");
  return AT_OK;
}

ATEerror_t at_servaddr_set(const char *param)
{
	space_fun((char*)param);	
	char* pos = strchr(param,'=');
  if(strchr(param,',')==NULL)
  {
    return AT_PARAM_ERROR; 
  }	
	if(strlen(param) - (pos-param)-1 >69)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.add,0,sizeof(user.add));
	memcpy(user.add,(param+(pos-param)+1),strlen((param+(pos-param)+1)));	
	
  return AT_OK;
}

/************** 			AT+CLIENT		 **************/
ATEerror_t at_client_get(const char *param)
{
	if(keep)
		printf(AT CLIENT"=");

	printf("%s\r\n",user.client);
  return AT_OK;
}

ATEerror_t at_client_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >128)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.client,0,sizeof(user.client));
	memcpy(user.client,(param+(pos-param)+1),strlen((param+(pos-param)+1)));
	
  return AT_OK;
}
/************** 			AT+UNAME		 **************/
ATEerror_t at_uname_get(const char *param)
{
	if(keep)
		printf(AT UNAME"=");

	printf("%s\r\n",user.uname);
	
  return AT_OK;
}

ATEerror_t at_uname_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >128)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.uname,0,sizeof(user.uname));
	memcpy(user.uname,(param+(pos-param)+1),strlen((param+(pos-param)+1)));
	
  return AT_OK;
}
/************** 			AT+PWD		 **************/
ATEerror_t at_pwd_get(const char *param)
{
	if(keep)
		printf("AT+PWD=");

	printf("%s\r\n",user.pwd);
  return AT_OK;
}

ATEerror_t at_pwd_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >200)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.pwd,0,sizeof(user.pwd));
	memcpy(user.pwd,(param+(pos-param)+1),strlen((param+(pos-param)+1)));
  return AT_OK;
}
/************** 			AT+PUBTOPIC		 **************/
ATEerror_t at_pubtopic_get(const char *param)
{
	if(keep)
		printf(AT PUBTOPIC"=");

	printf("%s\r\n",user.pubtopic);
  return AT_OK;
}

ATEerror_t at_pubtopic_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >128)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.pubtopic,0,sizeof(user.pubtopic));
	memcpy(user.pubtopic,(param+(pos-param)+1),strlen((param+(pos-param)+1)));
  return AT_OK;
}
/************** 			AT+SUBTOPIC		 **************/
ATEerror_t at_subtopic_get(const char *param)
{
	if(keep)
		printf(AT SUBTOPIC"=");

	printf("%s\r\n",user.subtopic);
  return AT_OK;
}

ATEerror_t at_subtopic_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >128)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.subtopic,0,sizeof(user.subtopic));
	memcpy(user.subtopic,(param+(pos-param)+1),strlen((param+(pos-param)+1)));
  return AT_OK;
}
/************** 			AT+TDC		 **************/
ATEerror_t at_tdc_get(const char *param)
{
	if(keep)
		printf("AT+TDC=");
	printf("%d\r\n",sys.tdc);
  return AT_OK;
}

ATEerror_t at_tdc_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint32_t tdc = atoi((param+(pos-param)+1));
	if(tdc > 0xFFFFFF || tdc<60)
	{
		return AT_PARAM_ERROR;
	}
	sys.tdc = tdc;
  return AT_OK;
}

/************** 			AT+INMOD		 **************/
ATEerror_t at_inmod_get(const char *param)
{
	if(keep)
		printf("AT+INTMOD=");
	printf("1:%d,2:%d,3:%d\r\n",sys.inmod,sys.inmod_pa4,sys.inmod_pa0);	
	
  return AT_OK;
}

ATEerror_t at_inmod_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t aa;
  uint8_t bb,cc;	
	char *param_temp;
	uint8_t char_number=0;
  for(uint8_t t=(pos-param)+1;t<strlen(param);t++)
	{
		param_temp[char_number++]=param[t];
	}
	param_temp[char_number]='\0';
	
	if((tiny_sscanf(param_temp, "%d,%d,%d", &aa,&bb,&cc) != 3))
	{
		return AT_PARAM_ERROR;
	}

		sys.inmod=aa;
		sys.inmod_pa4=bb;
		sys.inmod_pa0=cc;			
	  EX_GPIO_Init(sys.inmod);
		EX_GPIO_Init_pa4(sys.inmod_pa4);
		EX_GPIO_Init_pa0(sys.inmod_pa0);
	
	return AT_OK;	
}

/************** 			AT+5VT		 **************/
ATEerror_t at_5vt_get(const char *param)
{
	if(keep)
		printf("AT+5VT=");
	printf("%d\r\n",sys.power_time);
  return AT_OK;
}

ATEerror_t at_5vt_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint16_t power_time = atoi((param+(pos-param)+1));
	if(power_time > 0xFFFF)
	{
		return AT_PARAM_ERROR;
	}
	sys.power_time = power_time;
  return AT_OK;
}

/************** 			AT+PRO		 **************/
ATEerror_t at_pro_get(const char *param)
{
	if(keep)
		printf(AT PRO"=");
	  printf("%d,%d\r\n",sys.protocol,sys.platform);	

  return AT_OK;
}

ATEerror_t at_pro_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t protocol = param[(pos-param)+1];
	uint8_t platform;
	protocol = protocol - 0x30;
	if(protocol != COAP_PRO &&protocol != UDP_PRO && protocol != MQTT_PRO && protocol != TCP_PRO)
	{
		return AT_PARAM_ERROR;
	}
  if(strchr(param,',')==NULL)
  {
    return AT_PARAM_ERROR; 
  }	
  platform =  atoi(param+(strchr(pos,',')-param)+1);
  
	if(protocol==UDP_PRO||protocol==TCP_PRO||protocol==COAP_PRO)
	{
	  if(platform==0 || platform==5)
		{}
	  else
			return AT_PARAM_ERROR;
	}
	sys.protocol = protocol;
	sys.platform =platform;
  printf("Attention:Take effect after ATZ\r\n");		
	
  return AT_OK;
}


/************** 			AT+RXDL		 **************/
ATEerror_t at_rxdl_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint16_t rxdl = atoi((param+(pos-param)+1));
	if(rxdl > 0xFFFF)
	{
		return AT_PARAM_ERROR;
	}
	sys.rxdl = rxdl;
  return AT_OK;
}
ATEerror_t at_rxdl_get(const char *param)
{
	if(keep)
		printf(AT RXDL"=");
	printf("%d\r\n",sys.rxdl);
  return AT_OK;
}

/************** 			AT+WEIGHT		**************/
ATEerror_t at_weight_reset(const char *param)
{
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_RESET);
	WEIGHT_SCK_Init();
	WEIGHT_DOUT_Init();
	Get_Maopi();
	HAL_Delay(500);
  Get_Maopi();
	WEIGHT_SCK_DeInit();
	WEIGHT_DOUT_DeInit();		
	HAL_GPIO_WritePin(Power_5v_GPIO_Port, Power_5v_Pin, GPIO_PIN_SET);
	return AT_OK;	
}

ATEerror_t at_weight_get(const char *param)
{
	if(!keep)
		printf("%0.1f g\r\n",(float)Get_Weight());
	
	return AT_OK;
}

ATEerror_t at_weight_GapValue_set(const char *param)
{
	uint16_t gapvalue_a = 0;
	uint8_t  gapvalue_b = 0;
	
	char* pos = strchr(param,'=');
	char* pos1 = strchr(param,'.');
	if(pos1 == NULL)
	{
		gapvalue_a = atoi((param+(pos-param)+1));
		gapvalue_b = 0;
	}
	else
	{
		char inter[10] = {0};
		for(int i=0;i<pos1-pos-1;i++)
			inter[i] = param[pos-param+i+1];		
		
		gapvalue_a = atoi(inter);
		gapvalue_b = atoi((param+(pos1-param)+1));
	}
	if(gapvalue_b>=10)
  {
    return AT_PARAM_ERROR;
  }	
	sensor.GapValue = gapvalue_a+((float)gapvalue_b/10.0);
	Get_Weight();
		
  return AT_OK;	
}

ATEerror_t at_weight_GapValue_get(const char *param)
{
	if(sensor.GapValue == 0)
	{
		sensor.GapValue = 400.0;
	}
	if(keep)
		printf("AT+WEIGRE=");
	printf("%0.1f\r\n",sensor.GapValue);
		
  return AT_OK;		
}

/************** 			AT+CDP		**************/
ATEerror_t at_cdp_run(const char *param)
{
	shtDataPrint();
	return AT_OK;
}

ATEerror_t at_cdp_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t cdp = param[(pos-param)+1];
	if(cdp != '0')
	{
		return AT_PARAM_ERROR;
	}
	shtDataClear();
	return AT_OK;
}

/************** 			AT+EXT		**************/
ATEerror_t at_ext_get(const char *param)
{
	if(keep)
		printf(AT EXT"=");
	printf("1:%d,2:%d,3:%d,%d\r\n",sensor.exit_count,sensor.exit_count_pa4,sensor.exit_count_pa0,sensor.count_mode);
  return AT_OK;
}

ATEerror_t at_ext_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint32_t aa,bb,cc;
	uint8_t dd;
	char *param_temp;
	uint8_t char_number=0;
  for(uint8_t t=(pos-param)+1;t<strlen(param);t++)
	{
		param_temp[char_number++]=param[t];
	}
	param_temp[char_number]='\0';
	
	if((tiny_sscanf(param_temp, "%d,%d,%d,%d", &aa,&bb,&cc,&dd) != 4))
	{
		return AT_PARAM_ERROR;
	}

		sensor.exit_count=aa;
		sensor.exit_count_pa4=bb;
		sensor.exit_count_pa0=cc;			
    sensor.count_mode=dd;	
	
	return AT_OK;
}

/************** 			AT+LDATA		**************/
ATEerror_t at_ldata_get(const char *param)
{
	if(keep)
		printf(AT LDATA"=");
	printf("%s\r\n",(strlen(sensor.data)==0)?"NULL":sensor.data);
	return AT_OK;
}
/************** 			AT+DNSCFG		**************/
ATEerror_t at_dnscfg_get(const char *param)
{
	if(keep)
		printf(AT DNSCFG"=");
	printf("%s\r\n",user.dns_add);
	return AT_OK;
}

ATEerror_t at_dnscfg_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >19)
	{
		return AT_PARAM_ERROR;
	}
	if(countchar(pos,',')!=1 || countchar(pos,'.')!=6)
	{
		return AT_PARAM_ERROR;
	}
	memset(user.dns_add,0,sizeof(user.dns_add));
	memcpy(user.dns_add,(param+(pos-param)+1),strlen((param+(pos-param)+1)));		
	return AT_OK;
}
/************** 			AT+APN		**************/
ATEerror_t at_apn_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >40)
	{
		return AT_PARAM_ERROR;
	}
	memset(user.apn,0,sizeof(user.apn));
	memcpy(user.apn,(param+(pos-param)+1),strlen((param+(pos-param)+1)));	
	return AT_OK;
}
ATEerror_t at_apn_get(const char *param)
{
	if(keep)
		printf(AT APN"=");
	printf("%s\r\n",user.apn);
	return AT_OK;
}
/************** 			AT+GETSENSORVALUE		 **************/
ATEerror_t at_getsensorvalue_set(const char *param)
{
		char* pos = strchr(param,'=');
	uint8_t getsensorvalue = param[(pos-param)+1];
 
	if(getsensorvalue == '0' )
	{	
		txPayLoadDeal2(&sensor);
	}
	else if(getsensorvalue == '1')
	{	
		getsensorvalue_flag=1;
		//task_num = _AT_CSQ;
		//NBTASK(&task_num);	
	}
	else
	{
	return AT_PARAM_ERROR;
	}	
  return AT_OK;
}

/************** 			AT+CSQTIME		 **************/
ATEerror_t at_csqtime_get(const char *param)
{
	if(keep)
		printf("AT+CSQTIME=");
	printf("%d\r\n",sys.csq_time);
  return AT_OK;
}

ATEerror_t at_csqtime_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint32_t tdc = atoi((param+(pos-param)+1));
	if(tdc > 10|| tdc<1)
	{
		return AT_PARAM_ERROR;
	}
	sys.csq_time = tdc;
  return AT_OK;
}
/************** 			AT+DNSTIMER		 **************/
ATEerror_t at_dnstimer_get(const char *param)
{
	if(keep)
		printf("AT+DNSTIMER=");
	printf("%d\r\n",sys.dns_time);
  return AT_OK;
}

ATEerror_t at_dnstimer_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint32_t tdc = atoi((param+(pos-param)+1));
	if(tdc > 48)
	{
		return AT_PARAM_ERROR;
	}
	
	if(tdc==0)
	sys.dns_timer = 0;
  else	
	sys.dns_timer = 1;	
 
	sys.dns_time = tdc;
  return AT_OK;
}

/************** 			AT+TLSMOD		 **************/
ATEerror_t at_tlsmod_get(const char *param)
{
	if(keep)
		printf(AT TLSMOD"=");
	printf("%d,%d\r\n",sys.tlsmod,sys.cert);	

  return AT_OK;
}

ATEerror_t at_tlsmod_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t tlsmod = param[(pos-param)+1];
	uint8_t cert;
	tlsmod = tlsmod - 0x30;
	if(tlsmod > 0x02)
	{
		return AT_PARAM_ERROR;
	}

  if(strchr(param,',')==NULL)
  {
    return AT_PARAM_ERROR; 
  }	
	cert =  atoi(param+(strchr(pos,',')-param)+1);
	
	sys.tlsmod = tlsmod;
	sys.cert =cert;
	
  return AT_OK;
}

/************** 			AT+SLEEP		 **************/
ATEerror_t at_sleep_get(const char *param)
{
	if(keep)
		printf(AT SLEEP"=");
	printf("%d\r\n",sleep_status);	

  return AT_OK;
}

ATEerror_t at_sleep_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint32_t sleep_tem = atoi((param+(pos-param)+1));
	if(sleep_tem > 1)
	{
		return AT_PARAM_ERROR;
	}
	if(sleep_tem==1)
	 at_sleep_flag= sleep_tem;
  else	
	 NVIC_SystemReset();	
	
  return AT_OK;
}
/************** 			AT+MQOS		**************/
ATEerror_t at_mqos_get(const char *param)
{
	if(keep)
		printf(AT MQOS"=");
	printf("%d\r\n",mqtt_qos);	
	return AT_OK;
}

ATEerror_t at_mqos_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t noud = atoi((param+(pos-param)+1));
	if(noud > 2)
	{
		return AT_PARAM_ERROR;
	}
	mqtt_qos = noud;
	mqtt_qos_flags=1;
	return AT_OK;
}
/************** 			AT+GETLOG		**************/
ATEerror_t at_getlog_run(const char *param)
{
	DatalogPrint();
	return AT_OK;
}

ATEerror_t at_getlog_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t cdp = param[(pos-param)+1];
	if(cdp != '0')
	{
		return AT_PARAM_ERROR;
	}
	DatalogClear();
	sys.log_seq=0;
	return AT_OK;
}

/************** 			AT+CLOCKLOG		 **************/
ATEerror_t at_clocklog_set(const char *param)
{
	char* pos = strchr(param,'=');
	bool aa;
  uint16_t bb;	
	uint8_t cc,dd;
	char *param_temp;
	uint8_t char_number=0;
  for(uint8_t t=(pos-param)+1;t<strlen(param);t++)
	{
		param_temp[char_number++]=param[t];
	}
	param_temp[char_number]='\0';
	
	if((tiny_sscanf(param_temp, "%d,%d,%d,%d", &aa,&bb,&cc,&dd) != 4))
	{
		return AT_PARAM_ERROR;
	}
	if(((bb<3600)||(bb==65535))&&(cc<=255)&&(dd<=32))
	{
		sys.clock_switch=aa;
		sys.strat_time=bb;
		sys.tr_time=cc;
		sys.sht_noud=dd;
		first_sample=0;
		noud_flags = 1;
		nb_cclk_run(NULL);
	}
  else
	{
		return AT_PARAM_ERROR;		
	}		
	
  return AT_OK;	
}
ATEerror_t at_clocklog_get(const char *param)
{
	if(keep)
		printf("AT+CLOCKLOG=");
	printf("%d,%d,%d,%d\r\n",sys.clock_switch,sys.strat_time,sys.tr_time,sys.sht_noud);
  return AT_OK;			
}

/************** 			AT+OPTION1		 **************/
ATEerror_t at_uri1_get(const char *param)
{
	if(keep)
		printf("AT+URI1=");
	printf("%s\r\n",user.uri1);
	
  return AT_OK;
}

ATEerror_t at_uri1_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >128)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.uri1,0,sizeof(user.uri1));
	memcpy(user.uri1,(param+(pos-param)+1),strlen((param+(pos-param)+1)));	
	
  return AT_OK;
}
/************** 			AT+OPTION2		 **************/
ATEerror_t at_uri2_get(const char *param)
{
	if(keep)
		printf("AT+URI2=");
	printf("%s\r\n",user.uri2);
	
  return AT_OK;
}

ATEerror_t at_uri2_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >128)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.uri2,0,sizeof(user.uri2));
	memcpy(user.uri2,(param+(pos-param)+1),strlen((param+(pos-param)+1)));	
	
  return AT_OK;
}
/************** 			AT+OPTION3		 **************/
ATEerror_t at_uri3_get(const char *param)
{
	if(keep)
		printf("AT+URI3=");
	printf("%s\r\n",user.uri3);
	
  return AT_OK;
}

ATEerror_t at_uri3_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >128)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.uri3,0,sizeof(user.uri3));
	memcpy(user.uri3,(param+(pos-param)+1),strlen((param+(pos-param)+1)));	
	
  return AT_OK;
}
/************** 			AT+OPTION4		 **************/
ATEerror_t at_uri4_get(const char *param)
{
	if(keep)
		printf("AT+URI4=");
	printf("%s\r\n",user.uri4);
	
  return AT_OK;
}

ATEerror_t at_uri4_set(const char *param)
{
	space_fun((char*)param);
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >128)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.uri4,0,sizeof(user.uri4));
	memcpy(user.uri4,(param+(pos-param)+1),strlen((param+(pos-param)+1)));	
	
  return AT_OK;
}

/************** 		Other		 **************/
char *rtrim(char* str)
{
	for(int i=0;i<strlen(str);i++)
	{
		if(str[i]=='\r' || str[i]=='\n')
			str[i] = 0;
	} 
	return str;
}

uint8_t hexDetection(char* str)
{
	for(int i=0;i<strlen(str);i++)
	{
		if(isxdigit(str[i]) == 0)
			return 0;
	}
	return 1;
}
/************** 			Read and write and storage		 **************/
void config_Set(void)
{
	memset(general_parameters,0,sizeof(general_parameters));
	memset(servaddr_parameters,0,sizeof(servaddr_parameters));
	memset(mqtt_parameters_uname,0,sizeof(mqtt_parameters_uname));
	memset(mqtt_parameters_pwd,0,sizeof(mqtt_parameters_pwd));
	memset(mqtt_parameters_client,0,sizeof(mqtt_parameters_client));
	memset(mqtt_parameters_pubtopic,0,sizeof(mqtt_parameters_pubtopic));
	memset(mqtt_parameters_subtopic,0,sizeof(mqtt_parameters_subtopic));
	memset(coap_parameters1,0,sizeof(coap_parameters1));	
	memset(coap_parameters2,0,sizeof(coap_parameters2));
	memset(coap_parameters3,0,sizeof(coap_parameters3));
	memset(coap_parameters4,0,sizeof(coap_parameters4));	
	
	general_parameters[0]=sensor.exit_count_pa4;
	general_parameters[1]=sensor.count_mode<<16 |sys.inmod_pa4<<8 | sys.inmod_pa0;
	general_parameters[2]=sys.mod<<24    | sys.tdc;
	general_parameters[3]=sys.inmod<<24  | sys.protocol<<16 | sys.csq_time;
	general_parameters[4]=sys.rxdl<<16   | sys.power_time;
	general_parameters[5]=(int)(sensor.GapValue *10000);
	general_parameters[6]=sensor.exit_count;
	general_parameters[11]=qband_flag<<24  |sys.tr_time<<16 | noud_flags<<8 | sys.sht_noud;
	general_parameters[12]=sys.platform<<24 |sys.dns_timer<<16 |sys.dns_time<<8;
	general_parameters[28]=mqtt_qos_flags<<24 |mqtt_qos <<16 |sys.cert<<8 |sys.tlsmod;
	general_parameters[29]=sys.clock_switch<<24 | sys.strat_time<<8 |sys.log_seq;	
	general_parameters[30]=sensor.exit_count_pa0;
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.deui);i=i+4,j++)
			general_parameters[7+j]=user.deui[i+0]<<24 | user.deui[i+1]<<16 | user.deui[i+2]<<8 | user.deui[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.apn);i=i+4,j++)
			general_parameters[18+j]=user.apn[i+0]<<24 | user.apn[i+1]<<16 | user.apn[i+2]<<8 | user.apn[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.dns_add);i=i+4,j++)
			general_parameters[13+j]=user.dns_add[i+0]<<24 | user.dns_add[i+1]<<16 | user.dns_add[i+2]<<8 | user.dns_add[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.add);i=i+4,j++)
			servaddr_parameters[0+j]=user.add[i+0]<<24 | user.add[i+1]<<16 | user.add[i+2]<<8 | user.add[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.uname);i=i+4,j++)
			mqtt_parameters_uname[j]=user.uname[i+0]<<24 | user.uname[i+1]<<16 | user.uname[i+2]<<8 | user.uname[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.pwd);i=i+4,j++)
			mqtt_parameters_pwd[j]=user.pwd[i+0]<<24 | user.pwd[i+1]<<16 | user.pwd[i+2]<<8 | user.pwd[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.client);i=i+4,j++)
			mqtt_parameters_client[j]=user.client[i+0]<<24 | user.client[i+1]<<16 | user.client[i+2]<<8 | user.client[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.pubtopic);i=i+4,j++)
			mqtt_parameters_pubtopic[j]=user.pubtopic[i+0]<<24 | user.pubtopic[i+1]<<16 | user.pubtopic[i+2]<<8 | user.pubtopic[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.subtopic);i=i+4,j++)
			mqtt_parameters_subtopic[j]=user.subtopic[i+0]<<24 | user.subtopic[i+1]<<16 | user.subtopic[i+2]<<8 | user.subtopic[i+3];
			
	for(uint8_t i=0,j=0;i<strlen((char*)user.uri1);i=i+4,j++)
			coap_parameters1[j]=user.uri1[i+0]<<24 | user.uri1[i+1]<<16 | user.uri1[i+2]<<8 | user.uri1[i+3];
			
	for(uint8_t i=0,j=0;i<strlen((char*)user.uri2);i=i+4,j++)
			coap_parameters2[j]=user.uri2[i+0]<<24 | user.uri2[i+1]<<16 | user.uri2[i+2]<<8 | user.uri2[i+3];

	for(uint8_t i=0,j=0;i<strlen((char*)user.uri3);i=i+4,j++)
			coap_parameters3[j]=user.uri3[i+0]<<24 | user.uri3[i+1]<<16 | user.uri3[i+2]<<8 | user.uri3[i+3];

	for(uint8_t i=0,j=0;i<strlen((char*)user.uri4);i=i+4,j++)
			coap_parameters4[j]=user.uri4[i+0]<<24 | user.uri4[i+1]<<16 | user.uri4[i+2]<<8 | user.uri4[i+3];

	FLASH_erase(FLASH_USER_START_ADDR_CONFIG,(FLASH_USER_END_ADDR - FLASH_USER_START_ADDR_CONFIG) / FLASH_PAGE_SIZE);
	FLASH_erase(FLASH_USER_COAP_URI1,(FLASH_USER_COAP_END - FLASH_USER_COAP_URI1) / FLASH_PAGE_SIZE);	
	FLASH_program(FLASH_USER_START_ADDR_CONFIG,general_parameters, sizeof(general_parameters)/4);
	FLASH_program(FLASH_USER_START_SERVADDR_ADD,servaddr_parameters, sizeof(servaddr_parameters)/4);
//	FLASH_program(FLASH_USER_START_COAP,coap_parameters, sizeof(coap_parameters)/4);
	FLASH_program(FLASH_USER_START_MQTT_CLIENT,mqtt_parameters_client, sizeof(mqtt_parameters_client)/4);
	FLASH_program(FLASH_USER_START_MQTT_UNAME,mqtt_parameters_uname, sizeof(mqtt_parameters_uname)/4);
	FLASH_program(FLASH_USER_START_MQTT_PWD,mqtt_parameters_pwd, sizeof(mqtt_parameters_pwd)/4);
	FLASH_program(FLASH_USER_START_MQTT_PUBTOPIC,mqtt_parameters_pubtopic, sizeof(mqtt_parameters_pubtopic)/4);
	FLASH_program(FLASH_USER_START_MQTT_SUBTOPIC,mqtt_parameters_subtopic, sizeof(mqtt_parameters_subtopic)/4);
	FLASH_program(FLASH_USER_COAP_URI1,coap_parameters1, sizeof(coap_parameters1)/4);
	FLASH_program(FLASH_USER_COAP_URI2,coap_parameters2, sizeof(coap_parameters2)/4);
	FLASH_program(FLASH_USER_COAP_URI3,coap_parameters3, sizeof(coap_parameters3)/4);		
	FLASH_program(FLASH_USER_COAP_URI4,coap_parameters4, sizeof(coap_parameters4)/4);			
}

void config_Get(void)
{
	uint32_t add = FLASH_USER_START_PASSWORD;
	for(uint8_t i=0,j=0;i<2;i++,j=j+4)
	{
		uint32_t temp  = FLASH_read(add+i*4);
		sys.pwd[j] 	 = (temp>>24) & 0x000000FF;
		sys.pwd[j+1] 	 = (temp>>16) & 0x000000FF;
		sys.pwd[j+2] 	 = (temp>>8)  & 0x000000FF;
		sys.pwd[j+3] 		 = (temp)     & 0x000000FF;
	}
	if(strlen((char*)sys.pwd) == 0)
	{
		memcpy(sys.pwd,MCU_pwd,6);
		sys.pwd_flag = 0;
	}
	else if(strlen((char*)sys.pwd)==1 && strchr((char*)sys.pwd,'0'))
	{
		sys.pwd_flag = 2;
	}
  add = FLASH_USER_START_ADDR_CONFIG;	
	
	sensor.exit_count_pa4 = FLASH_read(add);
	sensor.exit_count_pa0 = FLASH_read(add+120);
	
	sensor.count_mode= FLASH_read(add+4)>>16& 0x000000FF;
	
	sys.inmod_pa4= FLASH_read(add+4)>>8& 0x000000FF;
	if(sys.inmod_pa4 != 0 && sys.inmod_pa4 != 1 && sys.inmod_pa4 != 2 && sys.inmod_pa4 != 3)
		sys.inmod_pa4 = 0;
	EX_GPIO_Init_pa4(sys.inmod_pa4);

	sys.inmod_pa0= FLASH_read(add+4)& 0x000000FF;
	if(sys.inmod_pa0 != 0 && sys.inmod_pa0 != 1 && sys.inmod_pa0 != 2 && sys.inmod_pa0 != 3)
		sys.inmod_pa0 = 0;
	EX_GPIO_Init_pa0(sys.inmod_pa0);
	
	sys.mod = FLASH_read(add+8) >>24;
	if(sys.mod == 0 || sys.mod > model10)
		sys.mod = model1;
	
	sys.tdc = FLASH_read(add+8)  & 0x00FFFFFF;
	if(sys.tdc == 0)
		sys.tdc = 7200;
	
	sys.inmod = FLASH_read(add+12)>>24;
	if(sys.inmod != 0 && sys.inmod != 1 && sys.inmod != 2 && sys.inmod != 3)
		sys.inmod = 0;
	EX_GPIO_Init(sys.inmod);
	
	sys.protocol = FLASH_read(add+12)>>16  & 0x000000FF;
	if(sys.protocol != COAP_PRO &&sys.protocol != UDP_PRO && sys.protocol != MQTT_PRO && sys.protocol != TCP_PRO)
		sys.protocol = UDP_PRO;
	
	sys.power_time = FLASH_read(add+16)    & 0x0000FFFF;	
	sys.rxdl       = FLASH_read(add+16)>>16& 0x0000FFFF;
	
	sensor.GapValue = FLASH_read(add+20)/10000.0;
	if(sensor.GapValue == 0.0)
		sensor.GapValue = 400.0;
	
	sensor.exit_count = FLASH_read(add+24);
	
	qband_flag = (FLASH_read(add+44))>>24&0xFF;
	sys.tr_time = (FLASH_read(add+44))>>16&0xFF;
	if(sys.tr_time == 0)
		sys.tr_time = 15;

	noud_flags=FLASH_read(add+44)>>8&0xFF;
	
	sys.sht_noud=FLASH_read(add+44)&0xFF;
	if((sys.sht_noud==0)&&(noud_flags==0))
		sys.sht_noud = 8;
	
	sys.csq_time = FLASH_read(add+12) &0xFF;
	if(sys.csq_time==0)
		sys.csq_time = 5;

  sys.dns_time = FLASH_read(add+48)>>8 &0xFF;	
	
  sys.dns_timer	= FLASH_read(add+48)>>16 &0xFF;	
	
  sys.tlsmod = FLASH_read(add+112) &0xFF;	
	
  sys.cert	= FLASH_read(add+112)>>8 &0xFF;	
  
	mqtt_qos_flags = FLASH_read(add+112)>>24 &0xFF;	
	
  mqtt_qos  = FLASH_read(add+112)>>16 &0xFF;	
	if((mqtt_qos==0)&&(mqtt_qos_flags==0))
		mqtt_qos = 2;  
	
	 sys.log_seq = FLASH_read(add+116) &0xFF;	

	sys.clock_switch = FLASH_read(add+116)>>24 &0xFF;
	
	sys.strat_time = FLASH_read(add+116)>>8 &0xFFFF;
	
  sys.platform	= FLASH_read(add+48)>>24 &0xFF;	
	
	add = add+28;
	for(uint8_t i=0,j=0;i<4;i++,j=j+4)
	{
		uint32_t temp  = FLASH_read(add+i*4);
		user.deui[j] 	 = (temp>>24) & 0x000000FF;
		user.deui[j+1] = (temp>>16) & 0x000000FF;
		user.deui[j+2] = (temp>>8)  & 0x000000FF;
		user.deui[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.deui) == 0)
	{
		sprintf((char*)user.deui, "%s", "NULL");
	}		
	
	add = FLASH_USER_START_ADDR_CONFIG + 0x04*18;
	for(uint8_t i=0,j=0;i<10;i++,j=j+4)
	{
		uint32_t temp  = FLASH_read(add+i*4);
		user.apn[j] 	 = (temp>>24) & 0x000000FF;
		user.apn[j+1] = (temp>>16) & 0x000000FF;
		user.apn[j+2] = (temp>>8)  & 0x000000FF;
		user.apn[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.apn) == 0)
	{
		#ifdef NB_1D
		sprintf((char*)user.apn, "%s", "iot.1nce.net");
		#else
		sprintf((char*)user.apn, "%s", "NULL");
		#endif
	}
	
		add = FLASH_USER_START_ADDR_CONFIG + 0x04*13;
	for(uint8_t i=0,j=0;i<5;i++,j=j+4)
	{
		uint32_t temp  = FLASH_read(add+i*4);
		user.dns_add[j] 	 = (temp>>24) & 0x000000FF;
		user.dns_add[j+1] = (temp>>16) & 0x000000FF;
		user.dns_add[j+2] = (temp>>8)  & 0x000000FF;
		user.dns_add[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.dns_add) == 0)
	{
		sprintf((char*)user.dns_add, "%s", "\"8.8.8.8\",\"8.8.4.4\"");
	}
	
	add = FLASH_USER_START_SERVADDR_ADD;
	for(uint8_t i=0,j=0;i<18;i++,j=j+4)
	{
		uint32_t temp = FLASH_read(add+i*4);
		user.add[j] 	= (temp>>24) & 0x000000FF;
		user.add[j+1] = (temp>>16) & 0x000000FF;
		user.add[j+2] = (temp>>8)  & 0x000000FF;
		user.add[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.add) == 0)
	{
		#ifdef NB_1D
		sprintf((char*)user.add, "%s", "67.207.76.90,4445");
		#else
		sprintf((char*)user.add, "%s", "NULL");
		#endif
	}
	if(sys.protocol == MQTT_PRO)
	{
		if(sys.platform==1)
		sprintf((char*)user.add, "%s", "mqtt3.thingspeak.com,1883");		
		else if(sys.platform==2)
		{
			if(sys.tlsmod==1)
		   sprintf((char*)user.add, "%s", "mqtt.datacake.co,8883");	
			else
		   sprintf((char*)user.add, "%s", "mqtt.datacake.co,1883");				
		}
		else if(sys.platform==3)
		sprintf((char*)user.add, "%s", "broker.hivemq.com,1883");			
	}
	//mqtt-uname
	add = FLASH_USER_START_MQTT_UNAME;
	for(uint8_t i=0,j=0;i<32;i++,j=j+4)
	{
		uint32_t temp   = FLASH_read(add+i*4);
		user.uname[j] 	= (temp>>24) & 0x000000FF;
		user.uname[j+1] = (temp>>16) & 0x000000FF;
		user.uname[j+2] = (temp>>8)  & 0x000000FF;
		user.uname[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.uname) == 0)
	{
		sprintf((char*)user.uname, "%s", "NULL");
	}
	
	//mqtt-pwd
	add = FLASH_USER_START_MQTT_PWD;
	for(uint8_t i=0,j=0;i<50;i++,j=j+4)
	{
		uint32_t temp = FLASH_read(add+i*4);
		user.pwd[j] 	= (temp>>24) & 0x000000FF;
		user.pwd[j+1] = (temp>>16) & 0x000000FF;
		user.pwd[j+2] = (temp>>8)  & 0x000000FF;
		user.pwd[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.pwd) == 0)
	{
		sprintf((char*)user.pwd, "%s", "NULL");
	}

	//mqtt-client
	add = FLASH_USER_START_MQTT_CLIENT;
	for(uint8_t i=0,j=0;i<32;i++,j=j+4)
	{
		uint32_t temp    = FLASH_read(add+i*4);		
		user.client[j] 	 = (temp>>24) & 0x000000FF;
		user.client[j+1] = (temp>>16) & 0x000000FF;
		user.client[j+2] = (temp>>8)  & 0x000000FF;
		user.client[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.client) == 0)
	{
		sprintf((char*)user.client, "%s", "NULL");
	}
	//mqtt-pubtopic
	add = FLASH_USER_START_MQTT_PUBTOPIC;
	for(uint8_t i=0,j=0;i<32;i++,j=j+4)
	{
		uint32_t temp      = FLASH_read(add+i*4);		
		user.pubtopic[j] 	 = (temp>>24) & 0x000000FF;
		user.pubtopic[j+1] = (temp>>16) & 0x000000FF;
		user.pubtopic[j+2] = (temp>>8)  & 0x000000FF;
		user.pubtopic[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.pubtopic) == 0)
	{
		sprintf((char*)user.pubtopic, "%s", "NULL");
	}
	//mqtt-subtopic
	add = FLASH_USER_START_MQTT_SUBTOPIC;
	for(uint8_t i=0,j=0;i<32;i++,j=j+4)
	{
		uint32_t temp      = FLASH_read(add+i*4);		
		user.subtopic[j] 	 = (temp>>24) & 0x000000FF;
		user.subtopic[j+1] = (temp>>16) & 0x000000FF;
		user.subtopic[j+2] = (temp>>8)  & 0x000000FF;
		user.subtopic[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.subtopic) == 0)
	{
		sprintf((char*)user.subtopic, "%s", "NULL");
	}
	//coap-option
	add = FLASH_USER_COAP_URI1;
	for(uint8_t i=0,j=0;i<32;i++,j=j+4)
	{
		uint32_t temp = FLASH_read(add+i*4);
		user.uri1[j] 	= (temp>>24) & 0x000000FF;
		user.uri1[j+1] = (temp>>16) & 0x000000FF;
		user.uri1[j+2] = (temp>>8)  & 0x000000FF;
		user.uri1[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.uri1) == 0)
	{
		sprintf((char*)user.uri1, "%s", "NULL");
	}
	add = FLASH_USER_COAP_URI2;
	for(uint8_t i=0,j=0;i<32;i++,j=j+4)
	{
		uint32_t temp = FLASH_read(add+i*4);
		user.uri2[j] 	= (temp>>24) & 0x000000FF;
		user.uri2[j+1] = (temp>>16) & 0x000000FF;
		user.uri2[j+2] = (temp>>8)  & 0x000000FF;
		user.uri2[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.uri2) == 0)
	{
		sprintf((char*)user.uri2, "%s", "NULL");
	}	
	add = FLASH_USER_COAP_URI3;
	for(uint8_t i=0,j=0;i<32;i++,j=j+4)
	{
		uint32_t temp = FLASH_read(add+i*4);
		user.uri3[j] 	= (temp>>24) & 0x000000FF;
		user.uri3[j+1] = (temp>>16) & 0x000000FF;
		user.uri3[j+2] = (temp>>8)  & 0x000000FF;
		user.uri3[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.uri3) == 0)
	{
		sprintf((char*)user.uri3, "%s", "NULL");
	}	
	add = FLASH_USER_COAP_URI4;
	for(uint8_t i=0,j=0;i<32;i++,j=j+4)
	{
		uint32_t temp = FLASH_read(add+i*4);
		user.uri4[j] 	= (temp>>24) & 0x000000FF;
		user.uri4[j+1] = (temp>>16) & 0x000000FF;
		user.uri4[j+2] = (temp>>8)  & 0x000000FF;
		user.uri4[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.uri4) == 0)
	{
		sprintf((char*)user.uri4, "%s", "NULL");
	}	
}
