#include "..\..\inculde\CQAPI.h"
#include "..\..\inculde\CQEVE.h"

int AuthCode;
CQEVENT(int, Initialize, 4)(int AuthCode)
{
	::AuthCode = AuthCode;
	return 0;
}
int getAuthCode()
{
	return AuthCode;
}

int CQ::getAuthCode()
{
	return AuthCode;
}