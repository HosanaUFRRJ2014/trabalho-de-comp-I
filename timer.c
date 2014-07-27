
#define FPS 60


typedef struct timer{
  int startTicks;

  //The ticks stored when the timer was paused
  int pausedTicks;

  //The timer status
  int paused;
  int started;
}Timer;

  // coisas  de vsync
    Timer fps;
    

void timer_start(Timer *timer)
{
    timer->started = 1;
    timer->paused = 0;
    timer->startTicks = SDL_GetTicks();
}
void timer_stop(Timer *timer)
{
    timer->started = 0;
    timer->paused = 0;
}
void timer_pause(Timer *timer)
{
     if( ( timer->started == 1 ) && (timer->paused == 0 ) )
    {
        //Pause the timer
        timer->paused = 1;

        //Calculate the paused ticks
        timer->pausedTicks = SDL_GetTicks() - timer->startTicks;
    }
}
void timer_unpause(Timer *timer)
{
     if( timer->paused == 1 )
    {
        //Unpause the timer
        timer->paused = 0;

        //Reset the starting ticks
        timer->startTicks = SDL_GetTicks() - timer->pausedTicks;

        //Reset the paused ticks
        timer->pausedTicks = 0;
    }
}

int timer_get_ticks(Timer *timer)
{
    if( timer->started == 1 )
    {
        //If the timer is paused
        if( timer->paused == 1 )
        {
            //Return the number of ticks when the timer was paused
            return timer->pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - timer->startTicks;
        }
    }
}
