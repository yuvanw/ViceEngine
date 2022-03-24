#include "GenericApplication.h"

FGenericApplication::FGenericApplication()
	: MessageHandler(new FGenericApplicationMessageHandler())
	, bIsRunning(true)
{
}

TUniquePtr<FGenericWindow> FGenericApplication::MakeWindow(const ANSICHAR* InWindowTitle, int32 InWidth, int32 InHeight)
{
	return MakeUnique<FGenericWindow>(InWindowTitle, InWidth, InHeight);
}

void FGenericApplication::SetApplicationMessageHandler(const TSharedPtr<FGenericApplicationMessageHandler>& InApplicationMessageHandler)
{
	MessageHandler = InApplicationMessageHandler;
}

bool FGenericApplication::IsRunning() const
{
	return bIsRunning;
}
