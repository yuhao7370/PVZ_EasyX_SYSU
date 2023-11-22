import os
import json

def get_gif_frame_count(gif_path):
    try:
        # 使用Pillow库打开GIF文件
        from PIL import Image
        with Image.open(gif_path) as img:
            return img.n_frames
    except Exception as e:
        print(f"Error getting frame count for {gif_path}: {e}")
        return None

def generate_folder_structure_json(folder_path):
    result = {"name": os.path.basename(folder_path), "type": "folder", "children": []}

    try:
        with os.scandir(folder_path) as entries:
            gif_frame_count = None

            for entry in entries:
                if entry.is_file():
                    # 如果是文件，添加到 children 列表中
                    # result["children"].append({"name": entry.name, "type": "file"})
                    # 如果文件是同名的GIF文件，获取帧数
                    if entry.name.lower().endswith(".gif") and entry.name.lower() == os.path.basename(folder_path).lower() + ".gif":
                        gif_frame_count = get_gif_frame_count(entry.path)
                elif entry.is_dir():
                    # 如果是文件夹，递归调用生成子文件夹的结构
                    result["children"].append(generate_folder_structure_json(entry.path))

            # 如果存在同名的GIF文件，添加帧数信息
            if gif_frame_count is not None:
                result["framecount"] = gif_frame_count

    except Exception as e:
        print(f"Error scanning {folder_path}: {e}")

    return result

def save_json(data, output_file="folder_structure.json"):
    with open(output_file, "w") as json_file:
        json.dump(data, json_file, indent=2)


if __name__ == "__main__":
    folder_path = r"C:\Users\27377\Desktop\程序设计基础作业\[0]Project_EasyX\Resources\Plants"  # 将此路径替换为实际的文件夹路径
    json_data = generate_folder_structure_json(folder_path)
    save_json(json_data)
    print("JSON structure saved to folder_structure.json")
