#include <stdlib.h>

#include "multilevelQueueScheduler.h"

int doesScheduleHaveFore(schedule *ps);
void attemptPromote( schedule *ps );
int min( int x, int y );

static const int STEPS_TO_PROMOTION = 50;
static const int FOREGROUND_QUEUE_STEPS = 5;

/* printNames
 * input: none
 * output: none
 *
 * Prints names of the students who worked on this solution
 */
void printNames( )
{
    /* TODO : Fill in you and your partner's names (or N/A if you worked individually) */
    printf("\nThis solution was completed by:\n");
    printf("Yesenia Molano Sanchez\n");
    printf("N/A\n\n");
}

/* createSchedule
 * input: none
 * output: a schedule
 *
 * Creates and return a schedule struct.
 */
schedule* createSchedule( ) {
    /* TODO: malloc space for a new schedule and initialize the data in it */
	schedule *ps = (schedule*)malloc(sizeof(schedule));
	if(ps == NULL)
	{
		printf("Failed malloc space for ps!\n");
		exit(-1);
	}
	ps->foreQueue = createQueue();
	ps->backQueue = createQueue();
	ps->timestep = 0;
	
    	return ps; /* TODO: Replace with your return value */
}

/* isScheduleUnfinished
 * input: a schedule
 * output: bool (true or false)
 *
 * Check if there are any processes still in the queues.
 * Return TRUE if there is.  Otherwise false.
 */
bool isScheduleUnfinished( schedule *ps ) {
    /* TODO: check if there are any process still in a queue.  Return TRUE if there is. */
//	printf("---isScheduleUnfinished\n");    	
	if( ps->foreQueue->qFront==NULL && ps->foreQueue->qRear==NULL && ps->backQueue->qFront == NULL && ps->backQueue->qFront == NULL)
    	{
		//printf("returns false\n");
        	return false;
	}
//	else if( (ps->foreQueue->qFront==NULL || ps->foreQueue->qRear==NULL) || (ps->backQueue->qFront==NULL || ps->backQueue->qRear==NULL) )
//    	{
//		printf("is this where?\n");
//        	fprintf( stderr, "isEmpty: Queue had inconsistent values for front and rear.\n" );
//        	exit(-1);
//	}
//	printf("returns true\n");
	return true; /* TODO: Replace with your return value */
}

/* addNewProcessToSchedule
 * input: a schedule, a string, a priority
 * output: void
 *
 * Create a new process with the provided name and priority.
 * Add that process to the appropriate queue
 */
void addNewProcessToSchedule( schedule *ps, char *processName, priority p ) {
    /* TODO: complete this function.
    The function "initializeProcessData" in processSimulator.c will be useful in completing this. */
//	printf("---addProcessToSchedule\n");
	queueType qt = (process*)malloc(sizeof(process));
	// checks if malloc worked
	if(qt == NULL)
	{
		printf("Failed to malloc space for qt!\n");
		exit(-1);
	}
	if(p == 0) // forerground process
	{	
		// mallocing space for processName
		qt->processName = (char*)malloc(sizeof(char));
		qt->processName = processName;	
		qt->p = p;
		qt->timestepAdded = ps->timestep; 
		qt->timestepPro = FOREGROUND_QUEUE_STEPS + (qt->timestepAdded); 
		qt->data = initializeProcessData(processName);
		qt->stepsuntil = (qt->timestepPro) - (qt->timestepAdded); 
		enqueue(ps->foreQueue, qt);
	}
	else if(p == 1) // background process
	{
		// mallocing space for processName
		qt->processName = (char*)malloc(sizeof(char));
		qt->processName = processName;
		qt->p = p;
		qt->timestepAdded = ps->timestep;
		qt->timestepPro = STEPS_TO_PROMOTION+(qt->timestepAdded);
		qt->data = initializeProcessData(processName);
		qt->stepsuntil = (qt->timestepPro)-(qt->timestepAdded);
		enqueue(ps->backQueue, qt);
		ps->backQueue->qRear->pNext = NULL;
	}
// DONE!   	free( processName ); /* TODO: This is to prevent a memory leak but you should remove it once you create a process to put processName into */
//	free(qt); - TODO: this causes an error need to free qt somewhere else!!
}

