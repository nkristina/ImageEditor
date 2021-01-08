# ImageEditor
ImageEditor is an object-oriented program written in C++. It is a class that allows the user to manipulate the loaded picture. Users can flip picture (vertically or horizontally), add new layers to the original picture and then draw on them or delete them, change colors and opacity of the pixels, crop or blur picture...
 
 Content:
 
 - Main.cpp - cals some functions of the class ImageEditor on the pictures for testing, with option of adding users original picture.
 - Piksel.h - header file with definition of class Piksel
 - Piksel.cpp - file with bodies of functions of class Piksel
 - Layer.h - header file with definision of structure Layer
 - ImageEditor.h - header file with definition of class ImageEditior
 - ImageEditor.cpp - file with bodies of functions of class ImageEditor
 - Test.obj - file which converts bmp format to format used in ImageEditor
 - Pictures for testing and result pictures
 
# Testing on your exampe
If you want to test the code on your example, after you cloned the repo and made a project with: Test.obj, .cpp, .h files in it, you need to add your picture to the repo folder. It is important to name it "**MyExampe.bmp**".

In min.cpp your example is under case 0. There you can choose what to do with your picture. 

The ouptput will be called "MyExample_RESULT.bmp", and you can find it in repo folder.
