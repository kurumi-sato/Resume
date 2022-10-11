#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "event.h"
#include "calendar.h"

/* Kurumi Sato UID: ksato1*/

/* This function initializes a Calendar struct based on the parameters. 
   The function allocates memory for calendar struct, chunk to memory pointed by
   the name field, name, event pointer, array of event structures, length of 
   array given by parameter of days. */
int init_calendar(const char *name, int days,
		  int (*comp_func) (const void *ptr1, const void *ptr2), 
		  void (*free_info_func) (void *ptr), Calendar ** calendar) {
  (*calendar) = malloc(sizeof(Calendar));

  if((*calendar) == NULL || name == NULL || days < 1) {
    return FAILURE;
  }

  /* Allocate memory for name */
  (*calendar)->name = (char *)malloc(strlen(name) + 1);
  (*calendar)->events = calloc(days, sizeof(Event));

  /* If memory allocation fails, return FAILURE */
  if (!(*calendar)->events || !(*calendar)->name) {
    return FAILURE;
  }

  strcpy((*calendar)->name, name);

  /* Copy name and days, comp function, and free info function */
  (*calendar)->days = days;
  (*calendar)->total_events = 0;
  (*calendar)->comp_func = comp_func;
  (*calendar)->free_info_func = free_info_func;

  return SUCCESS;
}

/* This function prints, to the designated output stream, the calendar’s name, 
days, and total number of events if print all is true; otherwise this 
information is not printed. Information about each event (name, start time, 
and duration) is printed regardless of the value of print all.*/

int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
  Event *current;
  int i;

  if (calendar == NULL || output_stream == NULL) {
    return FAILURE;
  }

  /* Print out the header*/
  if (print_all) {
    fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
    fprintf(output_stream, "Days: %d\n", calendar->days);
    fprintf(output_stream, "Total Events: %d\n\n", calendar->total_events);
  }

  /* Events title Header*/
  fprintf(output_stream, "**** Events ****\n");

  /* If the total number of events is greater than zero */
  if (calendar->total_events > 0) {
    /* Print all of the events */

    /* Go through all of the days  */
    for (i = 0; i < calendar->days; i++) {
      fprintf(output_stream, "Day %d\n", i + 1);
      /* Go through each event within the day */
      current = calendar->events[i];
      /* While the current event is not empty*/
      while (current != NULL) {
	fprintf(output_stream, "Event's Name: \"%s\", ", current->name);
	fprintf(output_stream, "Start_time: %d, ", current->start_time);
	fprintf(output_stream, "Duration: %d \n", current->duration_minutes);
	current = current->next;
      }
    }
  } /* If total number of events is zero then it still returns success
     but nothing is printed*/
  return SUCCESS; 
}

/* This function adds the specified event to the event list associated with 
parameter day, ensuring that the resulting event list is in increasing sorted 
order according to the comparison function (field comp func). If the new event 
and an existing event in the event list have the same comp func value, the new 
event is added before the existing event. The function allocates memory for the
 new event and for the event’s name. */
int add_event(Calendar *calendar, const char *name, int start_time, 
	      int duration_minutes, void *info, int day) {
  Event *new_event, *previous, *current;

  /* If any of the parameters don't make sense return FAILURE */
  if (!calendar || !name || start_time < 0 || start_time > 2400 || 
    duration_minutes <= 0 || day <=0 || day > calendar->days) {
    return FAILURE;
  }

  /* If the event already currently exists in the calendar, return FAILURE*/
  if (find_event(calendar, name, &new_event) == SUCCESS) {
    return FAILURE;
  }

  /* Allocate memory */
  new_event = malloc(sizeof(Event));
  new_event->name = (char *)malloc(strlen(name) + 1);

  if (!new_event || !new_event->name) {
    return FAILURE;
  }

  /* Create the new event*/
  strcpy(new_event->name, name);
  new_event->start_time = start_time;
  new_event->duration_minutes = duration_minutes;
  new_event->info = info;

  /* Set first event as your current event */
  current = calendar->events[day - 1];

  /* Add event to the front of the linked list  */
  if (current == NULL) {
    calendar->events[day - 1] = new_event;
    new_event->next = NULL;

  } else {
    /* Add event to front of list */
    if (calendar->comp_func(current, new_event) > 0) {
      calendar->events[day - 1] = new_event;
      new_event->next = current;

      /* Add to the end of the list*/
    } else {
      previous = current;
      current = current->next;
      
      while (current != NULL) {
	if(calendar->comp_func(current, new_event) > 0) {
	  break;
	}
	previous = current;
	current = current->next;
      }

      new_event->next = current;
      previous->next = new_event;
    }
  } /* end of else statement */ 
  (calendar->total_events)++;
  return SUCCESS;
}

/* This function returns SUCCESS if calendar has an event with the specified
   name. In this case, it returns a pointer to that event via parameter event 
   iff the parameter is not NULL (no pointer is returned if parameter event is 
   NULL). The function returns FAILURE if calendar and/or name are NULL, or if 
   calendar does not have an event with the specified name.*/

