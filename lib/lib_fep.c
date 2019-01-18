#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include    <time.h>


/************************************************************************/
/*      String -> Number                                                */
/************************************************************************/
int __Str2Int(str, len)
char *str;
int len;
{
    short i;
    int  rval = 0;
    int  sign = 1;

    for (i = 0; i < len; i++)
    {
        if (str[i] == '-') sign = -1;
        if( (*(str + i)) != 0x20 )
            rval = rval * 10 + *(str + i) - '0';
    }
    rval = rval * sign;
    return (rval);
}

long __Str2Long(str, len)
char *str;
int len;
{
    short i;
    long  rval = 0;
    long  sign = 1;

    for (i = 0; i < len; i++)
    {
        if (str[i] == '-') sign = -1;
        if( isdigit(*(str + i)) )
            rval = rval * 10 + *(str + i) - '0';
    }
    rval = rval * sign;
    return (rval);
}

long long __Str2LongLong(str, len)
char *str;
int len;
{
    short i;
    long double  rval = 0L;
    long sign = 1;

    for (i = 0; i < len; i++)
    {
        if (str[i] == '-') sign = -1;
//      if( (*(str + i)) != 0x20 )
        if( isdigit(*(str + i)) )
            rval = rval * 10L + (long double) (*(str + i) - '0');
    }
    rval = rval * sign;
    return (rval);
}

unsigned long __Str2uLong(str, len)
char *str;
int len;
{
    short i;
    unsigned long  rval = 0;

    for (i = 0; i < len; i++)
    {
        if( (*(str + i)) != 0x20 )
            rval = rval * 10 + *(str + i) - '0';
    }
    return (rval);
}

float __Str2Float(str, len, pivot)
char *str;
int len;
int pivot;
{
    short i;
    float rval = 0.0;
    double sign = 1.0;

    for (i = 0; i < len; i++)
    {
        if(str[i] == '-') sign = -1.0;
        if( isdigit(*(str + i)) )
            rval = rval * 10.0 + *(str + i) - '0';
    }
    if(rval != 0.0)
        rval = rval * 1.0 / pow(10, pivot) * 1.0 * sign;

    return (rval);
}

double __Str2Double(str, len, pivot)
char *str;
int len;
int pivot;
{
    short i;
    double rval = 0.0;
    double sign = 1.0;

    for (i = 0; i < len; i++)
    {
        if(str[i] == '-') sign = -1.0;
        if( isdigit(*(str + i)) )
            rval = rval * 10.0 + *(str + i) - '0';
    }
    if(rval != 0.0)
        rval = rval * 1.0 / pow(10, pivot) * 1.0 * sign;

    return (rval);
}

long double __Str2LongDouble(str, len, pivot)
char *str;
int len;
int pivot;
{
    short i;
    long double rval = 0.0L;
    long double sign = 1.0L;

    for (i = 0; i < len; i++)
    {
        if(str[i] == '-') sign = -1.0;
//      if( (*(str + i)) != 0x20 )
        if( isdigit(*(str + i)) )
            rval = rval * 10.0L + (long double) (*(str + i) - '0');
    }
    if(rval != 0.0)
        rval = rval * 1.0L / pow(10, pivot) * 1.0L * sign;
    return (rval);
}

double __Str2dDouble(str, len, pivot)
char *str;
int len;
int pivot;
{
    short i;
    double rval = 0.0;

    for (i = 0; i < len; i++)
    {
        if ( str[i] == '.' )    continue;
        rval = rval * 10.0 + *(str + i) - '0';
    }
    if(len != 0)
    {
        rval = rval * 1.0 / pow(10, pivot) * 1.0 ;
    }
    return (rval);
}

double __SignStr2Double(str, len, pivot)
char *str;
int len;
int pivot;
{
    short   i, start_idx;
    double  rval = 0.0;

    if (*str >= '0' && *str <= '9')
    {
        start_idx = 0;
    }
    else
    {
        start_idx = 1;
    }

    for (i = start_idx; i < len; i++)
        rval = rval * 10.0 + *(str + i) - '0';
    rval = rval * 1.0 / pow(10, pivot) * 1.0 ;

    if (*str == '-')
    {
        rval = rval * -1;
    }
    return (rval);
}

