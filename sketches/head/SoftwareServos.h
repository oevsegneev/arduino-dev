#define MIN_PULSE_WIDTH       500     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2500     // the longest pulse sent to a servo
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds

#define SERVOS_PER_BANK       10     // the maximum number of servos controlled by one timer bank
#define MAX_SERVOS   20				// the maximum number of possible servos to control on the UNO

#define INTERRUPT_OVERHEAD 2

#include <avr/interrupt.h>
#include <Arduino.h>

// identifies which servo pin is currently being pulsed
static volatile char ss_currentServo = -1;
static volatile unsigned char ss_pulseBank = 0;
static volatile int ss_pulseDiff = -1;
static volatile unsigned int ss_ServoTicks[MAX_SERVOS];
static volatile unsigned int ss_TurnOff[MAX_SERVOS];
static volatile unsigned int ss_refreshInterval;
static volatile unsigned int ss_bankClk;

/************ ISP (Interrupt Service Proceedure) ***********************/
SIGNAL (TIMER1_COMPA_vect)
{
  // if we are starting a new 20ms frame, initialize the timer counter to zero
  if(ss_currentServo < 0 )
    TCNT1 = 0;
  else
  {
    int index = ss_currentServo+ss_pulseBank;
    // stop the pulse on this servo if it is activated
    if(ss_ServoTicks[index] > 0)
    {
      digitalWrite(index, LOW);
      if (ss_TurnOff[index])
      {
        ss_ServoTicks[index]=0;
        ss_TurnOff[index]=0;
        pinMode(index, INPUT);
      }
    }

    if (ss_pulseDiff == 0)
    {
      // stop the pulse on this servo if it is activated
      int index = ss_currentServo+(ss_pulseBank^SERVOS_PER_BANK);
      if(ss_ServoTicks[index] > 0)
      {
        digitalWrite(index, LOW);
        if (ss_TurnOff[index])
        {
          ss_ServoTicks[index]=0;
          ss_TurnOff[index]=0;
          pinMode(index, INPUT);
        }
      }
    }
    else
    // if we need to wait for the sibling pulse to trigger do so
    if (ss_pulseDiff == 1)
    {
      // if we ran out of time ... stop the second pulse
      if (((unsigned)TCNT1)>(ss_bankClk+4))
      {
        int index = ss_currentServo+(ss_pulseBank^SERVOS_PER_BANK);
        // stop the pulse on this servo if it is activated
        if(ss_ServoTicks[index] > 0)
        {
          digitalWrite(index, LOW);
          if (ss_TurnOff[index])
          {
            ss_ServoTicks[index]=0;
            ss_TurnOff[index]=0;
            pinMode(index, INPUT);
          }
        }
      }
      else
      {
        // setup timer callback
        OCR1A = ss_bankClk;

        // indicate we are done with both servos
        ss_pulseDiff = -1;

        // switch to the other servo bank
        ss_pulseBank ^=SERVOS_PER_BANK;

        return;
      }
    }
  }

  // increment to the next pin
  if ((++ss_currentServo) < SERVOS_PER_BANK)
  {
    // keep track of which pulse is longer, bank0 or bank1
    unsigned int bankClk = 0;
    unsigned int altBankClk = 0;

    ss_pulseDiff = -1;
    ss_pulseBank = 0;

    int val = ss_ServoTicks[ss_currentServo];
    // setup to callback this function after the pulse size of the current servo
    if (val > 0)
    {
      // start the pulse
      digitalWrite(ss_currentServo, HIGH);

      bankClk = ((unsigned)TCNT1) + val;
    }

    int secondCurrentServo = ss_currentServo+SERVOS_PER_BANK;
    val = ss_ServoTicks[secondCurrentServo];
    if (val > 0)
    {
      // start the second pulse
      digitalWrite(secondCurrentServo, HIGH);

      altBankClk = ((unsigned)TCNT1)+val;

      // if this pulse is shorter than the first use it!
      if ((altBankClk<bankClk)||(bankClk==0))
      {
        // save the difference between the pulse sizes
        ss_pulseDiff = 1;

        ss_bankClk = bankClk;

        bankClk = altBankClk;

        // remember that we are referring to the second bank
        ss_pulseBank = SERVOS_PER_BANK;

      }
      else
      {
        ss_bankClk = altBankClk;

        if (altBankClk==bankClk)
          ss_pulseDiff = 0;
        else
          ss_pulseDiff = 1;
      }
    }

    if (bankClk > (((unsigned)TCNT1)+ INTERRUPT_OVERHEAD))
      OCR1A = bankClk - INTERRUPT_OVERHEAD;
    else
      OCR1A = ((unsigned)TCNT1) + 4;
  }
  else
  {
    // we finished servicing all servos so wait to complete the 20ms period before starting over
    // the - 4 is to ensure that we are not about to miss a OCR interrupt
    if ((unsigned)TCNT1 <  (ss_refreshInterval - 4))
      OCR1A = ss_refreshInterval;
    else
      // for whatever reason, we've exceeded the 20ms time period ... just setup to call back asap
      OCR1A = ((unsigned)TCNT1) + 4;

    // indicate that we are done with this period and to start a new one on the next callback
    ss_currentServo = -1;
  }
}

/****************** end of static functions ******************************/

void ss_Init()
{
  int i;
  for (i=0;i<MAX_SERVOS;i++)
  {
    ss_ServoTicks[i] = 0;
    ss_TurnOff[i] = 0;
  }

  TCCR1A = 0;             // normal counting mode
  TCCR1B = _BV(CS11);     // set prescaler of 8
  TCNT1 = 0;              // clear the timer count

  TIFR1 |= _BV(OCF1A);     // clear any pending interrupts;
  TIMSK1 |=  _BV(OCIE1A) ; // enable the output compare interrupt

  // cache the REFRESH_INTERVAL value in ticks instead of microseconds
  ss_refreshInterval = ((clockCyclesPerMicrosecond() * REFRESH_INTERVAL)>>3);
}

void ss_Destroy()
{
  // disable timer 1 output compare interrupt
  TIMSK1 &=  ~_BV(OCIE1A) ;
}

int ss_SetPosition(int pin, int value)
{
  // check that we have a valid pin (note that 0 and 1 are used for serial!)
  if ((pin<0)||(pin>=MAX_SERVOS)) return 0;

  // an out of bounds value indicates a servo shutdown
  if ((value < MIN_PULSE_WIDTH)||(value> MAX_PULSE_WIDTH))
  {
    if (ss_ServoTicks[pin]>0)
      ss_TurnOff[pin] = 1;

    return 0;
  }

  // convert from microseconds to clock ticks assuming prescaler of 8
  value = ((clockCyclesPerMicrosecond() * (value - INTERRUPT_OVERHEAD))>>3);

  // check that pin has been initialized
  int hold = ss_ServoTicks[pin];

  //unsigned char oldSREG = SREG;
  //cli();
  ss_ServoTicks[pin] = value;
  //SREG = oldSREG;

  // if value was zero, the pin had not been set to OUTPUT
  if (hold == 0)
  {
    // set servo pin to output
    pinMode( pin, OUTPUT);
  }
}

