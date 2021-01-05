#ifndef _at_h_
#define _at_h_

#include "stdio.h"
#include "ctype.h"
#include "string.h"
#include "usart.h"
#include "stm32l0xx_hal.h"
#include "common.h"
#include "flash_eraseprogram.h"

/* AT Command strings. Commands start with AT */
#define AT      	 "AT"
#define RESET      "Z"
#define CFGMOD     "+CFGMOD"
#define DEUI     	 "+DEUI"

#define SERVADDR   "+SERVADDR"
#define URI   		 "+URI"

#define FDR        "+FDR"
#define TDC        "+TDC"
#define PWORD      "+PWORD"
#define CFG        "+CFG"
#define INTMOD		 "+INTMOD"

#define CLIENT	 	 "+CLIENT"
#define UNAME			 "+UNAME"
#define PWD				 "+PWD"
#define PUBTOPIC	 "+PUBTOPIC"
#define SUBTOPIC	 "+SUBTOPIC"

#define CLRCOUNT   "+CLRCOUNT"
#define _5VT    	 "+5VT"

#define PRO     	 "+PRO"
#define CFM     	 "+CFM"
#define RXDL     	 "+RXDL"
#define WEIGRE     "+WEIGRE"
#define WEIGAP     "+WEIGAP"
#define CNTFAC     "+CNTFAC"
/**********************************************/

typedef enum
{
	AT_OK = 0,
  AT_ERROR,
	AT_PARAM_ERROR,
	AT_BUSY_ERROR,
	AT_TEST_PARAM_OVERFLOW,
	AT_RX_ERROR
}ATEerror_t;

static const char *const ATError_description[] =
{
  "\r\nOK\r\n",
	"\r\nERROR\r\n",
	"\r\nAT_PARAM_ERROR\r\n",
	"\r\nAT_BUSY_ERROR\r\n",
	"\r\nAT_TEST_PARAM_OVERFLOW\r\n",
	"\r\nAT_RX_ERROR\r\n"
};

ATEerror_t at(const char *param);
ATEerror_t at_que(const char *param);
ATEerror_t at_reset_run(const char *param);
ATEerror_t at_mod_set(const char *param);
ATEerror_t at_mod_get(const char *param);
ATEerror_t at_pword_get(const char *param);
ATEerror_t at_pword_set(const char *param);
ATEerror_t at_fdr_run(const char *param);
ATEerror_t at_cfg_run(const char *param);

ATEerror_t at_deui_set(const char *param);
ATEerror_t at_deui_get(const char *param);

ATEerror_t at_servaddr_get(const char *param);
ATEerror_t at_servaddr_set(const char *param);

ATEerror_t at_uri_get(const char *param);
ATEerror_t at_uri_set(const char *param);

ATEerror_t at_client_get(const char *param);
ATEerror_t at_client_set(const char *param);
ATEerror_t at_uname_get(const char *param);
ATEerror_t at_uname_set(const char *param);
ATEerror_t at_pwd_get(const char *param);
ATEerror_t at_pwd_set(const char *param);
ATEerror_t at_pubtopic_get(const char *param);
ATEerror_t at_pubtopic_set(const char *param);
ATEerror_t at_subtopic_get(const char *param);
ATEerror_t at_subtopic_set(const char *param);
ATEerror_t at_tdc_get(const char *param);
ATEerror_t at_tdc_set(const char *param);
ATEerror_t at_inmod_set(const char *param);
ATEerror_t at_inmod_get(const char *param);
ATEerror_t at_5vt_set(const char *param);
ATEerror_t at_5vt_get(const char *param);
ATEerror_t at_cntfac_set(const char *param);
ATEerror_t at_cntfac_get(const char *param);
ATEerror_t at_pro_set(const char *param);
ATEerror_t at_pro_get(const char *param);
ATEerror_t at_cfm_set(const char *param);
ATEerror_t at_cfm_get(const char *param);
ATEerror_t at_rxdl_set(const char *param);
ATEerror_t at_rxdl_get(const char *param);

ATEerror_t at_weight_reset(const char *param);
ATEerror_t at_weight_get(const char *param);
ATEerror_t at_weight_GapValue_set(const char *param);
ATEerror_t at_weight_GapValue_get(const char *param);

