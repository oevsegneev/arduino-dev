int32_t clamp( int32_t v, int32_t minv, int32_t maxv ){
    if (v<minv)
        return minv;
    if (v>maxv)
        return maxv;
    return v;
}

void runMotor( int side, int dir ){
    if( dir == FORWARD ){
        analogWrite( mpin[side][0], pwm[side] );
        analogWrite( mpin[side][1], 0 );
    } else {
        analogWrite( mpin[side][0], 0 );
        analogWrite( mpin[side][1], pwm[side] );
    }
}

void stopMotor( int side){
    analogWrite( mpin[side][0], 0 );
    analogWrite( mpin[side][1], 0 );
}

void setMotors( int dir, int val ){
    switch( dir ){
    case 0:{
        ldir = RELEASE;
        rdir = RELEASE;
        break;
    }
    // поворот налево
    case 1:{
        if(!stopped){
            ldir = BACKWARD;
            rdir = FORWARD;
        }
        break;
    }
    // движение вперед
    case 2:{
        if(!stopped){
            ldir = FORWARD;
            rdir = FORWARD;
        }
        break;
    }
    // поворот направо
    case 3:{
        if(!stopped){
            ldir = FORWARD;
            rdir = BACKWARD;
        }
        break;
    }
    // движение назад
    case 4:{
        if(!stopped){
            ldir = BACKWARD;
            rdir = BACKWARD;
        }
        break;
    }
    // установка скорости
    case 10:{
        if( val > 8 ){
            tpulse = 50000 - 150*val;
            beep(1, 50);
            stopped = 0;
        } else {
            ldir = RELEASE;
            rdir = RELEASE;
            stopped = 1;
            beep(2, 50);
        }
        break;
    }
    }
}

void controlMotors() {
    int err, pulse;
    int32_t pid;
    if( ldir == RELEASE )
        stopMotor(MOTOR_LEFT);
    else {
        pulse = (micros()>>8) - (ltime>>8);
        err = (tpulse>>10) - (lpulse>>10);
        // jumpstart
        if( pulse > (STALL_TPULSE>>8) ){
            lspd = clamp(lspd+2000, 50000, 255000);
        }
        else if (err>15){
            lspd = clamp(lspd-1000, 50000, 255000);
        }
        else {
            pid = -pk*err*abs(err);
            lspd = clamp(lspd+pid, 50000, 255000);
        }
        pwm[MOTOR_LEFT] = lspd/1000;
        runMotor(MOTOR_LEFT, ldir);
    }
  
    if( rdir == RELEASE )
        stopMotor(MOTOR_RIGHT);
    else {
        pulse = (micros()>>8) - (rtime>>8);
        err = (tpulse>>10) - (rpulse>>10);
        // jumpstart
        if( pulse > (STALL_TPULSE>>8) ){
            rspd = clamp(rspd+2000, 50000, 255000);;
        }
        else if (err>15){
            rspd = clamp(rspd-1000, 50000, 255000);;
        }
        else {
            pid = -pk*err*abs(err);
            rspd = clamp(rspd+pid, 50000, 255000);
        }
        pwm[MOTOR_RIGHT] = rspd/1000;
        runMotor(MOTOR_RIGHT, rdir);
    }

    #ifdef DEBUGM
    if(millis()-dtime > 100){
        dtime = millis();
        rd.setPacketValue(lpulse>>10);
        rd.setPacketValue(rpulse>>10);
        rd.setPacketValue(pwm[MOTOR_RIGHT]);
        rd.sendPacket();
    }
    #endif
}
