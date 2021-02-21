#include "SD_MMC.h"
#include "settings.h"

bool SDWriteFile(const char *path, const unsigned char *data, unsigned long len)
{
  int64_t start = esp_timer_get_time();
  Serial.printf("Writing file: %s\r\n", path);
  File file = SD_MMC.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return false;
  }
  if (file.write(data, len))
  {
    int64_t end = esp_timer_get_time();
    int64_t time = (end - start) / 1000;
    Serial.printf("File written: %ums \r\n",(uint32_t)time);
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
    return false;
  }
  file.close();
  return true;
}

bool SDappendFile(const char *path, const unsigned char *data, unsigned long len)
{
  Serial.printf("Appending to file: %s\r\n", path);

  File file = SD_MMC.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return false;
  }
  if (file.write(data, len))
  {
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
    return false;
  }
  file.close();
  return true;
}

bool SDInitFileSystem()
{
  Serial.println("Init file system");

  // pinMode(GPIO_NUM_14, PULLUP); // SD_CLK    
  // pinMode(GPIO_NUM_15, PULLUP); // SD_CMD
  // pinMode(GPIO_NUM_2, PULLUP);  // SD_DAT0
  // #ifndef USE_SDMMC_1BIT
  //   pinMode(GPIO_NUM_4, PULLUP);  // SD_DAT1
  //   pinMode(GPIO_NUM_12, PULLUP);  // SD_DAT2
  //   pinMode(GPIO_NUM_13, PULLUP);  // SD_DAT3
  // #else
  //   pinMode(GPIO_NUM_4, PULLDOWN);
  // #endif

  #ifdef USE_SDMMC_1BIT
    auto sdStatus = SD_MMC.begin("/sdcard",true);
    pinMode(LED_FLASH, OUTPUT);
    digitalWrite(LED_FLASH, LOW);
    if (!sdStatus)
    {      
      Serial.println("Card Mount Failed (SDMMC_HOST_FLAG_1BIT)");
      return false;
    }    
  #else
    if (!SD_MMC.begin())
    {
      Serial.println("Card Mount Failed");
      return false;
    }
  #endif
  uint8_t cardType = SD_MMC.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return false;
  }
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
    Serial.println("MMC");
  else if (cardType == CARD_SD)
    Serial.println("SDSC");
  else if (cardType == CARD_SDHC)
    Serial.println("SDHC");
  else
    Serial.println("UNKNOWN");

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\r\n", cardSize);
  Serial.printf("Total space: %lluMB\r\n", SD_MMC.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\r\n", SD_MMC.usedBytes() / (1024 * 1024));
  Serial.println("=======================================");
  Serial.println("");
  return true;
}

bool SDCreateDir(const char *path)
{
  Serial.printf("Creating Dir: %s\r\n", path);
  if (SD_MMC.mkdir(path))
  {
    Serial.println("Dir created");
  }
  else
  {
    Serial.println("mkdir failed");
    return false;
  }
  return true;
}

bool SDFileExists(const char *path)
{
  return SD_MMC.exists(path);
}
