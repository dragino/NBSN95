#include "at.h"

static uint8_t keep = 0;
static uint32_t config_data[32]={0};

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

/************** 			AT+CFGMOD			 **************/
ATEerror_t at_mod_set(const char *param)
{
	char* pos = strchr(param,'=');
	sys.mod = param[(pos-param)+1];

	if(sys.mod == model1)
	{
		user_main_printf("\r\nDefault mode\r\n");
	}
	else if(sys.mod == model2)
	{
		user_main_printf("\r\nDistance mode\r\n");
	}
	else if(sys.mod == model3)
	{
		user_main_printf("\r\n3xADC + I2C mode\r\n");
	}
	else if(sys.mod == model4)
	{
		user_main_printf("\r\n3xDS18B20 mode\r\n");
	}
	else if(sys.mod == model5)
	{
		user_main_printf("\r\nWeiget mode\r\n");
	}
	else if(sys.mod == model6)
	{
		user_main_printf("\r\nCounting mode\r\n");
	}
	
	else
	{
		user_main_printf("%s",ATError_description[AT_PARAM_ERROR]);
		return AT_PARAM_ERROR;
	}
	
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
	const char* p = param+(pos-param)+1;
	
	sys.pwd[0] = p[3]<<24 |  p[2]<<16 | p[1]<<8 | p[0];
	sys.pwd[1] = p[7]<<24 |  p[6]<<16 | p[5]<<8 | p[4];
		
  return AT_OK;
}

ATEerror_t at_pword_get(const char *param)
{	
	if(keep)
		printf("AT+PWORD=");

	printf("%s\r\n",(char*)sys.pwd);
	return AT_OK;
}

/************** 			AT+FDR		 **************/
ATEerror_t at_fdr_run(const char *param)
{
	FLASH_erase(FLASH_USER_START_ADDR_CONFIG,(FLASH_USER_END_ADDR - FLASH_USER_START_ADDR_CONFIG) / FLASH_PAGE_SIZE);
	NVIC_SystemReset();
  return AT_OK;
}

/************** 			AT+CFG		 **************/
ATEerror_t at_cfg_run(const char *param)
{
	keep = 1;
	user_main_printf();
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
	
  return AT_OK;
}

ATEerror_t at_servaddr_set(const char *param)
{
	char* pos = strchr(param,'=');
	if(strlen(param) - (pos-param)-1 >21)
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
	if(strlen(param) - (pos-param)-1 >64)
	{
		return AT_PARAM_ERROR;
	}

	memset(user.uri,0,sizeof(user.uri));
	memcpy(user.uri,(param+(pos-param)+1),strlen((param+(pos-param)+1)));	
	
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
	sys.tdc = atoi((param+(pos-param)+1));
	if(sys.tdc > 0xFFFFFF)
	{
		sys.tdc = 1200;
		return AT_PARAM_ERROR;
	}
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
		sys.inmod = '0';
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
	sys.power_time = atoi((param+(pos-param)+1));
	if(sys.power_time > 0xFFFFFF)
	{
		sys.power_time = 0;
		return AT_PARAM_ERROR;
	}
	
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
		user_main_printf("%0.1f g\r\n",(float)Get_Weight());
	
  return AT_OK;		
}
/************** 			AT+CNTFAC		**************/
ATEerror_t at_cntfac_set(const char *param)
{
	char* pos = strchr(param,'=');
	sensor.factor=atof((param+(pos-param)+1));
	pos = strchr(param,'.');
	if(pos == NULL)
		sensor.factor_number = 0;
	else
	{
		sensor.factor_number = strlen(param) - (pos-param)-1;
	}

	return AT_OK;
}

ATEerror_t at_cntfac_get(const char *param)
{
	if(keep)
		printf(AT CNTFAC"=");
	printf("%0.2f\r\n",sensor.factor);
	return AT_OK;
}

char *rtrim(char *str)
{
	for(int i=0;i<strlen(str);i++)
	{
		if(str[i]=='\r' || str[i]=='\n')
			str[i] = 0;
	}
  
	return str;
}

void config_Set(void)
{
	memset(config_data,0,sizeof(config_data));
	
	config_data[0]=sys.pwd[0];
	config_data[1]=sys.pwd[1];
	config_data[2]=sys.mod<<24 | sys.tdc;
	config_data[3]=sys.inmod<<24 | sys.power_time;
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.add);i=i+4,j++)
			config_data[4+j]=user.add[i+0]<<24 | user.add[i+1]<<16 | user.add[i+2]<<8 | user.add[i+3];
	
	for(uint8_t i=0,j=0;i<strlen((char*)user.uri);i=i+4,j++)
		config_data[10+j]=user.uri[i+0]<<24 | user.uri[i+1]<<16 | user.uri[i+2]<<8 | user.uri[i+3];
	
	config_data[27]= sensor.GapValue*10 ;
	config_data[28]= sensor.factor_number;
	config_data[29]= sensor.factor*pow(10,sensor.factor_number);
	
	FLASH_erase(FLASH_USER_START_ADDR_CONFIG,1);	
	FLASH_program(FLASH_USER_START_ADDR_CONFIG,config_data, 30);
}

void config_Get(void)
{
	uint32_t add = FLASH_USER_START_ADDR_CONFIG;
	
	sys.pwd[0] = FLASH_read(add);
	sys.pwd[1] = FLASH_read(add+4);
	if(sys.pwd[0] == 0 && sys.pwd[1] == 0)
	{
		sys.pwd[0] = 0x34333231;
		sys.pwd[1] = 0x38373635;
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
		
	sys.power_time = FLASH_read(add+12) & 0x00FFFFFF;
	
	for(uint8_t i=0,j=0;i<6;i++,j=j+4)
	{
		uint32_t temp = FLASH_read(add+16+i*4);
		user.add[j] 	= (temp>>24) & 0x000000FF;
		user.add[j+1] = (temp>>16) & 0x000000FF;
		user.add[j+2] = (temp>>8)  & 0x000000FF;
		user.add[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.add) == 0)
	{
		sprintf((char*)user.add+strlen((char*)user.add), "%s", addressDefault);
	}
	
	for(uint8_t i=0,j=0;i<16;i++,j=j+4)
	{
		uint32_t temp = FLASH_read(add+40+i*4);
		user.uri[j] 	= (temp>>24) & 0x000000FF;
		user.uri[j+1] = (temp>>16) & 0x000000FF;
		user.uri[j+2] = (temp>>8)  & 0x000000FF;
		user.uri[j+3] = (temp)     & 0x000000FF;
	}
	if(strlen((char*)user.uri) == 0)
	{
		sprintf((char*)user.uri+strlen((char*)user.uri), "%s", "/mqtt/COAPTEXT?c=");
		sprintf((char*)user.uri+strlen((char*)user.uri), "%s", nb.imei);
	}
	
	sensor.GapValue = FLASH_read(0x0801906C)/10;
	if(sensor.GapValue == 0)
		sensor.GapValue = 400.0;
	
	sensor.factor_number = FLASH_read(0x08019070);
	sensor.factor = (float)FLASH_read(0x08019074) / pow(10,sensor.factor_number);
	if(sensor.factor == 0.0)
		sensor.factor = 1.0;
}
