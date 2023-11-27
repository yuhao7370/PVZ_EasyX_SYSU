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


def save_json(data, output_file=r"C:\Users\27377\Desktop\程序设计基础作业\[0]Project_EasyX\Resources\folder_structure.json"):
    with open(output_file, "w") as json_file:
        json.dump(data, json_file, indent=2)


if __name__ == "__main__":
    folder_path = r"C:\Users\27377\Desktop\程序设计基础作业\[0]Project_EasyX\Resources\Zombies"  # 将此路径替换为实际的文件夹路径
    json_data = {}
    
    try:
        with os.scandir(folder_path) as entries:
            gif_frame_count = None
            
            for entry in entries:
                if(entry.name == "Thumbs.db"):
                    continue
                # print(entry.name)
                json_data[entry.name] = {"items":{}}
                if entry.is_dir():
                    dirpath = entry.path
                    with os.scandir(dirpath) as subentries:
                        for file in subentries:
                            if(file.is_dir()):
                                framecount = get_gif_frame_count(os.path.join(dirpath, file.name, file.name + ".gif"))
                                # if(framecount != 1):
                                json_data[entry.name]["items"][file.name]={"isgif": True, "framecount": framecount}
                                # else:
                                #     json_data[entry.name]["items"].append({"name": file.name, "isgif": False})
                            elif(file.is_file() and file.name.lower().endswith(".png")):
                                json_data[entry.name]["items"][file.name.split(".png")[0]].append({"isgif": False})
                                
                        
            #     if entry.is_file():
            #         # 如果是文件，添加到 children 列表中
            #         # result["children"].append({"name": entry.name, "type": "file"})
            #         # 如果文件是同名的GIF文件，获取帧数
            #         if entry.name.lower().endswith(".gif") and entry.name.lower() == os.path.basename(folder_path).lower() + ".gif":
            #             gif_frame_count = get_gif_frame_count(entry.path)
            #     elif entry.is_dir():
            #         # 如果是文件夹，递归调用生成子文件夹的结构
            #         result["children"].append(generate_folder_structure_json(entry.path))

            # # 如果存在同名的GIF文件，添加帧数信息
            # if gif_frame_count is not None:
            #     result["framecount"] = gif_frame_count

    except Exception as e:
        print(f"Error scanning {folder_path}: {e}")
        
    save_json(json_data)
    print("JSON structure saved to folder_structure.json")
