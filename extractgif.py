import os
from PIL import Image
import shutil

def extract_frames(gif_path, output_folder):
    # 打开 GIF 图像
    gif = Image.open(gif_path)

    try:
        # 获取 GIF 图像的帧数
        num_frames = gif.n_frames

        # 创建输出目录
        os.makedirs(output_folder, exist_ok=True)

        # 逐帧提取并保存为 PNG
        for i in range(num_frames):
            gif.seek(i)
            frame = gif.convert('RGBA')

            # 创建输出文件名，例如 frame_0.png, frame_1.png, ...
            output_filename = f"{output_folder}/frame_{i}.png"

            # 保存当前帧为 PNG 文件
            frame.save(output_filename, 'PNG')
            print(f"Saved {output_filename}")

    except Exception as e:
        print(f"Error: {e}")

    finally:
        # 关闭 GIF 图像
        gif.close()

def process_gif_folder(input_folder):
    # 递归扫描文件夹
    for root, dirs, files in os.walk(input_folder):
        for file_name in files:
            # 获取文件的完整路径
            file_path = os.path.join(root, file_name)

            # 检查文件是否为 GIF
            if file_name.lower().endswith(".gif"):

                # 创建输出文件夹的路径，例如 root/gifname
                output_folder = os.path.join(root, file_name.split('.')[0])

                # 提取 GIF 帧并保存为 PNG
                extract_frames(file_path, output_folder)

                # 移动原 GIF 到输出文件夹中
                shutil.move(file_path, os.path.join(output_folder, file_name))
                print(f"Moved {file_name} to {output_folder}")

if __name__ == "__main__":
    # 输入文件夹路径
    input_folder = r'C:\Users\27377\Desktop\程序设计基础作业\[0]Project_EasyX\Resources\Zombies'

    # 处理包含子文件夹的 GIF 文件夹
    process_gif_folder(input_folder)
