#include "matter_drivers.h"
#include "matter_interaction.h"
#include "led_driver.h"

#include <app-common/zap-generated/attribute-type.h>
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <protocols/interaction_model/StatusCode.h>

#include <stdio.h>

using namespace ::chip::app;
using chip::Protocols::InteractionModel::Status;

#define PWM_PIN         PA_23

MatterLED led;

// Set identify cluster and its callback on ep1
static Identify gIdentify1 = {
    chip::EndpointId{ 1 }, matter_driver_on_identify_start, matter_driver_on_identify_stop, Clusters::Identify::IdentifyTypeEnum::kVisibleIndicator, matter_driver_on_trigger_effect,
};

CHIP_ERROR matter_driver_my_dimmable_light_init()
{
    led.Init(PWM_PIN);
    return CHIP_NO_ERROR;
}

CHIP_ERROR matter_driver_my_dimmable_light_set_startup_value()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    bool LEDOnOffValue = 0;
    DataModel::Nullable<uint8_t> LEDCurrentLevelValue = 128;
    Status getonoffstatus;
    Status getcurrentlevelstatus;

    chip::DeviceLayer::PlatformMgr().LockChipStack();
    getonoffstatus = Clusters::OnOff::Attributes::OnOff::Get(1, &LEDOnOffValue);
    VerifyOrExit(getonoffstatus == Status::Success, err = CHIP_ERROR_INTERNAL);

    getcurrentlevelstatus = Clusters::LevelControl::Attributes::CurrentLevel::Get(1, LEDCurrentLevelValue);
    VerifyOrExit(getcurrentlevelstatus == Status::Success, err = CHIP_ERROR_INTERNAL);
    chip::DeviceLayer::PlatformMgr().UnlockChipStack();

    // Set LED to onoff value
    led.Set(LEDOnOffValue);

    // Set LED to currentlevel value
    led.SetBrightness(LEDCurrentLevelValue.Value());

exit:
    if (err == CHIP_ERROR_INTERNAL)
    {
        chip::DeviceLayer::PlatformMgr().UnlockChipStack();
    }
    return err;
}

void matter_driver_on_identify_start(Identify * identify)
{
    ChipLogProgress(Zcl, "OnIdentifyStart");
}

void matter_driver_on_identify_stop(Identify * identify)
{
    ChipLogProgress(Zcl, "OnIdentifyStop");
}

void matter_driver_on_trigger_effect(Identify * identify)
{
    switch (identify->mCurrentEffectIdentifier)
    {
    case Clusters::Identify::EffectIdentifierEnum::kBlink:
        ChipLogProgress(Zcl, "Clusters::Identify::EffectIdentifierEnum::kBlink");
        break;
    case Clusters::Identify::EffectIdentifierEnum::kBreathe:
        ChipLogProgress(Zcl, "Clusters::Identify::EffectIdentifierEnum::kBreathe");
        break;
    case Clusters::Identify::EffectIdentifierEnum::kOkay:
        ChipLogProgress(Zcl, "Clusters::Identify::EffectIdentifierEnum::kOkay");
        break;
    case Clusters::Identify::EffectIdentifierEnum::kChannelChange:
        ChipLogProgress(Zcl, "Clusters::Identify::EffectIdentifierEnum::kChannelChange");
        break;
    default:
        ChipLogProgress(Zcl, "No identifier effect");
        return;
    }
}

void matter_driver_uplink_update_handler(AppEvent *aEvent)
{
    chip::app::ConcreteAttributePath path = aEvent->path;

    // this example only considers endpoint 1
    VerifyOrExit(aEvent->path.mEndpointId == 1,
                 ChipLogError(DeviceLayer, "Unexpected EndPoint ID: `0x%02x'", path.mEndpointId));

    switch(path.mClusterId)
    {
    case Clusters::OnOff::Id:
        if (path.mAttributeId == Clusters::OnOff::Attributes::OnOff::Id)
        {
            led.Set(aEvent->value._u8);
        }
        printf("OnOff Cluster: aEvent->value._u8 = %d\n", aEvent->value._u8);
        break;
    case Clusters::LevelControl::Id:
        if(path.mAttributeId == Clusters::LevelControl::Attributes::CurrentLevel::Id)
        {
            led.SetBrightness(aEvent->value._u8);
        }
        printf("LevelControl: aEvent->value._u8 = %d\n", aEvent->value._u8);
        break;
    case Clusters::Identify::Id:
        break;
    }

exit:
    return;
}
