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
#include "time-tracker_resources.h"

GBitmap  *ttracker_app_resources_get_icon() 
{
    GBitmap* image = gbitmap_create_with_resource(RESOURCE_ID_ICON_CALENDAR_ITEM);
    
    GRect bounds = gbitmap_get_bounds(image);
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "image has size %d x %d", bounds.size.w, bounds.size.h);
    return image;
}
    