double __SignStr2dDouble(str, len, pivot)
char *str;
int len;
int pivot;
{
    short   i, start_idx;
    double  rval = 0.0;

    if (*str >= '0' && *str <= '9')
    {
        start_idx = 0;
    }
    else
    {
        start_idx = 1;
    }

    for (i = start_idx; i < len; i++)
    {
        if (str[i] == '.') continue;
        rval = rval * 10.0 + *(str + i) - '0';
    }
    rval = rval * 1.0 / pow(10, pivot) * 1.0 ;

    if (*str == '-')
    {
        rval = rval * -1;
    }
    return (rval);
}

double __SignStr2Float(str, len, pivot)
char *str;
int len;
int pivot;
{
    short   i, start_idx;
    float  rval = 0.0;

    if (*str >= '0' && *str <= '9')
    {
        start_idx = 0;
    }
    else
    {
        start_idx = 1;
    }

    for (i = start_idx; i < len; i++)
        rval = rval * 10.0 + *(str + i) - '0';
    rval = rval * 1.0 / pow(10, pivot) * 1.0 ;

    if (*str == '-')
    {
        rval = rval * -1;
    }
    return ((float)rval);
}

double __STR2DBL (str, len)
char* str;
int len;
{
    char s_value[32];
    double f_value;

    memset(s_value, 0x00, sizeof(s_value));
    memcpy(s_value, str, len);
    f_value = atof(s_value);

    return f_value;
}


/************************************************************************/
/*      Number -> String                                                */
/************************************************************************/

void __Int2Str(str, value, len)
char *str;
int  value;
int  len;
{
    short i;

    memset(str, 0x30, len);
    for(i = len - 1; i >= 0; i--)
    {
        str[i] = (value % 10) + '0';
        value = value / 10;
    }
}

void __Int2Strb(str, value, len)
char *str;
int  value;
int  len;
{
    short i;

    memset(str, 0x20, len);
    for(i = len - 1; i >= 0; i--)
    {
        str[i] = (value % 10) + '0';
        value = value / 10;
        if (value == 0)
            break;
    }
}

void __Long2Str(str, value, len)
char *str;
long value;
int  len;
{
    short i;

    memset(str, 0x30, len);
    for(i = len - 1; i >= 0; i--)
    {
        str[i] = (value % 10) + '0';
        value = value / 10;
    }
}

void __Long2Strb(str, value, len)
char *str;
long value;
int  len;
{
    short i;

    memset(str, 0x20, len);
    for(i = len - 1; i >= 0; i--)
    {
        str[i] = (value % 10) + '0';
        value = value / 10;
        if (value == 0)
            break;
    }
}

int __IntGetTime(void)
{
    char str_time[16];
    int int_time = 0;
    time_t  longtm;
    time(&longtm);
    memset(str_time, 0x00, sizeof(str_time));
    strftime(&str_time[0], 16, "%H%M%S", localtime(&longtm));
    int_time = __Str2Int(str_time ,6);
    return(int_time);
}

char* __IntGetMillTime(void)
{
    struct	timeval  tv;
    struct	tm       *date, date1;
	char	*p_time;

    gettimeofday (&tv, NULL);
    date = (struct tm *)localtime_r (&(tv.tv_sec), &date1);

    /* yyyymmddhhmmss (14 bytes)    */
    strftime (p_time, 15, "%Y%m%d%H%M%S", date);
    sprintf (&p_time[14], "%03d", tv.tv_usec / 1000);

    return (p_time);
}

int __GetDateInt()
{
    char    date_tmp[9];
    time_t  tl;
    int     date_val;

    memset(date_tmp, 0x00, sizeof(date_tmp));

    time(&tl);
    strftime(date_tmp, sizeof(date_tmp), "%Y%m%d", localtime(&tl));
    date_val = __Str2Int(date_tmp,8);

    return(date_val);
}

/************************************************************************/
/*      Get Rate of Two numbers                                         */
/************************************************************************/
double  __D2DRate(diff, stdval)
double  diff;
double  stdval;
{
    double  rate = 0.0;
    char    buff[12];

    if( diff == 0.0 || stdval == 0.0 )
    {
        rate = 0.0;
    }
    else
    {
        rate = (diff*100) / fabs(stdval);
        sprintf(buff, "%8.2f", rate);
        rate = atof(buff);
    }

    return rate;
}

