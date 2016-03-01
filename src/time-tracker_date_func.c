/*
 * Copyright 2015 Daniel Muhle (dmuhle at gmail dot com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <pebble.h>
#include "time-tracker_date_func.h"

// c.f https://en.wikipedia.org/wiki/ISO_week_date#Calculating_the_week_number_of_a_given_date

static int get_week_of_year(time_t date)
{
    struct tm now_tm = *(localtime(&date));
    
    int ordinal_date = now_tm.tm_yday;
    int week_day = 0 == now_tm.tm_wday ? 7 : now_tm.tm_wday; 
    
    int week = (ordinal_date - week_day + 10) / 7; // intentional use of integer arithmetic
  
    if (week > 0)
    {
        if (week < 53)
        {
            return week;            
        }        
        else
        {
            struct tm lastWeek;
            memset(&lastWeek, 0, sizeof(struct tm));
            
            lastWeek.tm_mday = 31;
            lastWeek.tm_mon = 11;
            lastWeek.tm_year = now_tm.tm_year;
            
            time_t sylvester = mktime(&lastWeek); // use sideeffect that mktime initialize all parts of struct tm
            
            int lastWeek_i32 =  get_week_of_year(sylvester);
            
            if (week > lastWeek_i32)
            {
                return 1;
            }
            else
            {
                return week;
            }
        }
    }
    else
    {
        struct tm lastWeekPrevYear;
        memset(&lastWeekPrevYear, 0, sizeof(struct tm));
        
        lastWeekPrevYear.tm_mday = 31;
        lastWeekPrevYear.tm_mon = 11;
        lastWeekPrevYear.tm_year = now_tm.tm_year - 1;
        
        time_t sylvester = mktime(&lastWeekPrevYear); // use sideeffect that mktime initialize all parts of struct tm
        
        return get_week_of_year(sylvester);
    }
    
}

int time_tracker_get_current_calendar_week()
{
    time_t now = time(NULL);
    
    return get_week_of_year(now);
}