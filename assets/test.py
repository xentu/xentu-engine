from time import time, ctime, sleep
import module1


module1.test_module('this works!', 'this works too!')


sleep(3) # Sleep for 3 seconds
print('slept for 3s')
print('Today is', ctime(time()))
print("done.")