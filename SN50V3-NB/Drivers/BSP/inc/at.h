#ifndef _at_h_
#define _at_h_

#include <stdbool.h>
#include "stdio.h"
#include "ctype.h"
#include "string.h"
#include "usart.h"
#include "stm32l0xx_hal.h"
#include "common.h"
#include "flash_eraseprogram.h"

/* AT Command strings. Commands start with AT */
#define AT      	 "AT"
#define MODEL			 "+MODEL"
#define RESET      "Z"
#define CFGMOD     "+CFGMOD"
#define DEUI     	 "+DEUI"

#define SERVADDR   "+SERVADDR"

#define FDR        "+FDR"
#define FDR1        "+FDR1"
#define TDC        "+TDC"
#define PWORD      "+PWORD"
#define CFG        "+CFG"
#define INTMOD		 "+INTMOD"
#define APN        "+APN"

#define CLIENT	 	 "+CLIENT"
#define UNAME			 "+UNAME"
#define PWD				 "+PWD"
#define PUBTOPIC	 "+PUBTOPIC"
#define SUBTOPIC	 "+SUBTOPIC"

#define CLRCOUNT   "+CLRCOUNT"
#define _5VT    	 "+5VT"
#define EXT      	 "+EXT"
#define PRO     	 "+PRO"
#define RXDL     	 "+RXDL"
#define WEIGRE     "+WEIGRE"
#define WEIGAP     "+WEIGAP"
#define CDP     	 "+CDP"
#define LDATA      "+LDATA"


#define GETSENSORVALUE      "+GETSENSORVALUE"
#define DNSCFG     "+DNSCFG"	//DNS Server

#define CSQTIME     "+CSQTIME"
#define DNSTIMER     "+DNSTIMER"
#define TLSMOD     "+TLSMOD"
#define SLEEP     "+SLEEP"
#define MQOS      "+MQOS"
#define GETLOG      "+GETLOG"
#define CLOCKLOG    "+CLOCKLOG"
#define URI1    "+URI1"
#define URI2    "+URI2"
#define URI3    "+URI3"
#define URI4    "+URI4"
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
ATEerror_t at_model_get(const char *param);
ATEerror_t at_reset_run(const char *param);
ATEerror_t at_mod_set(const char *param);
ATEerror_t at_mod_get(const char *param);
ATEerror_t at_pword_get(const char *param);
ATEerror_t at_pword_set(const char *param);
ATEerror_t at_fdr_run(const char *param);
ATEerror_t at_fdr1_run(const char *param);
ATEerror_t at_cfg_run(const char *param);

ATEerror_t at_deui_set(const char *param);
ATEerror_t at_deui_get(const char *param);

ATEerror_t at_servaddr_get(const char *param);
ATEerror_t at_servaddr_set(const char *param);

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
ATEerror_t at_rxdl_set(const char *param);
ATEerror_t at_rxdl_get(const char *param);

ATEerror_t at_weight_reset(const char *param);
ATEerror_t at_weight_get(const char *param);
ATEerror_t at_weight_GapValue_set(const char *param);
ATEerror_t at_weight_GapValue_get(const char *param);
ATEerror_t at_ext_get(const char *param);
ATEerror_t at_ext_set(const char *param);
ATEerror_t at_cdp_run(const char *param);
ATEerror_t at_cdp_set(const char *param);

ATEerror_t at_ldata_get(const char *param);
ATEerror_t at_dnscfg_get(const char *param);
ATEerror_t at_dnscfg_set(const char *param);
ATEerror_t at_apn_set(const char *param);
ATEerror_t at_apn_get(const char *param);

ATEerror_t at_return_error(const char *param);



ATEerror_t at_getsensorvalue_set(const char *param);

ATEerror_t at_csqtime_set(const char *param);
ATEerror_t at_csqtime_get(const char *param);

ATEerror_t at_dnstimer_set(const char *param);
ATEerror_t at_dnstimer_get(const char *param);

