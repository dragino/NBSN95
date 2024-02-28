# SN50v3-NB / SN50v3-NS
Source Code for SN503-NB: NB-IoT Sensor Node Base on Quectel BC660K-GL

<!-- TOC depthFrom:1 -->
- Mother Board: https://github.com/dragino/Lora/tree/master/LSN50/v3.0/motherboard 
- NB-IoT Module: https://github.com/dragino/NB-IoT/tree/master/NB%20ST/BC660K-GL 
<!-- /TOC -->

# FAQ
## 1. Switch the source between -NB and -NS products. 
The source support define for both -NB and -NS series models. The -NS series sample the voltage from rechargable battery which is different from -NB series. 

User can modify the MACRO during build to switch between these two series. 

Below is the location.

<img width="480" alt="eb575b8259f1ed115c87e391dc8c438" src="https://github.com/dragino/NBSN95/assets/652246/48784b33-20f5-48ea-abcd-a5b7854687b2">

* When there is **NB_NS** defined, the source code is for -NS series. 
* When there is **no NB_NS** defined, the source code is for -NB series. 



