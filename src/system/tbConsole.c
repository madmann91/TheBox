#include "tbConfig.h"
#include "tbConsole.h"

static const char* days[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static const char* months[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

void tbMessage(const char* format,...)
{
	va_list args;
	time_t t;
	struct tm* tm;

	time(&t);
	tm=localtime(&t);

	printf("[%s %s %d %02d:%02d:%02d] ",days[tm->tm_wday],months[tm->tm_mon],tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);

	va_start(args,format);
	vprintf(format,args);
	va_end(args);

	printf("\n");
}

void tbPrint(const char* format,...)
{
	va_list args;
	
	va_start(args,format);
	vprintf(format,args);
	va_end(args);
}
