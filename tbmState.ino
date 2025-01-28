// This file defines the states for the Tunnel Boring Machine (TBM) depending on the states of the 
// sensors (bentonite pump sensor, motor temp sensor, and flow monitor), as well as if the emergency stop button 
// was pressed. 
#include <Arduino.h>
#include <tbm.h> // reference to another file in the team repo

// defines system states 
enum State { 
  CONFIG, 
  RUNNING, 
  ERROR, 
  STOP
}; 

//sets current state to config 
State currentState = CONFIG; 


// amplifier module used: AD8495
const float maxTemp = 0;  // to be changed later

// ref resolution - 32 bits 
const float adcResolution = 65536;  

void state_setup() {
  Serial.begin(9600);
  Serial.println("System initialized - Entering CONFIG state");
} 

void state_loop(){
  switch (currentState) { 
    case CONFIG: 
      Serial.println("CONFIG: Configuring system"); 
      if (!checkStopped()) { 
        currentState = ERROR; 
      } else if (incomingMessage != TBM_START) { 
        currentState = CONFIG; 
      } else { 
        currentState = RUNNING; 
      }
      break; 

    case RUNNING:
      Serial.println("RUNNING - Current temp is %.1f°C\n", SystemData.motor_temp.value); 
      // checks if the max temp has been exceeded, if the emergency stop button has been pressed, or if the server has sent a message
      // to stop the motor
      if (SystemData.motor_temp.value >= maxTemp || SystemData.estop_button.value == 1 || incomingMessage == TBM_STOP) { 
        Serial.println("ERROR: Stopping TBM"); 
        currentState = ERROR; 
      }
      break; 

    case ERROR: 
      Serial.println("Stopping system"); 
      digitalWrite(MOTORCTRL_PIN, LOW);     // stops motor
      digitalWrite(PUMPCTRL_PIN, LOW);      // stops water pump 
      digitalWrite(BENTCTRL_PIN, LOW);      // stops bentonite pump 
      // send message to server 
      currentState = STOP; 
      break; 

    case STOP:
        // every time the system is stopped, it automatically resets. 
        if (!checkStopped()) { 
          currentState = ERROR; 
        } else { 
          Serial.println("System stopped. Resetting system."); 
          if (incomingMessage != TBM_START) { 
            currentState = STOP; 
          } else { 
            currentState = CONFIG; 
          }  
        }
      break;  

    default: 
      Serial.println("Unknown state"); 
      currentState = CONFIG; 
      break; 

  }
}

// helper method that checks if the motor, bentonite pump, and water pump has stopped working 
// return true if all three are stopped/off
// returns false otherwise
bool checkStopped() { 
  if (digitalRead(MOTORCTRL_PIN) != LOW) { 
    Serial.println("Motor not stopped!"); 
    return false; 
  } else if (digitalRead(PUMPCTRL_PIN) != LOW) { 
    Serial.println("Water pump not off!"); 
    return false; 
  } else if (digitalRead(BENTCTRL_PIN) != LOW) { 
    Serial.println("Bentonite pump not off! "); 
    return false; 
  } else { 
    return true; 
  }
}


  


  