int find_event(Calendar *calendar, const char *name, Event **event) {
  Event *current;
  int i;

  if (calendar == NULL || name == NULL || event == NULL)  {
    return FAILURE;
  }

  /* Go through all days to find the event */
  for (i = 0; i < calendar->days; i++) {
    current = calendar->events[i];

    /* Go through all of the events during that day */
    while (current != NULL) {
      /* If the event is found, then set current to the event pointer and then 
	 return SUCCESS*/
      if (strcmp(current->name, name) == 0) {
	*event = current;
	return SUCCESS;
      }
      /* Check next event*/
      current = current->next;
    }
  }
  /* If not found, return FAILURE */
  return FAILURE;
}

/* This function returns SUCCESS if calendar has an event with the specified 
   name in the specified day. In this case, it returns a pointer to that event 
   via parameter event if the parameter is not NULL.*/
int find_event_in_day(Calendar *calendar, const char *name, int day, 
		      Event **event) {
  Event *current;

  if (calendar == NULL || name == NULL || day > calendar->days || day < 1 ||
      event == NULL || find_event(calendar, name, event) == FAILURE)  {
    return FAILURE;
  }
  current = calendar->events[day - 1];

  /* Go through specified day to find the event */
  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      *event = current;
      return SUCCESS;
    }
    /* If not equal, move to the next event */
    current = current->next;
  }
  /* If not found, return FAILURE */
  return FAILURE;
}

/* 
This function returns SUCCESS if the calendar has the specified event. In this 
case, it removes the event from the calendar, updates the number of events, and 
frees any memory allocated for the event. It calls the free info func on the 
event’s info field iff both of these are non-NULL. This function returns 
FAILURE if calendar and/or name are NULL, or if the event is not in the 
calendar.*/
int remove_event(Calendar *calendar, const char *name) {
  Event **event = NULL;
  Event *previous, *current;
  int i;

  /* If calendar and//or name is NULL or if the event cannot be found, return 
     failure */
  if (!calendar || !name) {
    return FAILURE;
  }

  /* If the event does not exists, return FAILURE */
  if (find_event(calendar, name, event) == FAILURE) {
    return FAILURE;
  }
  /* Decrease total events by one */
  calendar->total_events--;

  for (i = 0; i < calendar->days; i++) {
    current = calendar->events[i];

    /* If current event is the same as the event wanting to be removed*/
    if (strcmp(current->name, name) == 0) {
	calendar->events[i] = current->next;
	current->next = NULL;
    } else { /* Otherwise move forward */
      while (current != NULL) {
          previous = current;
          current = current->next;
      }
      previous->next = current->next;
      current->next = NULL;
    }
    
    free(current->name);
    if(calendar->free_info_func) {
      calendar->free_info_func(current->info);
    }
    /* Free all memory allocated for the event */
    current->name = NULL;
    free(current->name);

  /* Free the event itself*/
  free(current);
  (calendar->total_events)--;

  } 
  return SUCCESS;
}

/* This function returns the info pointer associated with the specified event. 
The function returns NULL if the event is not found. Assume the calendar and 
name parameters are not NULL.*/
void *get_event_info(Calendar *calendar, const char *name) {
  Event *event;

  if (find_event(calendar, name, &event) != FAILURE) {
    return event->info; 
  }
  return NULL;
}

/* This function returns SUCCESS if calendar is not NULL. In this case, it 
removes every event (processing the event as described in function remove 
event) and sets every event list to empty. The array of pointers to event lists
 is not removed. */
int clear_calendar(Calendar *calendar) {
  int i;
  if (calendar != NULL){
    for(i = 0; i < calendar->days; i++){
      clear_day(calendar, i+1);
    }
    return SUCCESS;
  }
  return FAILURE;
}

/* This function returns FAILURE if calendar is NULL or if day is less than 1 
or greater than the number of calendar days. Otherwise it removes all the 
events for the specified day (processing each event as described in function 
remove event), sets the day’s event list to empty, and returns SUCCESS.*/
int clear_day(Calendar *calendar, int day) {
  Event *current = NULL , *previous = NULL;

  if (calendar == NULL || day < 1 || day > calendar->days ||
      calendar->total_events <= 0) {
    return FAILURE;
  }

  current = calendar->events[day - 1];
  while (current) {
    previous = current;
    current = current->next;

    free(previous->name);
    if (calendar->free_info_func && previous->info) {
      calendar->free_info_func(previous->info);
    }

    free(previous);
    (calendar->total_events)--;
  }
  calendar->events[day - 1] = NULL; 
  return SUCCESS;
}

/* This function returns FAILURE if calendar is NULL. Otherwise it removes 
every event (processing the event as described in function remove event), 
frees all memory that was dynamically allocated for the calendar, and returns 
SUCCESS. */
int destroy_calendar(Calendar *calendar) {
  if (calendar == NULL) {
    return FAILURE;
  }

  free(calendar->name);
  clear_calendar(calendar);
  free(calendar->events);
  free(calendar);

  return SUCCESS;
}