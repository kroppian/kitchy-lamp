#ifndef button
#define button

#define CONFIDENCE 100

short btn_pressed[btn_count];
short conf_level[btn_count];
short curr_conf[btn_count];

short last_state[btn_count]; 
short current_state[btn_count]; 

void btn_poll(int port, int pin_no, int button_no){

  conf_level[button_no] = CONFIDENCE;

  last_state[button_no] = current_state[button_no];
  current_state[button_no] = bit_is_clear(port,pin_no);

  // if we're confident enough that an event is actually happening
  if(curr_conf[button_no] > conf_level[button_no]){
  
    curr_conf[button_no]++; 

    // If the button went from unpressed to pressed
    if(current_state[button_no])
      btn_pressed[button_no] = 1; 
    else 
      btn_pressed[button_no] = 0; 
       
  }else if(last_state[button_no] == current_state[button_no]){
    curr_conf[button_no]++; 
  }else{
    curr_conf[button_no] = 0;
  
  }

}

int pressed(int button_no){

  return btn_pressed[button_no];

}


#endif

