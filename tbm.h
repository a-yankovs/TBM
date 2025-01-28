// file that defines all of our pins, pins have yet to be chosen 
#define RX 
#define TX 
#define ESTOPCTRL_PIN 
#define MOTORCTRL_PIN 
#define PUMPCTRL_PIN 
#define BENTCTRL_PIN 

#define ESTOPSENSE_PIN 
#define MOTOR_TEMP_PIN 
#define PUMP_TEMP_PIN 
#define FLOW_IN_PIN 
#define FLOW_OUT_PIN 
#define MOTORSENSE_PIN 
#define PUMPSENSE_PIN 


#define SENSOR_API "/sensor"
#define CONFIG_API "/config"


#define STOP "stop"
#define START "start"
#define RUNNING "running"
#define ERROR "error"

typedef enum { 
    NO_MESSAGE = 0, 
    TBM_INIT = 1,  
    TBM_START = 2,  
    TBM_STOP = 3,  
    TBM_ERROR = 4,  
    TBM_DATA = 5,  
} MessageID; 

MessageID incomingMessage = NO_MESSAGE; 
    
struct Sensor {
    bool active;
    int value;   // can be an array if we need multiple values.
    signed int timestamp;
};
