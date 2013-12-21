
template<class T>
size_t printBigInt(Print* printer, T n, uint8_t base = DEC)
{
  //stole this code from the printNumber function in Print.h :P

  char buf[8 * sizeof(T) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    T m = n;
    n /= base;
    char c = m - base * n;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return printer->write(str) + printer->write('\r') + printer->write('\n');
}

void drainSerial() {
  while (Serial.available() > 0)
    Serial.read();
}

void zeroBuffer(char * buffer, int length)
{
  for(int i = 0; i < length; i++)
    buffer[i] = 0;
}