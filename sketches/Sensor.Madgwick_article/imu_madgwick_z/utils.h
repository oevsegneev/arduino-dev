void quat_2_euler( float q[4], float e[3] ){
   //double sqw = q[0]*q[0];
   double sqx = q[1]*q[1];
   double sqy = q[2]*q[2];
   double sqz = q[3]*q[3];

   e[0] = atan2f(2.f * (q[2]*q[3] + q[1]*q[0]), 1 - 2.f * (sqx + sqy));// -sqx - sqy + sqz + sqw);
   e[1] = asinf(-2.f * (q[1]*q[3] - q[2]*q[0]));
   e[2] = atan2f(2.f * (q[1]*q[2] + q[3]*q[0]), 1 - 2.f * (sqy + sqz));//sqx - sqy - sqz + sqw);
}

float deadband(float v, float db){
  if(abs(v) < db) {
    return 0.0f;
  } else if(v > 0){
    return v - db;
  } else if(v < 0){
    return v + db;
  }
  return 0;
}
