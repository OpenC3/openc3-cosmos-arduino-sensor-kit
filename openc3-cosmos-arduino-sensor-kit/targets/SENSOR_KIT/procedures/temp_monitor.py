while True:
  temp = tlm("SENSOR_KIT BMP280 TEMP_C")
  print(temp)
  if temp > 25:
    cmd("SENSOR_KIT SET_LED with STATE 1")
  else:
    cmd("SENSOR_KIT SET_LED with STATE 0")
  wait(1)

