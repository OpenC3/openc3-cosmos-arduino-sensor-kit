while True:
  temp = tlm("BMP280 STATUS TEMP_C")
  print(temp)
  if temp > 25:
    cmd("BMP280 SET_LED with STATE 1")
  else:
    cmd("BMP280 SET_LED with STATE 0")
  wait(1)

