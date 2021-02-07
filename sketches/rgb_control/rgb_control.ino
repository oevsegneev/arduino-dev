int clr[3] = {9,10,11};
int to = 3;

void setup() {
    for(int i=0; i<3; i++)
        pinMode(clr[i], OUTPUT);

    setColor(0,255,0);
    delay(1000);
    setColor(0,0,255);
    delay(1000);
    setColor(255,0,0);
    delay(1000);
}

void loop() {
    for(int i=0;i<256;i++){
        setColor(255,i,0);
        delay(to);
    }
    for(int i=255;i>=0;i--){
        setColor(i,255,0);
        delay(to);
    }
    for(int i=0;i<256;i++){
        setColor(0,255,i);
        delay(to);
    }
    for(int i=255;i>=0;i--){
        setColor(0,i,255);
        delay(to);
    }
    for(int i=0;i<256;i++){
        setColor(i,0,255);
        delay(to);
    }
    for(int i=255;i>=0;i--){
        setColor(255,0,i);
        delay(to);
    }
   
}

void setColor(byte r, byte g, byte b ){
    analogWrite(clr[0], r);
    analogWrite(clr[1], g);
    analogWrite(clr[2], b);
}


