#include "utilities.h"
#include "linux_utilities.h"
#include "macos_utilities.h"
#include <string>

namespace cobot {

auto find_port() -> std::string {
#ifdef MACOS
  return macos::find_port();
#elif LINUX
  return ubuntu::find_port();
#else
#error "Unsupported platform"
#endif
}

} // namespace cobot

/*

def public_check(parameter_list, kwargs, robot_limit, class_name,
exception_class): for parameter in parameter_list[1:]: value =
kwargs.get(parameter, None) value_type = type(value) if parameter == 'id' and
value not in robot_limit[class_name][parameter]: check_id(value,
robot_limit[class_name][parameter], exception_class) elif parameter == 'rgb':
            check_rgb_value(value, exception_class, class_name)
        elif parameter == 'value':
            check_value_type(parameter, value_type, exception_class, int)
            if value < 0 or value > 4096:
                raise exception_class("The range of {} is 0 ~ 4096, but the
received value is {}".format(parameter, value)) elif parameter == 'pin_mode':
            check_value_type(parameter, value_type, exception_class, int)
            if value not in [0,1,2]:
                raise exception_class("The data supported by parameter {} is 0
or 1 or 2, but the received value is {}".format(parameter, value)) elif
parameter == 'pin_signal': check_0_or_1(parameter, value, [0, 1], value_type,
exception_class, int) elif parameter == 'speed': check_value_type(parameter,
value_type, exception_class, int) if not 1 <= value <= 100: raise
exception_class( "speed value not right, should be 1 ~ 100, the received speed
is %s" % value
                )
        elif parameter == 'flag':
            check_0_or_1(parameter, value, [0, 1, 254], value_type,
exception_class, int) elif parameter == 'gripper_type': check_0_or_1(parameter,
value, [1, 3, 4], value_type, exception_class, int) elif parameter == '_type_1':
            check_0_or_1(parameter, value, [1, 2, 3, 4], value_type,
exception_class, int) # if value not in [0, 1, 10]: #     raise
exception_class("The data supported by parameter {} is 0 or 1 or 10, but the
received value is {}".format(parameter, value)) elif parameter ==
'gripper_value': check_value_type(parameter, value_type, exception_class, int)
            if value < 0 or value > 100:
                raise exception_class("The range of {} is 0 ~ 100, but the
received value is {}".format(parameter, value)) elif parameter in ['account',
'password']: check_value_type(parameter, value_type, exception_class, str) #
TODO 280/320共用MyCobot，无法进行数据限位 # elif parameter == 'coords': #
check_coords(value, robot_limit, class_name, exception_class) elif parameter in
['rftype', 'move_type', 'end', 'is_linear', 'status', 'mode', 'direction']:
            check_0_or_1(parameter, value, [0, 1], value_type, exception_class,
int) elif parameter == 'acceleration': check_value_type(parameter, value_type,
exception_class, int) if not 1 <= value <= 100: raise exception_class(
                    "{} value not right, should be 1 ~ 100, the received is
{}".format(parameter, value)
                )
        elif parameter == 'angle':
            id = kwargs.get('id', None)
            index = robot_limit[class_name]['id'][id-1] - 1
            if value < robot_limit[class_name]["angles_min"][index] or value >
robot_limit[class_name]["angles_max"][index]: raise exception_class( "angle
value not right, should be {0} ~ {1}, but received {2}".format(
                        robot_limit[class_name]["angles_min"][index],
robot_limit[class_name]["angles_max"][index], value
                    )
                )
        elif parameter == 'encoders':
            if  "MyCobot" in class_name or "MechArm" in class_name:
                if len(value) != 6:
                    raise exception_class("The length of `encoders` must be 6.")
            elif "MyPalletizer" in class_name:
                if len(value) != 4:
                    raise exception_class("The length of `encoders` must be 4.")
            elif "MyPalletizer" in class_name:
                if len(value) != 7:
                    raise exception_class("The length of `encoders` must be 7.")
            for data in value:
                data_type = type(data)
                check_value_type(parameter, data_type, exception_class, int)
                if data < 0 or data > 4096:
                    raise exception_class("The range of encoder is 0 ~ 4096, but
the received value is {}".format(data)) elif parameter == 'speeds': if "MyCobot"
in class_name or "MechArm" in class_name: if len(value) not in [6, 7]: raise
exception_class("The length of `speeds` must be 6. but the received value is
{}".format(value)) elif "MyPalletizer" in class_name: if len(value) != 4: raise
exception_class("The length of `speeds` must be 4.") elif "MyArm" in class_name:
                if len(value) != 7:
                    raise exception_class("The length of `speeds` must be 7.")
            for data in value:
                data_type = type(data)
                check_value_type(parameter, data_type, exception_class, int)
                if data < 0 or data > 3400:
                    raise exception_class("The range of speed is 0 ~ 3400, but
the received value is {}".format(data)) elif parameter in ['servo_id_pdi',
'encode_id']: check_value_type(parameter, value_type, exception_class, int) # if
"MyCobot" in class_name or "MechArm" in class_name: #     if value < 1 or value
> 6: #             raise exception_class("The range of id is 1 ~ 6, but the
received is {}".format(value)) if "MyPalletizer" in class_name: if value < 1 or
value > 4: raise exception_class("The range of id is 1 ~ 4, but the received is
{}".format(value)) elif "MyArm" in class_name or "MyCobot" in class_name or
"MechArm" in class_name: if value < 1 or value > 7: raise exception_class("The
range of id is 1 ~ 7, but the received is {}".format(value)) elif parameter ==
"torque": torque_min = 150 torque_max = 980 if value < torque_min or value >
torque_max: raise exception_class("The range of torque is {} ~ {}, but the
received is {}".format(torque_min, torque_max, value)) elif parameter ==
"current": current_min = 1 current_max = 500 if value < current_min or value >
current_max: raise exception_class("The range of current is {} ~ {}, but the
received is {}".format(current_min, current_max, value)) elif parameter ==
'end_direction': check_0_or_1(parameter, value, [1, 2, 3], value_type,
exception_class, int)

*/