/* runNextProcessInSchedule
 * input: a schedule
 * output: a string
 *
 * Use the schedule to determine the next process to run and for how many time steps.
 * Call "runProcess" to attempt to run the process.  You do not need to print anything.
 * You should return the string received from "runProcess".  You do not need to use/modify this string in any way.
 */
char* runNextProcessInSchedule( schedule *ps ) {
    /* TODO: complete this function.
    	// TODO: need to free processName in here after a process finishes
    The function "runProcess", "loadProcessData", and "freeProcessData"
    in processSimulator.c will be useful in completing this.*/
//	printf("in runNextProcess\n");
	char *ret = NULL;
    	int numSteps;
    	int mini;
	bool b;
	int backsteps;
	int foresteps;
//	printf("INITIAL CALL TO ATTEMPT PROMOTE\n");
	attemptPromote( ps ); /* This will call your attemptPromote code. */
	    
	 /* TODO: Replace 0 with the number ef steps the next process should be run for.  It may help to use min. */


    /* TODO: Delete the following printf once you get the infinite loop fixed */
//    printf("IMPORTANT NOTE: There will be an infinite loop in runNextProcessInSchedule if you get isScheduleUnfinished and addNewProcessToSchedule working correctly\n"); 
    /* TODO: Uncomment the code below to dequeue elements from the two Queues and break your code out of the infinite loop.*/
    
//    if( !isEmpty(ps->foreQueue) )
  //      dequeue(ps->foreQueue);
  //  else if( !isEmpty(ps->backQueue) )
  //      dequeue(ps->backQueue);
    
    
	int call = doesScheduleHaveFore(ps);
        if(call == 1) // this means that there are still fore process in schedule and run the fore process
        {
                // calling loadProcessData
//		printf("what is at the fron of foreQueue before loading: %s\n", ps->foreQueue->qFront->qt->processName);
//		printf("---loadProcessData()\n");
                loadProcessData(ps->foreQueue->qFront->qt->data);
//		printf("after --- loadProcessData\n");
	
		//updating timestep added here
		ps->foreQueue->qFront->qt->timestepAdded = ps->timestep;
//		printf("what is at front of in foreQueue after loaded: %s\n", ps->foreQueue->qFront->qt->processName);

		// updating values in forqueue
//		printf("what is steps until beofre updating it: %d\n", ps->foreQueue->qFront->qt->stepsuntil);
		if(ps->foreQueue->qFront->qt->stepsuntil == 5 || (ps->foreQueue->qFront->qt->stepsuntil == 0) || (ps->foreQueue->qFront->qt->stepsuntil == 50))
		{
//			printf("ENTERED TO UPDATE STEPSUNTIL\n");
			ps->foreQueue->qFront->qt->timestepPro = 5+(ps->foreQueue->qFront->qt->timestepAdded); //timestep number to move to bakc of queue
			ps->foreQueue->qFront->qt->stepsuntil = (ps->foreQueue->qFront->qt->timestepPro)-(ps->timestep); //times steps until moving to the back of queue
		}
		//updating value in backqueue
		if(ps->backQueue->qFront != NULL && ps->backQueue->qRear != NULL)
		{
			ps->backQueue->qFront->qt->timestepPro = 50+(ps->backQueue->qFront->qt->timestepAdded); //same
                	ps->backQueue->qFront->qt->stepsuntil = (ps->backQueue->qFront->qt->timestepPro)-(ps->timestep); //same
		}

		// finding the minimum
		foresteps = (ps->foreQueue->qFront->qt->stepsuntil);
		
        	if(ps->backQueue->qFront != NULL && ps->backQueue->qRear != NULL) 
		{
//			printf("BACKSTEPS HERE!!\n");
			backsteps = (ps->backQueue->qFront->qt->stepsuntil);
		}
		else if(ps->backQueue->qFront == NULL && ps->backQueue->qRear == NULL)
		{
			backsteps = 10000;
		}
		
		if(backsteps == 10000)
			mini = foresteps;
		else{
			mini = min(foresteps,backsteps);
		}
        
 //       	printf("what is foresteps: %d\n", foresteps);
   //     	printf("what is backsteps: %d\n", backsteps);
//	        printf("what is min: %d\n", mini);
		
		numSteps = mini;

//		printf("---runProcess()\n");
		//TODO: if the number of steps to promote backQueue is less than 
		//      the number to move foreQueue to the back of queue
                b = runProcess(ps->foreQueue->qFront->qt->processName, &ret, &numSteps);
//		printf("---after runProcess()\n");
                ps->timestep+=numSteps;
		//printf("what is numsteps and what is b: %d\t%d", numSteps, b);
		
		// need to update the stepsuntil for forqueue and back queue
		ps->foreQueue->qFront->qt->stepsuntil = (ps->foreQueue->qFront->qt->timestepPro)-(ps->timestep); 
		if(ps->backQueue->qFront != NULL && ps->backQueue->qRear != NULL)
			ps->backQueue->qFront->qt->stepsuntil = (ps->backQueue->qFront->qt->timestepPro)-(ps->timestep); 
		//else if(ps->backQueue->qFront == NULL && ps->backQueue->qRear == NULL)
		//	ps->backQueue->qFront->qt->stepsuntil = 50;


//		printf("what is stepsuntil for current process: %d\n", ps->foreQueue->qFront->qt->stepsuntil);
		if(b == false && (ps->foreQueue->qFront->qt->stepsuntil == 0))
                {
//			printf("PROCESS NOT COMPLETE AND STEPSUNTIL IS EQUAL TO 0\n");
			process* qt1 = (process*)malloc(sizeof(process));
			qt1 = dequeue(ps->foreQueue);
			//printf("what is qt1: %s\n", qt1->processName);
			enqueue(ps->foreQueue, qt1);
			// updating timestepAdded for next forequeue and back queue
                	ps->foreQueue->qFront->qt->timestepAdded = ps->timestep;
//              	ps->backQueue->qFront->qt->timestepAdded = ps->timestep;
	
	
                	// updating timestep Pro for froequeue and backqueue
                	ps->foreQueue->qFront->qt->timestepPro = 5+(ps->foreQueue->qFront->qt->timestepAdded);
//              	ps->backQueue->qFront->qt->timestepPro = 50+(ps->backQueue->qFront->qt->timestepAdded);

                	// updating the steps until for the next forequeue
                	ps->foreQueue->qFront->qt->stepsuntil = (ps->foreQueue->qFront->qt->timestepPro)-(ps->timestep);
//              	ps->backQueue->qFront->qt->stepsuntil = (ps->backQueue->qFront->qt->timestepPro)-(ps->timestep);
                }
		else if(b == false && (ps->foreQueue->qFront->qt->stepsuntil != 0))
		{
//			printf("PROCESS NOT DONE AND NOT COMPELTE ROUND ROBIN\n");
		}
		else if(b == true)
		{
//			printf("freeing Process data()\n");	
			freeProcessData();
			process* qt1 = (process*)malloc(sizeof(process));
			qt1 = dequeue(ps->foreQueue);
//			printf("DEQUEUED ELEMENT IS %s\n", qt1->processName);
			// update timestep added for the next element
			if(isEmpty(ps->foreQueue) == 0)
			{
				ps->foreQueue->qFront->qt->timestepAdded = ps->timestep;
			}
			else
			{
//				printf("NOW WE NEED TO DO THE BACKGROUND PROCESSES");
			}
			//free(processName)
			//free(qt1)
			//free(qt)
			// free everything
		}

        }
        else if(call == 0) // this means that there are no more fore processes and now we are call runProcess for back processes
        {
//		printf("---CURRENT STEPSUNTIL PROMO: %d\n", ps->backQueue->qFront->qt->stepsuntil);

//		printf("---loadProcessData()\n");
		loadProcessData(ps->backQueue->qFront->qt->data);
//		printf("after-- loadPorcessData()\n");
//		printf("what is at the front of the backQueue after loaded: %s\n", ps->backQueue->qFront->qt->processName);
		// we are in this else if statement because there are no more foreQueue
		// in the schedule so our numSteps are just the number until PROMO
		// we only have to compare them in the if statement
		ps->backQueue->qFront->qt->stepsuntil = (ps->backQueue->qFront->qt->timestepPro)-(ps->timestep);
		
		numSteps = ps->backQueue->qFront->qt->stepsuntil;
//		printf("WHAT IS NUMSTEPS: %d\n", numSteps);
		
//		printf("---runProcess");
		b = runProcess(ps->backQueue->qFront->qt->processName, &ret, &numSteps);
//		printf("----after runProcess()\n");
		ps->timestep+=numSteps;
		
		// update again
		ps->backQueue->qFront->qt->stepsuntil = (ps->backQueue->qFront->qt->timestepPro)-(ps->timestep);
		if (b == true)
		{
			freeProcessData();
			process* qt2 = (process*)malloc(sizeof(process));
			qt2 = dequeue(ps->backQueue);
//			printf("DEQUEUED ELEMENT IS %s\n", qt2->processName);
		}
		//if((ps->timestep == 50 && ps->backQueue->qFront->qt->stepsuntil == 0) || (ps->timestep > 50 && ps->backQueue->qFront->qt->stepsuntil == 0))// also check if this works 
				// if(ps->backQueue->qFront->qt->stepsuntil == 0) - idk think about this
		//{
		//	printf("ALLING ATTEMPT PROMOTE\n");
		//	attemptPromote(ps);
		//	process* qt1 = (process*)malloc(sizeof(process));
		//	qt1 = dequeue(ps->backQueue);
			// just dequeueing here to escape infinte loop
		//	printf("BACKGROUND PROCESS DEQUEUED--%s\n", qt1->processName);
		//}
		
      }
   
    return ret; /* This will be the char* that runProcess stores in ret when you call it. */
}

