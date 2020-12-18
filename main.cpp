// Questions:
// 1) BMP file I/O 
// 2) What's best way to add color channels 
// 3) Resources for png file format
// 4) rotating and blur not really working (maybe related to file I/O)

#include <iostream>
#include <string>
#include<stack>
#include "BMP.h"

#define EXIT -1
#define DEGREE 248

using namespace std;
 
int main() {
    // string file = "./images/lighthouse.bmp";  
    // string file = "./images/jerbear.bmp";
    // string file = "./images/mercury.bmp";
    // string file = "./images/gradient.bmp";
    // string file = "./images/man.bmp";
    string file = "./images/lena512.bmp";
    int input = 0, numRot = 0, threshhold = 0, brightness = 0;
    BMP pic(file);
    stack<BMP> bmpStack;
    
    bmpStack.emplace(pic);
    pic.displayCurrentState();
    pic.imageHistogram();

    while(true){
        input = 0;
      
        cout << "\nEnter filter to apply or press 'q' to quit: " << endl;
        cout << "1: Undo" << endl;
        cout << "2: Laplace Convolution" << endl;
        cout << "3: Blur Convolution" << endl;
        cout << "4: Rotate" << endl;
        cout << "5: Mirror Horizontal" << endl;
        cout << "6: Binarize" << endl;
        cout << "7: Negative" << endl;
        cout << "8: Equalize" << endl;
        cout << "9: Brightness" << endl;
        cout << "10 : Save As BMP" << endl;
        cin >> input;

        switch (input){
            case 1:
                if(bmpStack.size() > 1){
                    bmpStack.pop();
                    pic = bmpStack.top();
            }
                 else{
                    cout << endl << "Error: Stack is empty!!!" << endl;
            }
                break;
            case 2:
                pic.laplaceConvolve();
                bmpStack.push(pic);
                break;
            case 3:
                pic.blurConvolve();
                bmpStack.push(pic);
                break;
            case 4: 
                cout << "Enter number of rotations(Each 90" << (char)DEGREE << " to the left)" << endl;
                cin >> numRot;
                pic.imageRotate(numRot);
                bmpStack.push(pic);
                break;
            case 5:
                pic.imageMirror();
                bmpStack.push(pic);
                break;
            case 6:
                cout << "Enter number for threshhold for white level(70 - 100)" << endl;
                cin >> threshhold;
                pic.imageBinarize(threshhold);
                bmpStack.push(pic);
                break;
            case 7:
                pic.imageNegative();
                bmpStack.push(pic);
                break;
            case 8:
                pic.imageEquilizer();
                bmpStack.push(pic);
                break;
            case 9:
                cout << "Enter number for brightness between -127 and l27" << endl;
                cin >> brightness;
                pic.brightness(brightness);
                bmpStack.push(pic);
                break;
            case 10:
               pic.imageWriter(pic.fileName);
               break;
            default: 
                input = EXIT;
                break;
        }
        bmpStack.top().displayCurrentState();
        pic.imageHistogram();
        if(input == EXIT) break;
    }
   
    return 0;
}// end main() 