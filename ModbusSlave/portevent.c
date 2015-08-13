#include "port.h"
#include "modbus.h"

static volatile char eHasEvent = 0;
static volatile eMBEventType eEventType;

void eMBPushEvent(eMBEventType eEvent)
{
	eHasEvent = 1;
	eEventType = eEvent;
}

char eMBPopEvent(eMBEventType* eEvent)
{
	char hasEvent = 0;
	if(eHasEvent)
	{
		*eEvent = eEventType;
		eHasEvent = 0;
		hasEvent = 1;
	}
	return hasEvent;
}
