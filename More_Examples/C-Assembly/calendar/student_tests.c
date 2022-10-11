#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/* Description here: This test checks ...  */
static int test1() {
   int days = 7;
   Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/* This test helps you see if you can accurately add an event and remove an
 event from the middle of the linked list but also what happens when you try 
 to remove an event that does not exist. Then destroys the calendar. Also 
 tests the legitimacy of the start time and duration*/
static int test2() {
  int days = 7;
  Calendar *calendar;

  printf("\nTest 2\n"); 
  if(init_calendar("Calendar", days, comp_minutes, NULL, &calendar) == SUCCESS){
    
    add_event(calendar, "study1", 1000, 20, NULL, 1);
    add_event(calendar, "study2", 1200, 60, NULL, 1);
    add_event(calendar, "study3", 1300, 30, NULL, 2); 
    add_event(calendar, "study4", 2500, 120, NULL, 3);
    add_event(calendar, "study100", 1400, 0, NULL, 3);
    add_event(calendar, "take the exam", 900, 1500, NULL, 4);
    add_event(calendar, "cry from the result of my exam", 1500, 90, NULL, 5);
    print_calendar(calendar, stdout, 1);

    if(remove_event(calendar, "does not exist")) {
      printf("does not exist\n");
    }

    if(remove_event(calendar, "study2") == SUCCESS){
      if(clear_day(calendar, 5) == SUCCESS){
	print_calendar(calendar, stdout, 0);

	if(clear_calendar(calendar) == SUCCESS){
	  print_calendar(calendar, stdout, 1);
	  return destroy_calendar(calendar);
	}
      }
    }
  }
  return FAILURE;
}

/* Sees whether the events in the calendar are defined properly. Also tests
   getting any of the information in an event. Also tests what happens
   when you add the same event twice. 
*/
static int test3() {
  int days = 31;
  Calendar *calendar;
  Event* event;
   
  printf("\nTest 3\n");

  if(init_calendar("Calendar", days, comp_minutes, NULL, &calendar) 
     == SUCCESS){

    /* Adding events */
    add_event(calendar, "monday", 1400, 3, NULL, 3);
    add_event(calendar, "tuesday", 900, 200, NULL, 2);

    print_calendar(calendar, stdout, 1);


    if(add_event(calendar, "wednesday", 800, 20, NULL, 32)== FAILURE){
      printf("cannot print day 32\n");
    }

    /* add same event twice */
    if(add_event(calendar, "monday", 1, 1, NULL, 1) == FAILURE){
      printf("added twice\n");
    }

    if (find_event(calendar, "monday", &event) == SUCCESS) {
      printf("Event found: %s, %d, %d\n", event->name, event->start_time,
	     event->duration_minutes);
    } else {
      printf("not found\n");
    }

    if (find_event_in_day(calendar, "tusday", 2, &event) == SUCCESS) {
      printf("Event found: %s, %d, %d\n", event->name, event->start_time,
	     event->duration_minutes);
    } else {
      printf("not tuesday");
    }
    return destroy_calendar(calendar);
  }
  return FAILURE;
}

/* Test 4 That tests to find an event on a specific day */
static int test4() {
  Calendar *calendar;
  Event *event;
  int total_days = 31;
  if (init_calendar("calendar", total_days, comp_minutes, NULL, &calendar)
      == SUCCESS) {
    add_event(calendar, "day 1", 1400, 30, NULL, 2);
    if(find_event_in_day(calendar, "day 1", 2, &event) == SUCCESS) {
      return destroy_calendar(calendar);
    }
  }
  return FAILURE;
}

/* Test 5 tests removing an event from the front and the end of a list */
static int test5() {
  Calendar *calendar;

  if (init_calendar("calendar", 3, comp_minutes, NULL, &calendar)
      == SUCCESS) {
    add_event(calendar, "event 1", 600, 30, NULL, 1);
    add_event(calendar, "event 2", 1500, 50, NULL, 1);
    add_event(calendar, "event 3", 900, 40, NULL, 1);
    add_event(calendar, "event 4", 2100, 30, NULL, 2);

    if (remove_event(calendar, "event 1") == SUCCESS && 
	remove_event(calendar, "event 4") == SUCCESS) {
      return destroy_calendar(calendar);
    }
  }
  return FAILURE;
}


int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) result = FAILURE;
   if (test2() == FAILURE) result = FAILURE;
   if (test3() == FAILURE) result = FAILURE;
   if (test4() == FAILURE) result = FAILURE;
   if (test5() == FAILURE) result = FAILURE;

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}