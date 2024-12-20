#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

// Função para redimensionar a imagem usando interpolação bilinear
Mat bilinearResize(const Mat &img, int newWidth, int newHeight) {
    Mat resized;
    resize(img, resized, Size(newWidth, newHeight), 0, 0, INTER_LINEAR);
    return resized;
}

void processImage(const string &inputPath, const string &outputDir, double originalDpi, double reducedDpi, double restoredDpi) {
    Mat img = imread(inputPath, IMREAD_GRAYSCALE);
    if (img.empty()) {
        cerr << "Erro ao abrir a imagem: " << inputPath << endl;
        return;
    }

    // Reduzir a imagem
    double scaleDown = reducedDpi / originalDpi;
    int reducedWidth = static_cast<int>(img.cols * scaleDown);
    int reducedHeight = static_cast<int>(img.rows * scaleDown);
    Mat reducedImg = bilinearResize(img, reducedWidth, reducedHeight);

    // Ampliar a imagem reduzida de volta
    double scaleUp = restoredDpi / reducedDpi;
    int restoredWidth = static_cast<int>(reducedImg.cols * scaleUp);
    int restoredHeight = static_cast<int>(reducedImg.rows * scaleUp);
    Mat restoredImg = bilinearResize(reducedImg, restoredWidth, restoredHeight);

    string imgDir = outputDir + "/Imagens transformadas";
    fs::create_directories(imgDir);

    string baseName = fs::path(inputPath).stem().string();
    imwrite(imgDir + "/" + baseName + "_original.png", img);
    imwrite(imgDir + "/" + baseName + "_reduced.png", reducedImg);
    imwrite(imgDir + "/" + baseName + "_restored.png", restoredImg);

    cout << "Processamento concluído para: " << inputPath << endl;
}

int main() {
    cout << "Iniciando o programa..." << endl;

    vector<string> inputImages = {"input/input1.tif", "input/input2.tif", "input/input3.tif"};
    string outputDir = "output";
    double originalDpi = 1250.0;
    double reducedDpi = 100.0;
    double restoredDpi = 1250.0;

    fs::create_directories(outputDir);

    for (const string &imgPath : inputImages) {
        processImage(imgPath, outputDir, originalDpi, reducedDpi, restoredDpi);
    }

    cout << "Processo concluído para todas as imagens." << endl;
    return 0;
}
