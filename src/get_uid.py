import ctypes
from ctypes import wintypes

# 定义结构体 HW_PROFILE_INFOA
class HW_PROFILE_INFOA(ctypes.Structure):
    _fields_ = [
        ("dwDockInfo", wintypes.DWORD),
        ("szHwProfileGuid", ctypes.c_char * 39),  # GUID 长度为 38 字符，加上终止符为 39
        ("szHwProfileName", ctypes.c_char * 80)
    ]

# 加载 Advapi32.dll
advapi32 = ctypes.WinDLL('Advapi32', use_last_error=True)

# 定义函数原型
advapi32.GetCurrentHwProfileA.argtypes = [ctypes.POINTER(HW_PROFILE_INFOA)]
advapi32.GetCurrentHwProfileA.restype = wintypes.BOOL

def get_unique_id():
    hw_profile_info = HW_PROFILE_INFOA()

    # 调用 GetCurrentHwProfileA
    result = advapi32.GetCurrentHwProfileA(ctypes.byref(hw_profile_info))

    if not result:
        raise ctypes.WinError(ctypes.get_last_error())

    # 返回 GUID（szHwProfileGuid），需要解码为字符串
    return hw_profile_info.szHwProfileGuid.decode('ascii')

# 示例调用
try:
    unique_id = get_unique_id()
    print(f"Unique Hardware Profile GUID: {unique_id}")
except Exception as e:
    print(f"Error: {e}")
