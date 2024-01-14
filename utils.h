#include <chrono>

void fillArray(int arr[], int size, int type = 0) {
    if(type == 1){
        const int counts[] = {8, 4, 4};
        const int values[] = {1, 2, 3};

        int index = 0;
        for (int i = 0; i < 3; ++i) {
            std::fill_n(arr + index, counts[i], values[i]);
            index += counts[i];
        }

        std::random_shuffle(arr, arr + size);
    }
    else{
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % 10;
        }
        std::random_shuffle(arr, arr + size);
    }
}  

// 播放音频文件，生成一个随机别名
void PlayAudio(const std::string& audioFilePath) {
    // 生成随机别名
    int randomNumber = rand() * rand();
    std::string alias = "sound_" + std::to_string(randomNumber);

    // 打开和播放音频文件
    std::string command = "open " + audioFilePath + " alias " + alias;
    mciSendStringA(command.c_str(), NULL, 0, NULL);

    command = "play " + alias;
    mciSendStringA(command.c_str(), NULL, 0, NULL);
}