ATEerror_t at_tlsmod_set(const char *param);
ATEerror_t at_tlsmod_get(const char *param);
ATEerror_t at_sleep_set(const char *param);
ATEerror_t at_sleep_get(const char *param);
ATEerror_t at_mqos_set(const char *param);
ATEerror_t at_mqos_get(const char *param);

ATEerror_t at_getlog_run(const char *param);
ATEerror_t at_getlog_set(const char *param);

ATEerror_t at_clocklog_set(const char *param);
ATEerror_t at_clocklog_get(const char *param);

ATEerror_t at_uri1_set(const char *param);
ATEerror_t at_uri1_get(const char *param);
ATEerror_t at_uri2_set(const char *param);
ATEerror_t at_uri2_get(const char *param);
ATEerror_t at_uri3_set(const char *param);
ATEerror_t at_uri3_get(const char *param);
ATEerror_t at_uri4_set(const char *param);
ATEerror_t at_uri4_get(const char *param);
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
	/** AT+MODEL **/	
  {		
    .string = AT MODEL,
		.size_string = sizeof(MODEL) - 1,
#ifndef NO_HELP
    .help_string = AT MODEL "   : Get module information",
#endif
    .get = at_model_get,
    .set = at_return_error,
    .run = at_return_error,
  },
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
		/** AT+FDR1 **/	
	{
    .string = AT FDR1,
		.size_string = sizeof(FDR1) - 1,
#ifndef NO_HELP
    .help_string = AT FDR1 "     : Reset parameters to factory default values except for passwords",
#endif
    .get = at_return_error,
    .set = at_return_error,
    .run = at_fdr1_run,
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
    .help_string = AT INTMOD "  : Get or Set the trigger interrupt mode (0:input,1:falling or rising,2:falling,3:rising)",
#endif
    .get = at_inmod_get,
    .set = at_inmod_set,
    .run = at_return_error,
  },
		/** AT+APN **/	
	{
    .string = AT APN,
		.size_string = sizeof(APN) - 1,
#ifndef NO_HELP
    .help_string = AT APN "     : Get or set the APN",
#endif
    .get = at_apn_get,
    .set = at_apn_set,
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
			/** AT+EXT **/	
	{
    .string = AT EXT,
		.size_string = sizeof(EXT) - 1,
#ifndef NO_HELP
    .help_string = AT EXT "     : Get or Set Count value",
#endif
    .get = at_ext_get,
    .set = at_ext_set,
    .run = at_return_error,
  },
			/** AT+CDP **/	
	{
    .string = AT CDP,
		.size_string = sizeof(CDP) - 1,
#ifndef NO_HELP
    .help_string = AT CDP "     : Read or Clear cached data",
#endif
    .get = at_return_error,
    .set = at_cdp_set,
    .run = at_cdp_run,
  },
			/** AT+LDATA **/	
	{
    .string = AT LDATA,
		.size_string = sizeof(LDATA) - 1,
#ifndef NO_HELP
    .help_string = AT LDATA "   : Get the last upload data",
#endif
    .get = at_ldata_get,
    .set = at_return_error,
    .run = at_return_error,
  },
			/** AT+GETSENSORVALUE **/	
	{
    .string = AT GETSENSORVALUE,
		.size_string = sizeof(GETSENSORVALUE) - 1,
#ifndef NO_HELP
    .help_string = AT GETSENSORVALUE "     : Returns the current sensor measurement",
#endif
    .get = at_return_error,
    .set = at_getsensorvalue_set,
    .run = at_return_error,
  },
				/** AT+DNSCFG **/	
	{
    .string = AT DNSCFG,
		.size_string = sizeof(DNSCFG) - 1,
#ifndef NO_HELP
    .help_string = AT DNSCFG "  : Get or Set DNS Server",
#endif
    .get = at_dnscfg_get,
    .set = at_dnscfg_set,
    .run = at_return_error,
  },
				/** AT+CSQTIME **/	
	{
    .string = AT CSQTIME,
		.size_string = sizeof(CSQTIME) - 1,
#ifndef NO_HELP
    .help_string = AT CSQTIME "  : Get or Set the time to join the network ",
#endif
    .get = at_csqtime_get,
    .set = at_csqtime_set,
    .run = at_return_error,
  },
				/** AT+DNSTIMER **/	
	{
    .string = AT DNSTIMER,
		.size_string = sizeof(DNSTIMER) - 1,
#ifndef NO_HELP
    .help_string = AT DNSTIMER "  : Get or Set the NDS timer ",
#endif
    .get = at_dnstimer_get,
    .set = at_dnstimer_set,
    .run = at_return_error,
  },
				/** AT+TLSMOD **/	
	{
    .string = AT TLSMOD,
		.size_string = sizeof(TLSMOD) - 1,
#ifndef NO_HELP
    .help_string = AT TLSMOD "  : Get or Set the TLS mode ",
#endif
    .get = at_tlsmod_get,
    .set = at_tlsmod_set,
    .run = at_return_error,
  },
				/** AT+SLEEP **/	
	{
    .string = AT SLEEP,
		.size_string = sizeof(SLEEP) - 1,
#ifndef NO_HELP
    .help_string = AT SLEEP "  : Get or Set the sleep mode ",
#endif
    .get = at_sleep_get,
    .set = at_sleep_set,
    .run = at_return_error,
  },
				/** AT+MQOS **/	
	{
    .string = AT MQOS,
		.size_string = sizeof(MQOS) - 1,
#ifndef NO_HELP
    .help_string = AT MQOS "  : Set the QoS level of MQTT",
#endif
    .get = at_mqos_get,
    .set = at_mqos_set,
    .run = at_return_error,
  },
				/** AT+GETLOG **/	
	{
    .string = AT GETLOG,
		.size_string = sizeof(GETLOG) - 1,
#ifndef NO_HELP
    .help_string = AT GETLOG "  : Print serial port logs",
#endif
    .get = at_return_error,
    .set = at_getlog_set,
    .run = at_getlog_run,
  },
				/** AT+CLOCKLOG **/			
	{
	  .string = AT CLOCKLOG,
    .size_string = sizeof(AT CLOCKLOG) - 1,
#ifndef NO_HELP
    .help_string = AT CLOCKLOG ": Get or set SHT record time",
#endif
    .get = at_clocklog_get,
    .set = at_clocklog_set,
    .run = at_return_error,
	},
				/** AT+URI **/			
	{
	  .string = AT URI1,
    .size_string = sizeof(AT URI1) - 1,
#ifndef NO_HELP
    .help_string = AT URI1 ": Get or set CoAP option 1",
#endif
    .get = at_uri1_get,
    .set = at_uri1_set,
    .run = at_return_error,
	},
	{
	  .string = AT URI2,
    .size_string = sizeof(AT URI2) - 1,
#ifndef NO_HELP
    .help_string = AT URI2 ": Get or set CoAP option 2",
#endif
    .get = at_uri2_get,
    .set = at_uri2_set,
    .run = at_return_error,
	},
	{
	  .string = AT URI3,
    .size_string = sizeof(AT URI3) - 1,
#ifndef NO_HELP
    .help_string = AT URI3 ": Get or set CoAP option 3",
#endif
    .get = at_uri3_get,
    .set = at_uri3_set,
    .run = at_return_error,
	},
	{
	  .string = AT URI4,
    .size_string = sizeof(AT URI4) - 1,
#ifndef NO_HELP
    .help_string = AT URI4 ": Get or set CoAP option 4",
#endif
    .get = at_uri4_get,
    .set = at_uri4_set,
    .run = at_return_error,
	},
};

ATEerror_t ATInsPro( char* at);
void space_fun(char *str);
#endif
