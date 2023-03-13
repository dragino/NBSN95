#include "at.h"

static uint8_t  keep = 0;
static uint32_t general_parameters[32]={0};
static uint32_t servaddr_parameters[32]={0};
static uint32_t coap_parameters[32]={0};
static uint32_t mqtt_parameters_basic[32]={0};
static uint32_t mqtt_parameters_topic[32]={0};

uint8_t getsensorvalue_flag=0;
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
		printf("AT+MODEL=");
	printf("NBSN95,%s\r\n",AT_VERSION_STRING);
	return AT_OK;
}

/************** 			AT+CFGMOD			 **************/
ATEerror_t at_mod_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t mod = param[(pos-param)+1];

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
	else
	{
		return AT_PARAM_ERROR;
	}
	sys.mod = mod;
  return AT_OK;
}

ATEerror_t at_mod_get(const char *param)
{
	if(keep)
		printf("AT+CFGMOD=");
	printf("%c\r\n",sys.mod);
	
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
	upLink_flash_clear(sys.list);
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD,EEPROM_USER_START_FDR_FLAG,0x01);//store fdr_flag
	HAL_FLASHEx_DATAEEPROM_Lock();
	FLASH_erase(FLASH_USER_START_ADDR_CONFIG,(FLASH_USER_END_ADDR - FLASH_USER_START_ADDR_CONFIG) / FLASH_PAGE_SIZE);
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
	if(strchr(param,':') != NULL) 
	{
		return AT_PARAM_ERROR;
	}
	
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >69)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.add,0,sizeof(user.add));
	memcpy(user.add,(param+(pos-param)+1),strlen((param+(pos-param)+1)));	
	
  return AT_OK;
}

/************** 			AT+URI		 **************/
ATEerror_t at_uri_get(const char *param)
{
	if(keep)
		printf("AT+URI=");
	printf("%s\r\n",user.uri);
	
  return AT_OK;
}

ATEerror_t at_uri_set(const char *param)
{
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >128)
	{
		return AT_PARAM_ERROR;
	}
	
	memset(user.uri,0,sizeof(user.uri));
	memcpy(user.uri,(param+(pos-param)+1),strlen((param+(pos-param)+1)));	
	
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
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >40)
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
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >40)
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
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >40)
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
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >64)
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
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >64)
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
	printf("%c\r\n",sys.inmod);
	
  return AT_OK;
}

ATEerror_t at_inmod_set(const char *param)
{
	char* pos = strchr(param,'=');
	sys.inmod = param[(pos-param)+1];
	if(sys.inmod != '0' && sys.inmod != '1' && sys.inmod != '2' && sys.inmod != '3')
	{
		return AT_PARAM_ERROR;
	}
	EX_GPIO_Init(sys.inmod-0x30);
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
	printf("%d\r\n",sys.protocol);
  return AT_OK;
}

ATEerror_t at_pro_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t protocol = param[(pos-param)+1];
	protocol = protocol - 0x30;
	if(protocol != COAP_PRO && protocol != UDP_PRO && protocol != MQTT_PRO && protocol != TCP_PRO)
	{
		return AT_PARAM_ERROR;
	}
	
	sys.protocol = protocol;
	
  return AT_OK;
}

/************** 			AT+CFM		 **************/
ATEerror_t at_cfm_get(const char *param)
{
	if(keep)
		printf(AT CFM"=");
	printf("%c\r\n",sys.cfm);
  return AT_OK;
}

ATEerror_t at_cfm_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t cfm = param[(pos-param)+1];
	if(cfm != '0' && cfm != '1')
	{
		return AT_PARAM_ERROR;
	}
	
	sys.cfm = cfm;
	
  return AT_OK;
}
/************** 			AT+CUM		 **************/
ATEerror_t at_cum_get(const char *param)
{
	if(keep)
		printf(AT CUM"=");
	printf("%c\r\n",sys.cum_flag);
  return AT_OK;
}

ATEerror_t at_cum_set(const char *param)
{
	char* pos = strchr(param,'=');
	uint8_t cum = param[(pos-param)+1];
	if(cum != '0' && cum != '1')
	{
		return AT_PARAM_ERROR;
	}
	
	sys.cum_flag = cum;
	
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
	WEIGHT_SCK_Init();
	WEIGHT_DOUT_Init();
	Get_Maopi();
  Get_Maopi();
	return AT_OK;	
}

ATEerror_t at_weight_get(const char *param)
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
	if(!keep)
		printf("%0.1f g\r\n",(float)Get_Weight());
	
  return AT_OK;		
}

/************** 			AT+CDP		**************/
ATEerror_t at_cdp_run(const char *param)
{
	printList(sys.list);
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
	upLink_flash_clear(sys.list);
	return AT_OK;
}

