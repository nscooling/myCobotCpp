from pymycobot import MyCobot
import time



mc = MyCobot('/dev/tty.usbserial-56E30047721', 115200)

sts = mc.get_basic_version()
print(f"get_basic_version = {sts}")
angles = mc.get_angles()
print(f"get_angles = {angles}")
# mc.send_angle(5,-90,5);
# time.sleep(1)
print(mc.get_coords())
mc.send_angle(5,90,50);
time.sleep(5)
print(mc.get_coords())

mc.send_angle(5,-90,50);
time.sleep(5)
print(mc.get_coords())

mc.send_angle(5,0,50);
time.sleep(5)
print(mc.get_coords())

# mc.send_angle(4,0,5);
# time.sleep(1)
# mc.send_angle(3,0,5);
# time.sleep(1)
angles = mc.get_angles()
print(f"get_angles = {angles}")
print(mc.get_tool_reference())
print(mc.get_world_reference())
print(mc.get_reference_frame())
print(mc.get_encoders())
# for count in range(5):
#     mc.set_color(255, 0, 0)
#     time.sleep(1)
#     mc.set_color(0, 255, 0)
#     time.sleep(1)
#     mc.set_color(0, 0, 255)
#     time.sleep(1)
#     mc.set_color(0, 0, 0)
#     time.sleep(1)