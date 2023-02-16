#include "pch.h"
#include "Event.h"

CallbackNoArgRef MakeCallbackNoArgRef(CallbackNoArg arg)
{
	return MakeRef<CallbackNoArg>(arg);
}
