#include <windows.h>
#include <iostream>
#include <string>

std::string get_unique_id() {
    HW_PROFILE_INFOA HwProfInfo;

    // 调用 GetCurrentHwProfileA 获取硬件配置信息
    if (!GetCurrentHwProfileA(&HwProfInfo)) {
        std::cerr << "Failed to get hardware profile information." << std::endl;
        return "";
    }

    // 返回 GUID
    return std::string(HwProfInfo.szHwProfileGuid);
}

bool copy_to_clipboard(const std::string& text) {
    // 打开剪切板
    if (!OpenClipboard(nullptr)) {
        std::cerr << "Failed to open clipboard." << std::endl;
        return false;
    }

    // 清空剪切板
    if (!EmptyClipboard()) {
        std::cerr << "Failed to empty clipboard." << std::endl;
        CloseClipboard();
        return false;
    }

    // 分配全局内存来保存字符串
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hGlobal) {
        std::cerr << "Failed to allocate global memory." << std::endl;
        CloseClipboard();
        return false;
    }

    // 将字符串复制到全局内存
    memcpy(GlobalLock(hGlobal), text.c_str(), text.size() + 1);
    GlobalUnlock(hGlobal);

    // 设置剪切板内容为 CF_TEXT（纯文本）
    if (!SetClipboardData(CF_TEXT, hGlobal)) {
        std::cerr << "Failed to set clipboard data." << std::endl;
        GlobalFree(hGlobal);
        CloseClipboard();
        return false;
    }

    // 关闭剪切板
    CloseClipboard();
    return true;
}

int main() {
    // 获取硬件配置 GUID
    std::string unique_id = get_unique_id();

    if (!unique_id.empty()) {
        std::cout << "GODOT设备ID: " << unique_id << std::endl;

        // 将 GUID 复制到剪切板
        if (copy_to_clipboard(unique_id)) {
            std::cout << "已复制到剪切板" << std::endl;
        }
        else {
            std::cout << "复制到剪切板错误" << std::endl;
        }
    }
    else {
        std::cout << "获取设备ID错误" << std::endl;
    }
    system("pause");
    return 0;
}
