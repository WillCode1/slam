std_msgs/Header header
# float64 timestamp
uint32 ins_status           # INS初始化状态,所有比特位都为1成功
uint32 wheel_speed_status   # 轮速状态，表示有无轮速
# ====================
uint32 position_status      # 定位类型，固定解满足厘米级
uint32 heading_status       # 航向类型，固定解满足厘米级

uint32 numsv                # 卫星数，20~30满足高精度
float64 hdop                # 水平分量精度因子，为纬度和经度误差平方和的开根号值
float64 rtk_age             # ‌RTK差分龄期，基站数据和移动站数据之间存在时间差

float64 sec_of_week         # 周内秒
float64 gps_week_number     # GPS周

float32 temperature
# ====================
float64 latitude
float64 longitude
float64 altitude

float64 north_velocity
float64 east_velocity
float64 ground_velocity

float64 roll
float64 pitch
float64 azimuth
# ====================
float64 x_angular_velocity
float64 y_angular_velocity
float64 z_angular_velocity

float64 x_acc
float64 y_acc
float64 z_acc
# ====================
float32 latitude_std
float32 longitude_std
float32 altitude_std

float32 north_velocity_std
float32 east_velocity_std
float32 ground_velocity_std

float32 roll_std
float32 pitch_std
float32 azimuth_std
