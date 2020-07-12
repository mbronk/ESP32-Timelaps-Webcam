Import("env") # access to global construction environment
import re
import os

host_rx = re.compile(r'.+DNS_HOSTNAME\s=\s*["](\w+)["]')
with open('include/settings.h', "r") as f:
    for l in f.readlines(): 
        if host_rx.match(l):
            env.Append(ESP_HOSTNAME = host_rx.match(l).group(1))

pwd_rx = re.compile(r'.+OTA_PASSWORD(\s=)?\s*["]([^"]+)["]')
with open('include/wifi_credentials.h', "r") as f:
    for l in f.readlines():         
        print(l)
        if pwd_rx.match(l):
            env.Append(OTA_PASSWORD = pwd_rx.match(l).group(2))
