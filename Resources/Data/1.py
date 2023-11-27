from PIL import Image
import os

def scale_image(input_file, output_file, scale_factor):
    # 打开图片
    with Image.open(input_file) as img:
        # 计算新的大小
        width, height = img.size
        new_width = int(width * scale_factor)
        new_height = int(height * scale_factor)

        # 缩放图片
        scaled_img = img.resize((new_width, new_height), Image.ANTIALIAS)

        # 保存缩放后的图片
        scaled_img.save(output_file)

if __name__ == "__main__":
    # 获取脚本所在目录
    script_directory = os.path.dirname(os.path.abspath(__file__))

    # 输入文件夹路径
    input_folder = os.path.join(script_directory, "Plants")

    # 输出文件夹路径
    output_folder = os.path.join(script_directory, "scaled_images")

    # 缩放因子
    scale_factor = 0.5

    # 遍历输入文件夹中的所有子文件夹
    for folder in os.listdir(input_folder):
        folder_path = os.path.join(input_folder, folder)
        input_file = os.path.join(folder_path, "0", "frame_0.png")
        output_file = os.path.join(folder_path, "0", "scaled.png")

        # 检查文件是否存在
        if os.path.exists(input_file):
            # 创建输出文件夹
            os.makedirs(os.path.join(folder_path, "0"), exist_ok=True)

            # 缩放图片
            scale_image(input_file, output_file, scale_factor)

            print(f"文件夹 {folder} 中的图片已缩放，并保存为 {output_file}")
