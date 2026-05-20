#include "types.h"
#include "date_utils.h"

u8 is_leap_year(u32 year)
{
    if((year % 400) == 0)
        return 1;
    else if((year % 100) == 0)
        return 0;
    else if((year % 4) == 0)
        return 1;
    else
        return 0;
}

u32 days_in_month(u32 month, u32 year)
{
    switch(month)
    {
        case 1:  return 31;
        case 2:  return is_leap_year(year) ? 29 : 28;
        case 3:  return 31;
        case 4:  return 30;
        case 5:  return 31;
        case 6:  return 30;
        case 7:  return 31;
        case 8:  return 31;
        case 9:  return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: return 0;
    }
}

u8 is_valid_date(u32 date, u32 month, u32 year)
{
    u32 max_days;

    if(year > 4095)
        return 0;

    if(month < 1 || month > 12)
        return 0;

    max_days = days_in_month(month, year);

    if(date < 1 || date > max_days)
        return 0;

    return 1;
}
