led = 0

while true

  if led == 1
    led = 0
  else
    led = 1
  end

  if sw1_read() == 0
    led = 1
  end

  led1_write( led )
  sleep 2

end
