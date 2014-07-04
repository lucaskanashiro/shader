#include <Magick++.h> 
#include <iostream>

using namespace std; 
using namespace Magick;

int main(int argc,char **argv) 
{ 
  InitializeMagick(*argv);

  // Construct the image object. Seperating image construction from the 
  // the read operation ensures that a failure to read the image file 
  // doesn't render the image object useless. 
  Image image;
  try { 
    // Read a file into image object 
    // image.read("/home/luciano/Documents/UnB/ICG/repositorios/InteractiveMaze/Model_Texture/TexturaArcadeEsquedo/Right_Side_Image.jpg");
    image.read("/home/luciano/Documents/UnB/ICG/repositorios/InteractiveMaze/Model_Texture/TexturaArcadeEsquedo/Buttons.jpg");
    // Crop the image to specified size (width, height, xOffset, yOffset)
    // image.crop( Geometry(100,100, 100, 100) );

    // Write the image to a file 
    // image.write( "x.gif" );


    int width = image.columns();
    int height = image.rows();

    cout << "width: " << width << endl;
    cout << "height: " << height << endl;

    Pixels view(image);
    PixelPacket *pixels = view.get(0, 0, width, height);

    for(int i = 0; i < width; i++)
    {
    	for(int j = 0; j < height; j++)
    	{
    		Color color = pixels[width * i + j];
    		cout << "(" << (int)color.redQuantum() << ", ";
    		cout << (int)color.greenQuantum() << ", ";
    		cout << (int)color.blueQuantum() << ")";
    	}
    	cout << endl;
    }


  } 
  catch( Exception &error_ ) 
    { 
      cout << "Caught exception: " << error_.what() << endl; 
      return 1; 
    } 
  return 0; 
}