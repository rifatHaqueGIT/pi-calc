// ======================================================================
// You must modify this file and then submit it for grading to D2L.
// ======================================================================
//
// count_pi() calculates the number of pixels that fall into a circle
// using the algorithm explained here:
//
// https://en.wikipedia.org/wiki/Approximations_of_%CF%80
//
// count_pixels() takes 2 paramters:
//  r         =  the radius of the circle
//  n_threads =  the number of threads you should create
//
//
#include "calcpi.h"
#include <vector>
#include <thread>
//Struct to send
struct Task{
 double startX =0, endX =0;
  uint64_t partial_count;

};
void piCount(Task &task, int r){
  double rsq = double(r) * r; // r squared
  task.partial_count = 0;// make sure the partial count is initialized to zero before we count
  for( double x = task.startX ; x <= task.endX ; x ++)//same algorithm as before
    for( double y = 0 ; y <= r ; y ++)
      if( x*x + y*y <= rsq) task.partial_count ++;
    
}
uint64_t count_pixels(int r, int n_threads)
{
  
  std::vector <std::thread> threads; // a vector to hold the threads
  Task task[n_threads]; //an array of tasks
  uint64_t count = 0;
  //Works for n_threads <= r-1
  for(int i = 0 ; i < n_threads; i++){
    if(i == 0)
     task[i].startX = 1; //The first task will always start at 1
    if(task[i-1].endX < r)//if the end of the last task is less then the max which is r
      task[i].startX = task[i-1].endX +1; //set this task to be 1 plus the last task
    else
      task[i].startX = task[i-1].endX; //otherwise don't add 1

    task[i].endX = task[i].startX + (r-1)/n_threads; // All tasks will intially have this as their end. (r-1)/n_threads is the rough size of each partition

    if(i == n_threads-1)//last entry in the partition
      task[i].endX = r;// makes sure the last value is included in the last partition (r must be the end or the entire count wont be covered)
    if(task[i].endX > r)// makes sure the last index will not go beyond r 
      task[i].endX = r; //set to r as ceiling
    if(task[i].startX >= task[i].endX) // This is when the threads cannot partition the radius anymore
      {
        task[i-1].endX = r; // this is used to make the previous entires end equal to the radius. This would be the last value of the last partition.
        n_threads = i-1;// Decrease the n_threads to ignore this case where end and start equal each other.
        break;//Break as no additional threads are needed.
      }
  }
 
  for(int i = 0 ; i < n_threads; i++)
    threads.emplace_back(piCount, std::ref(task[i]),r);
  for(auto & h : threads)
    h.join();
  for(int i = 0; i < n_threads; i++) //adding up partial-counts
    count += task[i].partial_count;
 
 
 
  return count * 4 + 1;
}

