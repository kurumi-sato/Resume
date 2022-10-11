#include <stdio.h>
#include <math.h>

#define MAX_ASSIGNMENTS 50

/* Scans all of the assigments data */
void scan_data(int N, int assignments[]) {
   int i;
   for (i = 0; i < (N * 4); i += 4) {
     scanf(" %d, %d, %d, %d", &assignments[i], &assignments[i+1], 
	   &assignments[i+2], &assignments[i+3]);   
   }
}

/* Drops an assignment by setting weight to 0 */
void drop_scores(int assignments[], int N, int X) {
   int i, lowest_grade, curr_grade, index, drop_index = 0;

   /* Drops number of lowest assignments given, goes through every 
      assignment*/
   for (index = 0; index < X; index++) {

      /* Finds the lowest assignment score, first arbitrarily assigning the 
         first assignment as the lowest*/
      lowest_grade = assignments[1] * assignments[2];
      for (i = 0; i < (N * 4); i += 4) {
          curr_grade = assignments[i+1] * assignments[i+2];
          /* Check to make sure the current grade is not zero */
          if (curr_grade != 0) {
              /* If current grade is lower than the lowest grade, set lowest 
		 grade to current*/
              if (curr_grade < lowest_grade) {
                  lowest_grade = curr_grade;
                  drop_index = i;
              }
          }
      }
      /* Set lowest assignment weight equal to zero so it does not get 
	 calculated in the score */
      assignments[drop_index + 2] = 0;
   }
}

/* Calculate the numeric score by dropping assignments, accounting for weights 
and late penalties */
static double calculate_score(int assignments[], int N, int P, int X) {
   int i, grade;
   double weight_sum = 0, score = 0;

   /* Drop lowest score if user inputs more than 0 assignments to drop*/
   if (X > 0) {
      drop_scores(assignments, N, X);
   }

   /* Calculate new weigt and sum after assignments are dropped */
   for (i = 0; i < (N * 4); i += 4) {
	 weight_sum += assignments[i+2];
   }

   /* If the sum of the weight is greater than 100, print invalid */
   if (weight_sum > 100.0) {
      printf("ERROR: Invalid values provided\n");
      return 0;
      
   /* Otherwise, calculate numeric score*/
   } else {
       for (i = 0; i < (N * 4); i += 4) {
	 /* Calculate the grade */
	 grade = assignments[i+1] - ( assignments[i+3] * P);
	 score += (grade * (assignments[i+2]) / weight_sum);
       }
   }
   return score;
}

/* Calculate the mean where weight and penalties are used */
static double calculate_mean (int assignments[], int N, int P) {
   int i;
   double mean, sum = 0;

   /* compute the mean of all assign scores */
   for (i = 0; i < (N * 4); i += 4) {
      sum += (assignments[i+1] - (assignments[i+3]) * P);
   }
   mean = (sum / N);
   return mean;
}

/* Calculate the standard deviation of taking account weight and late penalty */
static double calculate_sd(int assignments[], int N, int X) {
   int i, grade;
   double mean, variance, sd, sum = 0;
   
   /* Find the mean accounting for late penalty and weight*/
   mean = calculate_mean(assignments, N, X);
   
   for (i = 0; i < (N * 4); i += 4) {
      grade = assignments[i+1] - (assignments[i+3] * X);
      sum += pow((grade - mean), 2);
   }
   variance = sum / (float) N;
   sd = sqrt(variance);
   
   return sd;
}

/* Print the values of all of the assignments*/
void print_values(int N, int assignments[]) {
   int count = 1, i;
   /* print in groups of four from the next largest num */
   
   while (count <= N) {
     for (i = 0; i < (N * 4); i += 4) {
       if (assignments[i] == count) {
	 printf("%d, %d, %d, %d\n", assignments[i], assignments[i+1], 
		assignments[i+2], assignments[i+3]);
       }
     }
     count++;
   }
}

/* Prints program output */
/* Numeric score, late penalty, dropped, assignment list, and stats if 'Y' */
static void print_output(int assignments[], int N, int P, int X, char W) {
   int i, temp_array[MAX_ASSIGNMENTS * 4];
   double score, mean = 0, sd = 0;
   
   /* Copies the assigment array into a temporary array */
   for (i = 0; i < (N * 4); i++) {
      temp_array[i] = assignments[i];
   }
   /* If number of assignments dropped is greater than zero but less than 
      assigments, calculate the numeric score. Otherwise, set score to 0 */
   if (0 <= X && X < N) {
       score = calculate_score(temp_array, N, P, X);
   } else {
       score = 0;
   }

   /* Print numeric score, late penalty, and number of dropped assigments */
   printf("Numeric Score: %5.4f\n", score);
   printf("Points Penalty Per Day Late: %d\n", P);
   printf("Number of Assignments Dropped: %d\n", X);

   /* Print assigments data */
   printf("Values Provided:\n");
   printf("Assignment, Score, Weight, Days Late\n");
   print_values(N, assignments);
   
   /* Print stats if the input character was yes */
   if (W == 'Y' || W == 'y') {
      mean = calculate_mean(assignments, N, P);
      sd = calculate_sd(assignments, N, P);
      printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean, sd);
   }
}

/* Main */
int main() {
    /* N is the number of assignments */
    /* P is the points penalty per day late*/
    /* X is the number of assignments to drop*/
    /* W is whether statistial number is to be generated*/
   int P, X, N;
   char W;
   
   /* Create an array for the list of assignments*/
   int assignments[MAX_ASSIGNMENTS * 4];
   
   /* Scan data */
   scanf(" %d %d %c", &P, &X, &W);
   scanf(" %d", &N);
   scan_data(N, assignments);
   
   /* Print data if number of assigments, late penalty and dropped are not 
      negative*/
   if (!(N < 0) && !(P < 0) && !(X < 0)) {
      print_output(assignments, N, P, X, W);
   }
   /* End program */
   return 0;
}