/************** 			AT+EXT		**************/
ATEerror_t at_ext_get(const char *param)
{
	if(keep)
		printf(AT EXT"=");
	printf("%d\r\n",sensor.exit_count);
  return AT_OK;
}

ATEerror_t at_ext_set(const char *param)
{
	char* pos = strchr(param,'=');
	sensor.exit_count = atoi((param+(pos-param)+1));
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

/************** 			AT+FBAND		**************/
ATEerror_t at_fband_get(const char *param)
{
	if(keep)
		printf(AT FBAND"=");
	printf("%c\r\n",sys.nband_flag);
	return AT_OK;
}

ATEerror_t at_fband_set(const char *param)
{
	char* pos = strchr(param,'=');
	if(pos[1]== '0' || pos[1]== '1')
		sys.nband_flag = pos[1];
	else
		return AT_PARAM_ERROR;
	
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
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >15)
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
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >20)
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
	
	general_parameters[0]=sys.pwd[0]<<24 | sys.pwd[1]<<16 	| sys.pwd[2]<<8 | sys.pwd[3];
	general_parameters[1]=sys.pwd[4]<<24 | sys.pwd[5]<<16 	| sys.pwd[6]<<8 | sys.pwd[7];
	general_parameters[2]=sys.mod<<24    | sys.tdc;
	general_parameters[3]=sys.inmod<<24  | sys.protocol<<16 | sys.cfm<<8 		| sys.cum_flag;
	general_parameters[4]=sys.rxdl<<16   | sys.power_time;
	general_parameters[5]=(int)(sensor.GapValue *10000);
	general_parameters[6]=sensor.exit_count;
	general_parameters[20]=sys.nband_flag<<24;
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.deui);i=i+4,j++)
			general_parameters[7+j]=user.deui[i+0]<<24 | user.deui[i+1]<<16 | user.deui[i+2]<<8 | user.deui[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.apn);i=i+4,j++)
			general_parameters[11+j]=user.apn[i+0]<<24 | user.apn[i+1]<<16 | user.apn[i+2]<<8 | user.apn[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.dns_add);i=i+4,j++)
			general_parameters[16+j]=user.dns_add[i+0]<<24 | user.dns_add[i+1]<<16 | user.dns_add[i+2]<<8 | user.dns_add[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.add);i=i+4,j++)
			servaddr_parameters[0+j]=user.add[i+0]<<24 | user.add[i+1]<<16 | user.add[i+2]<<8 | user.add[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.uri);i=i+4,j++)
			coap_parameters[j]=user.uri[i+0]<<24 | user.uri[i+1]<<16 | user.uri[i+2]<<8 | user.uri[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.uname);i=i+4,j++)
			mqtt_parameters_basic[j]=user.uname[i+0]<<24 | user.uname[i+1]<<16 | user.uname[i+2]<<8 | user.uname[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.pwd);i=i+4,j++)
			mqtt_parameters_basic[11+j]=user.pwd[i+0]<<24 | user.pwd[i+1]<<16 | user.pwd[i+2]<<8 | user.pwd[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.client);i=i+4,j++)
			mqtt_parameters_basic[22+j]=user.client[i+0]<<24 | user.client[i+1]<<16 | user.client[i+2]<<8 | user.client[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.pubtopic);i=i+4,j++)
			mqtt_parameters_topic[j]=user.pubtopic[i+0]<<24 | user.pubtopic[i+1]<<16 | user.pubtopic[i+2]<<8 | user.pubtopic[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.subtopic);i=i+4,j++)
			mqtt_parameters_topic[16+j]=user.subtopic[i+0]<<24 | user.subtopic[i+1]<<16 | user.subtopic[i+2]<<8 | user.subtopic[i+3];
	
	FLASH_erase(FLASH_USER_START_ADDR_CONFIG,(FLASH_USER_END_ADDR - FLASH_USER_START_ADDR_CONFIG) / FLASH_PAGE_SIZE);
	FLASH_program(FLASH_USER_START_ADDR_CONFIG,general_parameters, sizeof(general_parameters)/4);
	FLASH_program(FLASH_USER_START_SERVADDR_ADD,servaddr_parameters, sizeof(servaddr_parameters)/4);
	FLASH_program(FLASH_USER_START_COAP,coap_parameters, sizeof(coap_parameters)/4);
	FLASH_program(FLASH_USER_START_MQTT_BASIC,mqtt_parameters_basic, sizeof(mqtt_parameters_basic)/4);
	FLASH_program(FLASH_USER_START_MQTT_TOPIC,mqtt_parameters_topic, sizeof(mqtt_parameters_topic)/4);
}

