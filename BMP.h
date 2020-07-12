#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

#define BMP_HEADER_SIZE      54
#define BMP_COLOR_TABLE_SIZE  1024
#define BLACK 255
#define WHITE 0
#define THRESHHOLD 85

using namespace std;

class BMP{
    private:

    public: 
    string fileName;
    int width, height, imgSize, bitDepth;
    char imgHeader[BMP_HEADER_SIZE];
    char imgColorTable[BMP_COLOR_TABLE_SIZE];
    char* imgBuffer;
   
    BMP(const string& fileName); // Explicit means no implicit conversions to be made
    BMP(const BMP& bmpObj);
    ~BMP();
    
    void imageReader(const string& fileName);
    void imageWriter(string& fileName);
    void displayCurrentState();

    // void imageEquilizer(const char*, BMP*);
    void imageBinarize(int threshhold);
    void imageRotate(int imgRot);
    void imageMirror();
    void imageNegative();
    void line_detector();
    void laplaceConvolve();
    void blurConvolve();
    void imageHistogram();
    
    void operator= (const BMP& initPic);
};

#endif // BMP_H_INCLUDED
