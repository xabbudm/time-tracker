#include <pebble.h>
#include "time-tracker_persistence.h"


static TPersistenceTimeTrackerDataPoint s_data_points[] = 
{
    {
        .name = "montag",
        .start_time = (time_t)0,
        .stop_time = (time_t)0,
        .pause_time = 0
    },
    {
        .name = "dienstag",
        .start_time = (time_t)0,
        .stop_time = (time_t)0,
        .pause_time = 0
    },
    {
        .name = "mittwoch",
        .start_time = (time_t)0,
        .stop_time = (time_t)0,
        .pause_time = 0
    },
    {
        .name = "donnerstag",
        .start_time = (time_t)0,
        .stop_time = (time_t)0,
        .pause_time = 0
    },
    {
        .name = "freitag",
        .start_time = (time_t)0,
        .stop_time = (time_t)0,
        .pause_time = 0
    },
    {
        .name = "samstag",
        .start_time = (time_t)0,
        .stop_time = (time_t)0,
        .pause_time = 0
    },
    {
        .name = "sonntag",
        .start_time = (time_t)0,
        .stop_time = (time_t)0,
        .pause_time = 0
    }
};

ETimeTrackerPersistenceState time_tracker_load_week_data()
{
    int bytes_read = 
        persist_read_data(TIME_PERSISTENCE_WEEK_TIME_KEY, s_data_points, sizeof(s_data_points));
    
    if (bytes_read != E_DOES_NOT_EXIST )
    {
        return TIME_TRACKER_PERSISTENCE_SUCCESS;
    }
    else
    {
        // create initial data
        time_tracker_init_for_new_week(true);
        
        if (persist_exists(TIME_PERSISTENCE_WEEK_TIME_KEY))
        {
            return TIME_TRACKER_PERSISTENCE_SUCCESS;
        }
        else 
        {
            return TIME_TRACKER_PERSISTENCE_READ_FAILED;
        }
    }
}

ETimeTrackerPersistenceState time_tracker_save_week_data()
{    
    int bytes_written = 
        persist_write_data(TIME_PERSISTENCE_WEEK_TIME_KEY, s_data_points, sizeof(s_data_points));
        
    if (bytes_written > 0)
    {
        return TIME_TRACKER_PERSISTENCE_SUCCESS;
    }
    else 
    {
        return TIME_TRACKER_PERSISTENCE_WRITE_FAILED;
    }
}

void time_tracker_init_for_new_week(bool save_persistent)
{
    for (int i = 0; i < 7; ++i)
    {
        s_data_points[i].start_time = (time_t)0;
        s_data_points[i].stop_time = (time_t)0;
        s_data_points[i].pause_time = 0;
    }
    
    if (save_persistent)
    {
        time_tracker_save_week_data();
    }
}

ETimeTrackerPersistenceState time_tracker_save_data_point(TTrackerAppDataPoint* data_point)
{
    if (0 != data_point)
    {
        if (data_point->start > 0)
        {
	    struct tm* tm_data_point = localtime(&(data_point->start));
        
            // save a copy of start time
            struct tm start_time = *tm_data_point;
            
            /* ***
            // initialize stop_time
            struct tm stop_time = start_time;
            uint8_t hours = start_time.tm_hour;
            
            if ((hours + 8) < 24) 
            {
                stop_time.tm_hour = hours + 8;
            }
            else
            {
                stop_time.tm_hour = 23;
                stop_time.tm_min = 59;
            }                        
            
            */
            TPersistenceTimeTrackerDataPoint* persistence_data_point = 
                &s_data_points[start_time.tm_wday];
            
            persistence_data_point->start_time = data_point->start;
            persistence_data_point->stop_time = data_point->stop;
            persistence_data_point->pause_time = data_point->pause;
            
            return time_tracker_save_week_data();
        }
        else
        {            
            return TIME_TRACKER_PERSISTENCE_NO_START_TIME_SET;
        }
    }
    else 
    {
        return TIME_TRACKER_PERSISTENCE_INVALID_DATA;
    }

    return TIME_TRACKER_PERSISTENCE_SUCCESS;
}
