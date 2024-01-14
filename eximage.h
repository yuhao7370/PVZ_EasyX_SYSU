#include <graphics.h>  
#include <math.h>
#include <conio.h>
#include <gdiplus.h>
#pragma comment( lib, "MSIMG32.LIB")
#pragma comment(lib, "gdiplus.lib")

#define  PI 3.1415926535897932
using namespace Gdiplus;
// int WindowWidth, WindowHeight;
// WindowWidth = 800;
// WindowHeight = 600;


void loadImageSequence(std::vector<IMAGE>& images, const std::string& path, int frames) {
    for (int i = 0; i < frames; i++) {
        std::string str = path + "/frame_" + std::to_string(i) + ".png";
        loadimage(&images[i], str.c_str());
        // std::cout << "Successfully loaded image: " << str << std::endl;
    }
}  

IMAGE* crop_image(IMAGE* srcimg, int x1, int y1, int x2, int y2) {
    // 计算裁剪区域的宽度和高度
    int crop_width = x2 - x1;
    int crop_height = y2 - y1;

    // 创建新的 IMAGE 对象，用于存储裁剪后的图像
    IMAGE* cropped_img = new IMAGE(crop_width, crop_height);

    // 获取源图像和裁剪后图像的 HDC
    HDC srcDC = GetImageHDC(srcimg);
    HDC croppedDC = GetImageHDC(cropped_img);

    // 使用 BitBlt 函数将裁剪后的图像绘制到新的 IMAGE 对象上
    BitBlt(croppedDC, 0, 0, crop_width, crop_height, srcDC, x1, y1, SRCCOPY);

    // 释放资源
    ReleaseDC(NULL, srcDC);
    ReleaseDC(NULL, croppedDC);

    return cropped_img;
}

void alpha_composite(IMAGE* dstimg, int x, int y, IMAGE* srcimg, BYTE alpha = 255) //同Pillow的 img.alpha_composite
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    int w = srcimg->getwidth();
    int h = srcimg->getheight();
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA };
    AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void alpha_composite_middle(IMAGE* dstimg, int x, int y, IMAGE* srcimg, BYTE alpha = 255) //alpha_composite 自动居中 anchor = mm
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    int w = srcimg->getwidth();
    int h = srcimg->getheight();
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA };
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
