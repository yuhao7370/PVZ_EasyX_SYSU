import os
from PIL import Image

# 获取当前脚本所在目录
current_dir = os.path.dirname(os.path.abspath(__file__))

# 创建cold文件夹
cold_dir = os.path.join(current_dir, 'cold')
os.makedirs(cold_dir, exist_ok=True)

# 遍历同根目录下的png图片
for filename in os.listdir(current_dir):
    if filename.endswith('.png'):
        # 打开图片
        image_path = os.path.join(current_dir, filename)
        image = Image.open(image_path)

        # 将不透明部分变成蓝色
        image = image.convert('RGBA')
        data = image.getdata()
        new_data = []
        for item in data:
            if item[3] > 0:
                new_data.append((0, 0, 255, item[3]))
            else:
                new_data.append(item)
        image.putdata(new_data)

        # 保存处理后的图片到cold文件夹
        new_filename = os.path.join(cold_dir, filename)
        image.save(new_filename)

        # 关闭图片
        image.close()