/* attemptPromote
 * input: a schedule
 * output: none
 *
 * Promote every background process that has been waiting for 50 time steps.
 * This function might be tricky so you might save it for last.
 */
void attemptPromote( schedule *ps ) {
    /* TODO: complete this function.
    The function "promoteProcess" in processSimulator.c will be useful in completing this. */
//	printf("INSIDE ATTEMPT PROMOTE\n");
	// update stepsuntil
//	printf("CURRENT TIMESTEP IS: %d\n", ps->timestep);
	//printf("TIMESTEP UNTIL PROMO BEFORE UPDATED IS: %d\n", ps->backQueue->qFront->qt->stepsuntil);
	//printf("TIMESTEP PRO BEFORE IS: %d\n", ps->backQueue->qFront->qt->timestepPro);
	///printf("TIME STEP ADDED BEFORE IS: %d\n", ps->backQueue->qFront->qt->timestepAdded);
//	ps->backQueue->qFront->qt->stepsuntil = (ps->backQueue->qFront->qt->timestepPro)-(ps->backQueue->qFront->qt->timestepAdded);
	//printf("TIMESTEPS UNTIL UPDATED IN ATTEMPT IS : %d\n", ps->backQueue->qFront->qt->stepsuntil);
	process* qt1 = (process*)malloc(sizeof(process));
//	qt1 = ps->backQueue->qFront->qt;
    	//schedule* ps1 = createSchedule();
	int i = 0;
	if(ps->backQueue->qFront == NULL && ps->backQueue->qRear == NULL)
	{
//		printf("NOT DOING ANYTHING IN ATTEMPT PROMOTE\n");
		return;
	}
	else if(ps->timestep == ps->backQueue->qFront->qt->timestepPro)      	
        {
//	        printf("TIMESTEP UNTIL PROMO BEFORE UPDATED IS: %d\n", ps->backQueue->qFront->qt->stepsuntil);
  //      	printf("TIMESTEP PRO BEFORE IS: %d\n", ps->backQueue->qFront->qt->timestepPro);
    //    	printf("TIME STEP ADDED BEFORE IS: %d\n", ps->backQueue->qFront->qt->timestepAdded);
//       		ps->backQueue->qFront->qt->stepsuntil = (ps->backQueue->qFront->qt->timestepPro)-(ps->timestep);
//	        printf("TIMESTEPS UNTIL UPDATED IN ATTEMPT IS : %d\n", ps->backQueue->qFront->qt->stepsuntil);

		qt1 = ps->backQueue->qFront->qt;
		
		//if(qt1->stepsuntil == 0)
		//{
			//qt1 = ps->backQueue->qFront->qt;
		//	printf("---PROMOTE PROCESS CALLed HERE\n");
               	//	promoteProcess(ps->backQueue->qFront->qt->processName, ps->backQueue->qFront->qt->data);
                	//process* qt1 = (process*)malloc(sizeof(process));
                //	qt1 = dequeue(ps->backQueue);
                //	printf("-- QT1 PROMOTED PROCESS DEQUEUEED FRO BACK: %s\n", qt1->processName);
		//	enqueue(ps->foreQueue, qt1);
			//qt1 = ps->backQueue->qFront->qt;
		//	temp = ps->backQueue->qFront;
			//continue;
		//}
		while(ps->timestep == qt1->timestepPro)
		{
			// if the next process is has also been waiting for 50 time steps
			// then continue checking the other ones
			// if the next one has been waiting for less than number of time steps then exit
			// from loop and continue;
//			printf("--INSIDE WHILE LOOP\n");
			promoteProcess(ps->backQueue->qFront->qt->processName, ps->backQueue->qFront->qt->data);
			qt1 = dequeue(ps->backQueue);
			enqueue(ps->foreQueue, qt1);
			//ps->foreQueue->qFront->qt->stepsuntil = 5;
			//update qt1 here
			qt1 = getNext(ps->backQueue);
			ps->foreQueue->qFront->qt->stepsuntil = 5;
			if(qt1 == NULL)
			{
				break;
			}
//			printf("WHAT IS QT1 GETNEXT() RETURN: %s\n", qt1->processName);
//			printf("WHAT IS QT1 GETNEXT() PROMO STEP NUMBER: %d\n", qt1->timestepPro);
		}
//		printf("OUT AND AFTER WHILE LOOP IN ATTEMPT");
	}
		// nedd to figure out how to move to the next thing in schedule here
		//printf("FIGURE THIS OUT IN WHILE IN ATTEMPT PROMOTE\n");
	//	
	//	qt1 = getNext(ps->backQueue);
		//qt1 = dequeue(ps1->backQueue);
		//enqueue(ps1->foreQueue, qt1);
	//	temp = ps->backQueue->qFront->qt1;
	//	if(temp == NULL)
	//		printf("TEMP IS EQUAL TO NULL\n");
		//printf("TEMP: %s\n", temp->qt->processName);
	//	else
		//	printf("TEMP IS NOT MOVEING ON\n");
	//		qt1 = temp->qt;
//		printf("LOOPING THROUGH: %s\n", qt1->processName);
//	}
	return;
}
/* freeSchedule
 * input: a schedule
 * output: none
 *
 * Free all of the memory associated with the schedule.
 */
void freeSchedule( schedule *ps ) {
    /* TODO: free any data associated with the schedule as well as the schedule itself.
    the function "freeQueue" in queue.c will be useful in completing this. */
	freeQueue(ps->foreQueue);
	freeQueue(ps->backQueue);
}

/* PROVIDED FUNCTION
 * freeSchedule
 * input: two ints
 * output: the smaller of the 2 ints
 */
int min( int x, int y ){
    if( x<y )
        return x;
    return y;
}
int doesScheduleHaveFore(schedule *ps)
{
	if(ps->foreQueue->qFront == NULL && ps->foreQueue->qRear == NULL)
	{
		return 0;
	}
	return 1;
}
