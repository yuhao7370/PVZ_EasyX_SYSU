#include <graphics.h>  
#include <math.h>
#include <conio.h>
#pragma comment( lib, "MSIMG32.LIB")

#define  PI 3.1415926535897932

// int WindowWidth, WindowHeight;
// WindowWidth = 800;
// WindowHeight = 600;


void loadImageSequence(std::vector<IMAGE>& images, const std::string& path, int frames) {
    for (int i = 0; i < frames; i++) {
        std::string str = path + "/frame_" + std::to_string(i) + ".png";
        loadimage(&images[i], str.c_str());
        std::cout << "Successfully loaded image: " << str << std::endl;
    }
}  

void alpha_composite(IMAGE* dstimg, int x, int y, IMAGE* srcimg) //同Pillow的 img.alpha_composite
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    int w = srcimg->getwidth();
    int h = srcimg->getheight();
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void alpha_composite_middle(IMAGE* dstimg, int x, int y, IMAGE* srcimg) //alpha_composite 自动居中 anchor = mm
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    int w = srcimg->getwidth();
    int h = srcimg->getheight();
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(dstDC, x - w/2, y - h/2, w, h, srcDC, 0, 0, w, h, bf);
}

IMAGE* scaleImage(IMAGE* srcimg, double scaleFactor) {
    int srcWidth = srcimg->getwidth();
    int srcHeight = srcimg->getheight();
    // 计算放大后的宽度和高度
    int destWidth = static_cast<int>(srcWidth * scaleFactor);
    int destHeight = static_cast<int>(srcHeight * scaleFactor);
    // 创建目标图像
    IMAGE* destimg = new IMAGE(destWidth, destHeight);
    // 获取设备上下文
    HDC srcDC = GetImageHDC(srcimg);
    HDC destDC = GetImageHDC(destimg);
    // 使用 StretchBlt 函数进行图像缩放
    StretchBlt(destDC, 0, 0, destWidth, destHeight, srcDC, 0, 0, srcWidth, srcHeight, SRCCOPY);
    return destimg;
}

