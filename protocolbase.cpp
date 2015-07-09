#include "protocolbase.h"

ProtocolBase::ProtocolBase()
{
    theException = NOEXCEPTION;
    memset (swDisDate,0,32);
    memset (swDisTime,0,32);
}

enum ProtocolException ProtocolBase::ExceptionProc ()
{
    enum ProtocolException e = theException;
    theException = NOEXCEPTION;
    return e;
}
void  ProtocolBase::StrDate_ProtocolDateSwitch(const BYTE* p)
{
    p = p;
}
