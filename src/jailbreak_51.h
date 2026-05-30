#ifndef JAILBREAK_51_H_
#define JAILBREAK_51_H_

#include "jailbreak.h"
#include "device.h"
#include "lockdown.h"

int jailbreak_51(const char* udid, status_cb_t status_cb,
                 device_t* device, lockdown_t* lockdown,
                 const char* product, const char* build);

#endif /* JAILBREAK_51_H_ */
