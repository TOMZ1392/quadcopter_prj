#ifndef SWITCH_COFIGURED
#define SWITCH_COFIGURED
#define SWITCH_INPUT A3
#define KEYPRS_DLY 50
//#define EN_DEBUG
#define EN_DEBUG_ERR
#define INTERVAL_MINUS 10
#define INTERVAL_PLUS 10
typedef enum
{
    NO_BTN_PRS,
    RGT_BTN,
    UP_BTN,
    LFT_BTN,
    DWN_BTN,
    SET_BTN,
    BTN_ERR = 0xFF,
} btnMap_en;

typedef struct
{
    btnMap_en buttonId;
    uint16_t buttonADClb;
} btnADCMap_t;

#define BTN_MAP_COUNT 5

btnADCMap_t btnADCMap[BTN_MAP_COUNT] = {
    {RGT_BTN, 114},
    {UP_BTN, 130},
    {LFT_BTN, 150},
    {DWN_BTN, 177},
    {SET_BTN, 1014}, // 1023
};
#endif
boolean isInInterval(uint32_t input, uint32_t fixed, uint16_t minus, uint16_t plus)
{
    if (input > fixed - minus && input < fixed + plus)
    {
        return true;
    }
    else
    {
        return false;
    }
}

btnMap_en DetectkeyPress()
{
    volatile uint16_t swtV_1 = 0;
    uint8_t mapIdx = 0;
    btnMap_en btn_en = BTN_ERR;

    btnMap_en retVal = NO_BTN_PRS;

    swtV_1 = analogRead(SWITCH_INPUT);

#ifdef EN_DEBUG
    Serial.println(swtV_1);
#endif
    if (swtV_1 == 0)
    {
        return retVal;
    }
    else
    {
        while (mapIdx < BTN_MAP_COUNT)
        {
            if (isInInterval(swtV_1, btnADCMap[mapIdx].buttonADClb, INTERVAL_MINUS, INTERVAL_PLUS))
            {
                delay(KEYPRS_DLY);
                swtV_1 = analogRead(SWITCH_INPUT);
                if (isInInterval(swtV_1, btnADCMap[mapIdx].buttonADClb, INTERVAL_MINUS, INTERVAL_PLUS))
                {
                    btn_en = btnADCMap[mapIdx].buttonId;
                    break;
                }
                else
                {
                    return NO_BTN_PRS;
                }
            }
            mapIdx++;
        }

        switch (btn_en)
        {
        case RGT_BTN:
            Serial.println("Right key pressed");
            retVal = RGT_BTN;
            break;
        case UP_BTN:
            Serial.println("Up key pressed");
            retVal = UP_BTN;
            break;
        case LFT_BTN:
            Serial.println("Left key pressed");
            retVal = LFT_BTN;
            break;
        case DWN_BTN:
            Serial.println("Down key pressed");
            retVal = DWN_BTN;
            break;
        case SET_BTN:
            Serial.println("Set key pressed");
            retVal = SET_BTN;
            break;
        default:
#ifdef EN_DEBUG_ERR
            Serial.println(swtV_1);
#endif
            Serial.println("error!!");
            retVal = BTN_ERR;
            break;
        }
    }
    return retVal;
}
