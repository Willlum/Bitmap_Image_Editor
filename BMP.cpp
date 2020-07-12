#include <string>
#include <fstream>
#include <sstream> 
#include <iostream>
#include <cstring>
#include <math.h>
#include "BMP.h"

    string fileName;
    int width, height, imgSize, bitDepth, offSet, rowSize;
    char imgHeader[BMP_HEADER_SIZE];
    char imgColorTable[BMP_COLOR_TABLE_SIZE];
    char* imgBuffer;

    BMP::BMP(const string &fileName){    
        this->fileName = fileName;       
        imageReader(fileName);
    }
    BMP::BMP(const BMP& bmpObj){
        this -> fileName = bmpObj.fileName;
        this -> width = bmpObj.width;
        this -> height = bmpObj.height;
        this -> imgSize = bmpObj.imgSize;
        this -> bitDepth = bmpObj.bitDepth;
        memcpy(imgHeader, bmpObj.imgHeader, BMP_HEADER_SIZE);
        memcpy(imgColorTable, bmpObj.imgColorTable, BMP_COLOR_TABLE_SIZE);
        imgBuffer = new char[imgSize]; 
        memcpy(imgBuffer, bmpObj.imgBuffer, imgSize);
    }
    BMP::~BMP(){
        delete[] imgBuffer;
        imgBuffer = nullptr;
    }
    ////////////////////////////////////////////////////////
    //                   Image I/O                        //
    ////////////////////////////////////////////////////////

    void BMP::imageReader(const string &fileName){
        ifstream inputFile;

        inputFile.open(fileName, ios::binary);
        if(!inputFile.is_open()){
             cerr << "Unable to connect to file." << endl;
             exit(1);
        }

        //Get bit information from header file
        inputFile.read(imgHeader, BMP_HEADER_SIZE);

        offSet = *(int *) &imgHeader[10];
        width = *(int *) &imgHeader[18];
        height = *(int *) &imgHeader[22];
        bitDepth = *(int *) &imgHeader[28];

        width = floor((width * bitDepth + 31) / 32) * 4; //Adjust for padding for pixel array

        imgSize = width * height;

        //Checks if color table exist
        if(bitDepth <= 8){
            inputFile.read(imgColorTable, BMP_COLOR_TABLE_SIZE);
        }

        imgBuffer = new char[imgSize]; 

        //Read in pixel data to buffer
        inputFile.seekg(offSet, ios::beg);
        inputFile.read(imgBuffer, imgSize);

        // int count = 0;
        // for (size_t i = 0; i < imgSize; i++)
        // {
        //     cout << hex << (int)imgBuffer[i] << " " ;
        //     count++;
        // }
        // cout << dec << endl << count;

        inputFile.close();
    }
    void BMP::imageWriter(string &fileName){
        ofstream outputFile;
        ostringstream copyName;
        copyName << fileName;
        copyName.seekp(-4, ios::end); //Replace .bmp with new copy file extention
        copyName << " - copy.bmp";

        outputFile.open(copyName.str(), ios::binary);
          
        if(!outputFile.is_open()){
            cerr << "Unable to connect to file." << endl;
            exit(2);
        }
        
        outputFile.write(imgHeader, BMP_HEADER_SIZE);

        if(bitDepth <= 8){
            outputFile.write(imgColorTable, BMP_COLOR_TABLE_SIZE);
        }

        outputFile.write(imgBuffer, imgSize);

        outputFile.close();
        cout << endl << "File: " << copyName.str() << " is saved!" << endl;
    }
    void BMP::displayCurrentState(){
        
        ofstream outputFile;
       
        outputFile.open("./tempBMP.bmp", ios::binary);
          
        if(!outputFile.is_open()){
            cerr << "Unable to connect to file." << endl;
            exit(2);
        }
        
        outputFile.write(imgHeader, BMP_HEADER_SIZE);

        if(bitDepth <= 8){
            outputFile.write(imgColorTable, BMP_COLOR_TABLE_SIZE);
        }

        outputFile.write(imgBuffer, imgSize);
       
        outputFile.close();
    }
     void BMP::operator= (const BMP& bmpObj){
        this -> fileName = bmpObj.fileName;
        this -> width = bmpObj.width;
        this -> height = bmpObj.height;
        this -> imgSize = bmpObj.imgSize;
        this -> bitDepth = bmpObj.bitDepth;
        memcpy(imgHeader, bmpObj.imgHeader, BMP_HEADER_SIZE);
        memcpy(imgColorTable, bmpObj.imgColorTable, BMP_COLOR_TABLE_SIZE);
        imgBuffer = new char[imgSize]; 
        memcpy(imgBuffer, bmpObj.imgBuffer, imgSize);
    }
    
    ////////////////////////////////////////////////////////
    //                   Filters                          //
    ////////////////////////////////////////////////////////

    void BMP::imageBinarize(int threshhold){
        for(int i = 0; i < imgSize; i++){
            imgBuffer[i] = (imgBuffer[i] < threshhold) ? WHITE : BLACK;
        }
        // imageWriter("./imgBinarizeCopy.bmp");
    }
    void BMP::imageNegative(){
        int negVal;

        for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                    negVal = 255 - imgBuffer[i*width+j];
                    imgBuffer[i*width+j] = negVal;
            }
        }
        // imageWriter("./imgNegativeCopy.bmp");
    }
    void BMP::imageRotate(int numRot){ 
    
        char tempBuffer[width*height];

            for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                    tempBuffer[i * width - j] = imgBuffer[j * height + i]; //Rotates images 90 degrees to the left
                    // tempBuffer[i * height + j] = imgBuffer[j * width + i]; //Rotates images 90 degrees to the right but doesnt recurse for some reason
                }
            }

        memcpy(imgBuffer, tempBuffer, imgSize);

        if(numRot > 1){
            imageRotate(numRot-1);
        } 
        // imageWriter("./imgRotatedCopy.bmp");
    }
    void BMP::imageMirror(){
        
        char tempBuffer[width*height];
        
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                tempBuffer[height*j+i] = imgBuffer[width*j-i]; //Flips image on yaxis
                }
            }
        memcpy(imgBuffer, tempBuffer, imgSize);
    }
    void BMP::line_detector(){

        int verMask[3][3] = {{-1,2,-1},
                         {-1,2,-1},
                         {-1,2,-1}};

        int horMask[3][3] = {{-1,-1,-1},
                              {2, 2, 2},
                             {-1,-1,-1}};

        int leftDiagMask[3][3] = {{2,-1,-1},
                              {-1,2,-1},
                              {-1,-1,2}};

        int rightDiagMask[3][3] = {{-1,-1,2},
                               {-1,2,-1},
                               {2,-1,-1}};

        char tempBuffer[imgSize];
        int sum;

        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                sum = 0;
                for(int ii = 0; ii < 3; ii++){
                    for(int jj = 0; jj < 3; jj++){
                        sum += imgBuffer[i+ii+(j+jj)*height] * horMask[ii][jj];
                }
        }
        if (sum > 255) sum = 255;
        if (sum < 0) sum = 0;

        tempBuffer[i + j*height] = sum;
                }
            }
        memcpy(imgBuffer, tempBuffer, imgSize);
        // imageWriter("./imgLinesCopy.bmp");
    }   
    void BMP::laplaceConvolve(){
        int di, dj, img, val;
        const int KERNEL_SIZE = 5;
        float mask;
        int laplaceKernel[25] = {-1,-1,-1,-1,-1,
                                -1,-1,-1,-1,-1,
                                -1,-1,24,-1,-1,
                                -1,-1,-1,-1,-1,
                                -1,-1,-1,-1,-1
                                };
        
        char tempBuffer[imgSize];

        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                val = 0;
            for(int ii = 0; ii < KERNEL_SIZE;ii++){
                    for(int jj = 0; jj < KERNEL_SIZE; jj++){
                        di = i - ii;
                        dj = j - jj;
                        if(di >= 0 && dj >= 0) {// Conditional to buffer border pixels since no padding
                            val += laplaceKernel[ii * KERNEL_SIZE + jj]*imgBuffer[dj+di*width];
                        }
                        
                    }
                }

                if(val>255) val = 255;
                if(val<0) val = 0; 

                tempBuffer[j + i*width] = (char)val;
                
            }
        }
        memcpy(imgBuffer, tempBuffer, imgSize);
    }
    void BMP::blurConvolve(){
        int di, dj;
        const int KERNEL_SIZE = 3;
        float mask, img, val;
        
        float blurKernel[9] = { 1/9.0,1/9.0,1/9.0,
                                1/9.0,1/9.0,1/9.0,
                                1/9.0,1/9.0,1/9.0
                            };
        
        char tempBuffer[imgSize];

        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                val = 0;
                for(int ii = 0; ii < KERNEL_SIZE;ii++){
                    for(int jj = 0; jj < KERNEL_SIZE; jj++){
                        di = i - ii;
                        dj = j - jj;
                        if(di >= 0 && dj >= 0) // Buffer border pixels since we have no padding
                            val += (float)blurKernel[ii * KERNEL_SIZE + jj]*imgBuffer[dj+di*width];
                    }
                }

                tempBuffer[j + i*width] = (char)val;
            }
        }
        memcpy(imgBuffer, tempBuffer, imgSize);
    }
    void BMP::imageHistogram(){

        int colorInstance = 0;
        int hist[255] = {0};
        int sum = 0;
        ofstream ofs;

        for(int i = 0; i < width; i++){
            for(int j = 0; j < height; j++){
                colorInstance = imgBuffer[i+j*height];
                if(colorInstance >= 0) hist[colorInstance] += 1;
            }
        }

        ofs.open("image_histogram.txt", ios::out);

        if (!ofs.is_open()){
            cerr << "Unable to connect to file." << endl;
            exit(1);
        }
        
        for(int i = 1; i < 255; i++){
            if(hist[i] != 0){
                 ofs << i << "," << hist[i] << endl;
                 sum += hist[i];
            }
        }
        ofs << sum;

        ofs.close();
    }

    // void imageEquilizer(const char* name, BMP* pic){
    //     long *hist;
    //     long sum, histEqual[255];
    //     BMP pic1;

    //     BMPCopy(pic, &pic1);
    //     hist = imageHistogram(&pic1);

    //     for(int i = 0; i < 255; i++){
    //         sum = 0;
    //         for(int j = 0; j <= i; j++){
    //             sum += hist[j];
    //         }
    //     histEqual[i] = (255 * sum + .5);
    //     }
    //     for(int x = 0; x < pic1.width; x++ ){
    //         for(int y = 0; y < pic1.height; y++){
    //                 pic1.imgBuffer[x+y*pic1.height] = histEqual[pic1.imgBuffer[x+y*pic->height]];
    //         }
    //     }
    //     imageWriter(name, &pic1);
    // }

