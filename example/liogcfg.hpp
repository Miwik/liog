#ifndef LIOGCFG
#define LIOGCFG

#include "../liog.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// override the default config
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

liog_create()
liog_set(infos_display_file) = false;
liog_set(infos_display_line) = false;
liog_set(infos_style) = style::dark;
liog_set(dt_style) = style::dark;
liog_end

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// single params config to use by inheritance to create custom messages
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

liog_create(DISPLAY_FILE)
liog_set(infos_display_file) = true;
liog_end

liog_create(DISPLAY_LINE)
liog_set(infos_display_line) = true;
liog_end

liog_create(DISPLAY_DATE)
liog_set(dt_date_display) = true;
liog_end

liog_create(DISPLAY_TIME)
liog_set(dt_time_display) = true;
liog_set(dt_delimiters_fcolor) = fcolor::magenta;
liog_end

liog_create(BLUE_VALUES)
liog_set(msg_values_fcolor) = fcolor::blue;
liog_end

liog_create(BRIGHT_VALUES)
liog_set(msg_values_style) = style::bright;
liog_end

liog_create(NO_TAG)
liog_set(tag_display) = false;
liog_end

liog_create(NO_INFOS)
liog_set(infos_display) = false;
liog_end

liog_create(RED_TAG)
liog_set(tag_fcolor) = fcolor::red;
liog_end

liog_create(YELLOW_TAG)
liog_set(tag_fcolor) = fcolor::yellow;
liog_end

liog_create(CYAN_TAG)
liog_set(tag_fcolor) = fcolor::cyan;
liog_end

liog_create(RED_MSG)
liog_set(msg_fcolor) = fcolor::red;
liog_end

liog_create(YELLOW_MSG)
liog_set(msg_fcolor) = fcolor::yellow;
liog_end

liog_create(NOINFOS)
liog_set(msg_prefix) = nullptr;
liog_set(tag_display) = false;
liog_set(infos_display) = false;
liog_end

liog_create(NONEWLINE)
liog_set(msg_append_newline) = false;
liog_end

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// custom messages
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

liog_create(RAW, NOINFOS, NONEWLINE)
liog_end

liog_create(MSG, NO_TAG, NO_INFOS, BLUE_VALUES, DISPLAY_TIME/*, DISPLAY_DATE*/)
liog_end

liog_create(QUEUE, NO_INFOS, BLUE_VALUES, CYAN_TAG, DISPLAY_TIME)
liog_set(tag) = "QUEUE";
liog_end

liog_create(ERROR, DISPLAY_FILE, DISPLAY_LINE, RED_TAG, RED_MSG, DISPLAY_TIME, DISPLAY_DATE)
liog_set(tag) = "ERROR";
liog_set(infos_fcolor) = fcolor::red;
liog_end

liog_create(WARNING, DISPLAY_FILE, DISPLAY_LINE, YELLOW_TAG, YELLOW_MSG, DISPLAY_TIME)
liog_set(tag) = "WARNING";
liog_set(infos_fcolor) = fcolor::yellow;
liog_end

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // LIOGCFG