ATEerror_t at_cntfac_set(const char *param);
ATEerror_t at_cntfac_get(const char *param);

ATEerror_t at_return_error(const char *param);

/*Other*/
char *rtrim(char *str);
uint8_t hexDetection(char* str);
void config_Set(void);
void config_Get(void);

struct ATCommand_s 
{
  const char *string;                       /*< command string, after the "AT" */
  const int size_string;                    /*< size of the command string, not including the final \0 */
  ATEerror_t (*get)(const char *param);     /*< =? after the string to get the current value*/
  ATEerror_t (*set)(const char *param);     /*< = (but not =?\0) after the string to set a value */
  ATEerror_t (*run)(const char *param);     /*< \0 after the string - run the command */
#if !defined(NO_HELP)
  const char *help_string;                  /*< to be printed when ? after the string */
#endif
};

static const struct ATCommand_s ATCommand[] =
{	
	/** ATZ **/	
  {		
    .string = AT RESET,
		.size_string = sizeof(RESET) - 1,
#ifndef NO_HELP
    .help_string = AT RESET "        : Trig a reset of the MCU",
#endif
    .get = at_return_error,
    .set = at_return_error,
    .run = at_reset_run,
  },
	/** AT+CFGMOD **/	
  {
    .string = AT CFGMOD,
		.size_string = sizeof(CFGMOD) - 1,
#ifndef NO_HELP
    .help_string = AT CFGMOD "  : Working mode selection",
#endif
    .get = at_mod_get,
    .set = at_mod_set,
    .run = at_return_error,
  },
	/** AT+DEUI **/	
  {
    .string = AT DEUI,
		.size_string = sizeof(DEUI) - 1,
#ifndef NO_HELP
    .help_string = AT DEUI "    : Get or set the Device ID",
#endif
    .get = at_deui_get,
    .set = at_deui_set,
    .run = at_return_error,
  },
	/** AT+PWORD **/
	{
    .string = AT PWORD,
		.size_string = sizeof(PWORD) - 1,
#ifndef NO_HELP
    .help_string = AT PWORD "   : Get or set the System password",
#endif
    .get = at_pword_get,
    .set = at_pword_set,
    .run = at_return_error,
  },	
		/** AT+FDR **/	
	{
    .string = AT FDR,
		.size_string = sizeof(FDR) - 1,
#ifndef NO_HELP
    .help_string = AT FDR "     : Reset Parameters to Factory Default",
#endif
    .get = at_return_error,
    .set = at_return_error,
    .run = at_fdr_run,
  },
		/** AT+CFG **/	
	{
    .string = AT CFG,
		.size_string = sizeof(CFG) - 1,
#ifndef NO_HELP
    .help_string = AT CFG "     : Print all settings",
#endif
    .get = at_return_error,
    .set = at_return_error,
    .run = at_cfg_run,
  },
		/** AT+SERVADDR **/	
	{
    .string = AT SERVADDR,
		.size_string = sizeof(SERVADDR) - 1,
#ifndef NO_HELP
    .help_string = AT SERVADDR ": Get or Set the Server address",
#endif
    .get = at_servaddr_get,
    .set = at_servaddr_set,
    .run = at_return_error,
  },
			/** AT+URI **/	
	{
    .string = AT URI,
		.size_string = sizeof(URI) - 1,
#ifndef NO_HELP
    .help_string = AT URI "   : Get or set CoAP options",
#endif
    .get = at_uri_get,
    .set = at_uri_set,
    .run = at_return_error,
  },
		/** AT+CLIENT **/	
	{
    .string = AT CLIENT,
		.size_string = sizeof(CLIENT) - 1,
#ifndef NO_HELP
    .help_string = AT CLIENT "  : Get or Set the MQTT clientID",
#endif
    .get = at_client_get,
    .set = at_client_set,
    .run = at_return_error,
  },	
	/** AT+UNAME **/	
	{
    .string = AT UNAME,
		.size_string = sizeof(UNAME) - 1,
#ifndef NO_HELP
    .help_string = AT UNAME "   : Get or Set the MQTT Username",
#endif
    .get = at_uname_get,
    .set = at_uname_set,
    .run = at_return_error,
  },
	/** AT+PWD **/	
	{
    .string = AT PWD,
		.size_string = sizeof(PWD) - 1,
#ifndef NO_HELP
    .help_string = AT PWD "     : Get or Set the MQTT password",
#endif
    .get = at_pwd_get,
    .set = at_pwd_set,
    .run = at_return_error,
  },
	/** AT+PUBTOPIC **/	
	{
    .string = AT PUBTOPIC,
		.size_string = sizeof(PUBTOPIC) - 1,
#ifndef NO_HELP
    .help_string = AT PUBTOPIC ": Get or set MQTT publishing topic",
#endif
    .get = at_pubtopic_get,
    .set = at_pubtopic_set,
    .run = at_return_error,
  },
	/** AT+SUBTOPIC **/	
	{
    .string = AT SUBTOPIC,
		.size_string = sizeof(SUBTOPIC) - 1,
#ifndef NO_HELP
    .help_string = AT SUBTOPIC ": Get or set MQTT subscription topic",
#endif
    .get = at_subtopic_get,
    .set = at_subtopic_set,
    .run = at_return_error,
  },
		/** AT+TDC **/	
	{
    .string = AT TDC,
		.size_string = sizeof(TDC) - 1,
#ifndef NO_HELP
    .help_string = AT TDC "     : Get or set the application data transmission interval in s",
#endif
    .get = at_tdc_get,
    .set = at_tdc_set,
    .run = at_return_error,
  },
		/** AT+INTMOD **/	
	{
    .string = AT INTMOD,
		.size_string = sizeof(INTMOD) - 1,
#ifndef NO_HELP
    .help_string = AT INTMOD "  : Get or Set the trigger interrupt mode (0:Disable,1:falling or rising,2:falling,3:rising)",
#endif
    .get = at_inmod_get,
    .set = at_inmod_set,
    .run = at_return_error,
  },
	/** AT+5VT **/	
	{
    .string = AT _5VT,
		.size_string = sizeof(_5VT) - 1,
#ifndef NO_HELP
    .help_string = AT _5VT "     : Get or Set extend the time of 5V power",
#endif
    .get = at_5vt_get,
    .set = at_5vt_set,
    .run = at_return_error,
  },
			/** AT+PRO **/	
	{
    .string = AT PRO,
		.size_string = sizeof(PRO) - 1,
#ifndef NO_HELP
    .help_string = AT PRO "     : Get or Set usage agreement (1:COAP,2:UDP,3:MQTT,4:TCP)",
#endif
    .get = at_pro_get,
    .set = at_pro_set,
    .run = at_return_error,
  },
			/** AT+CFM **/	
	{
    .string = AT CFM,
		.size_string = sizeof(CFM) - 1,
#ifndef NO_HELP
    .help_string = AT CFM "     : Get or Set confirmation mode (0: Off 1: On)",
#endif
    .get = at_cfm_get,
    .set = at_cfm_set,
    .run = at_return_error,
  },
			/** AT+RXDL **/	
	{
    .string = AT RXDL,
		.size_string = sizeof(RXDL) - 1,
#ifndef NO_HELP
    .help_string = AT RXDL "    : Get or Set the receiving time",
#endif
    .get = at_rxdl_get,
    .set = at_rxdl_set,
    .run = at_return_error,
  },
			/** AT+WEIGAP **/	
	{
    .string = AT WEIGAP,
		.size_string = sizeof(WEIGAP) - 1,
#ifndef NO_HELP
    .help_string = AT WEIGAP "  : Get or Set the GapValue of weight",
#endif
    .get = at_weight_GapValue_get,
    .set = at_weight_GapValue_set,
    .run = at_return_error,
  },
			/** AT+WEIGRE **/	
	{
    .string = AT WEIGRE,
		.size_string = sizeof(WEIGRE) - 1,
#ifndef NO_HELP
    .help_string = AT WEIGRE "  : Get weight or set weight to 0g",
#endif
    .get = at_weight_get,
    .set = at_return_error,
    .run = at_weight_reset,
  },
			/** AT+CNTFAC **/	
	{
    .string = AT CNTFAC,
		.size_string = sizeof(CNTFAC) - 1,
#ifndef NO_HELP
    .help_string = AT CNTFAC "  : Get or set counting parameters",
#endif
    .get = at_cntfac_get,
    .set = at_cntfac_set,
    .run = at_return_error,
  },
};

ATEerror_t ATInsPro( char* at);

#endif
