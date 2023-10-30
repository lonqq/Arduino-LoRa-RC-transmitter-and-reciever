#ifndef _MIXER_H_
#define _MIXER_H_

void computeChannelOutputs();

int16_t calcRateExpo(int16_t input, int16_t rate, int16_t expo);
bool checkSwitchCondition(uint8_t sw);
void moveMix(uint8_t newPos, uint8_t oldPos);
void swapMix(uint8_t posA, uint8_t posB);
void syncWaveform(uint8_t idx);
void reinitialiseMixerCalculations();

extern int16_t mixSources[MIXSOURCES_COUNT]; 

#endif