void config_Get(void)
{
	uint32_t add = FLASH_USER_START_ADDR_CONFIG;
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
		memcpy(sys.pwd,"12345678",8);
		sys.pwd_flag = 0;
	}
	else if(strlen((char*)sys.pwd)==1 && strchr((char*)sys.pwd,'0'))
	{
		sys.pwd_flag = 2;
	}
	
	sys.mod = FLASH_read(add+8) >>24;
	if(sys.mod == 0 || sys.mod > model6)
		sys.mod = model1;
	
	sys.tdc = FLASH_read(add+8)  & 0x00FFFFFF;
	if(sys.tdc == 0)
		sys.tdc = 3600;
	
	sys.inmod = FLASH_read(add+12)>>24;
	if(sys.inmod != '0' && sys.inmod != '1' && sys.inmod != '2' && sys.inmod != '3')
		sys.inmod = '0';
	EX_GPIO_Init(sys.inmod-0x30);
	
	sys.protocol = FLASH_read(add+12)>>16  & 0x000000FF;
	if(sys.protocol != COAP_PRO && sys.protocol != UDP_PRO && sys.protocol != MQTT_PRO && sys.protocol != TCP_PRO)
		sys.protocol = UDP_PRO;
	
	sys.cfm = FLASH_read(add+12)>>8 & 0x000000FF;	
	if(sys.cfm != '0' && sys.cfm != '1')
		sys.cfm = '0';
	
	sys.cum_flag = FLASH_read(add+12) & 0x000000FF;	
	if(sys.cum_flag != '0' && sys.cum_flag != '1')
		sys.cum_flag = '0';
	
	sys.power_time = FLASH_read(add+16)    & 0x0000FFFF;	
	sys.rxdl       = FLASH_read(add+16)>>16& 0x0000FFFF;
	
	sensor.GapValue = FLASH_read(add+20)/10000.0;
	if(sensor.GapValue == 0.0)
		sensor.GapValue = 400.0;
	
	sensor.exit_count = FLASH_read(add+24);

	sys.nband_flag = FLASH_read(add+80)>>24 &0xFF;
	if(sys.nband_flag!='0' && sys.nband_flag!='1')
		sys.nband_flag = '1';
	
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

	add = FLASH_USER_START_ADDR_CONFIG + 0x04*11;
	for(uint8_t i=0,j=0;i<5;i++,j=j+4)
	{
		uint32_t temp  = FLASH_read(add+i*4);
		user.apn[j] 	 = (temp>>24) & 0x000000FF;
		user.apn[j+1] = (temp>>16) & 0x000000FF;
		user.apn[j+2] = (temp>>8)  & 0x000000FF;
		user.apn[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.apn) == 0)
	{
		sprintf((char*)user.apn, "%s", "NULL");
	}
	
		add = FLASH_USER_START_ADDR_CONFIG + 0x04*16;
	for(uint8_t i=0,j=0;i<4;i++,j=j+4)
	{
		uint32_t temp  = FLASH_read(add+i*4);
		user.dns_add[j] 	 = (temp>>24) & 0x000000FF;
		user.dns_add[j+1] = (temp>>16) & 0x000000FF;
		user.dns_add[j+2] = (temp>>8)  & 0x000000FF;
		user.dns_add[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.dns_add) == 0)
	{
		sprintf((char*)user.dns_add, "%s", "8.8.8.8,8.8.4.4");
	}
	
	
	add = FLASH_USER_START_SERVER_ADD;
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
		sprintf((char*)user.add, "%s", "NULL");
	}
	
	//coap-option
	add = FLASH_USER_START_COAP;
	for(uint8_t i=0,j=0;i<32;i++,j=j+4)
	{
		uint32_t temp = FLASH_read(add+i*4);
		user.uri[j] 	= (temp>>24) & 0x000000FF;
		user.uri[j+1] = (temp>>16) & 0x000000FF;
		user.uri[j+2] = (temp>>8)  & 0x000000FF;
		user.uri[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.uri) == 0)
	{
		sprintf((char*)user.uri, "%s", "NULL");
	}
	
	//mqtt-uname
	add = FLASH_USER_START_MQTT_BASIC;
	for(uint8_t i=0,j=0;i<10;i++,j=j+4)
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
	add = FLASH_USER_START_MQTT_BASIC+0x04*11;
	for(uint8_t i=0,j=0;i<10;i++,j=j+4)
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
	add = FLASH_USER_START_MQTT_BASIC+0x04*22;
	for(uint8_t i=0,j=0;i<10;i++,j=j+4)
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
	add = FLASH_USER_START_MQTT_TOPIC;
	for(uint8_t i=0,j=0;i<16;i++,j=j+4)
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
	add = FLASH_USER_START_MQTT_TOPIC+0x04*16;
	for(uint8_t i=0,j=0;i<16;i++,j=j+4)
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
}
