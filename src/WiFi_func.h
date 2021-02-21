#pragma once
#ifndef _WIFI_FUNC_H
#define _WLAN_H

void WiFiInit(void);
int WiFiDetermineMode(void);
bool WiFiConnectSTA(void);
void WiFiCreateAP(void);


#endif
