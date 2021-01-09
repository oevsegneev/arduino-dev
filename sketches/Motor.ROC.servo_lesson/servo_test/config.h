#define SERVOMIN  150
#define SERVOMAX  600

#define HIP_SPD 2
#define LEG_SPD 5
#define EYE_SPD 1

#define SCAN_STEPS 5
#define WALK_BWD_STEPS 8

// states
#define ST_IDLE 1
#define ST_STAND 2
#define ST_SWEEP 4
#define ST_WALK_FWD 8
#define ST_WALK_BWD 16
#define ST_TURN_L 32
#define ST_TURN_R 64
#define ST_STRAFE_L 128
#define ST_STRAFE_R 256
#define ST_WAIT 512
#define ST_SCAN 1024

#define ST_BUSY 2048

// state times
#define ST_IDLE 0
#define ST_STAND_T 2000
#define ST_SWEEP_T 4000
#define ST_WALK_T 5000

// numbers
#define FRH 0 //front right hip
#define FRL 1 //front right leg
#define FLH 2 //front left hip
#define FLL 3 //front left leg
#define RLH 4 //rear left hip
#define RLL 5 //rear left leg
#define RRH 6 //rear right hip
#define RRL 7 //rear right leg
#define BBN 8 //sevor for backbone
#define EYE 9 //sevor for sensor
#define LSR 10 //laser

// speed
#define SPD_STAND 3
#define SPD_SWEEP 1
#define SPD_WALK 1

// timeout
#define STEP_TO_STAND 200
#define STEP_TO_SWEEP 1000
#define STEP_TO_WALK 200

// pins
#define FRH_PIN 0
#define FRL_PIN 1
#define FLH_PIN 2
#define FLL_PIN 3
#define RLH_PIN 4
#define RLL_PIN 5
#define RRH_PIN 6
#define RRL_PIN 7
#define BBN_PIN 8
#define EYE_PIN 9
#define LSR_PIN 14

// defaults
#define FRH_DEF 45
#define FRL_DEF 74 + 60
#define FLH_DEF 75
#define FLL_DEF 86 - 60
#define RLH_DEF 45
#define RLL_DEF 74 + 60
#define RRH_DEF 80
#define RRL_DEF 91 - 60
#define BBN_DEF 83
#define EYE_DEF 75
#define LSR_DEF 90

