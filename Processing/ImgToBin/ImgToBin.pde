final color PALETTE_16[] = {
  0xFF000000, 0xFFAA00AA, 0xFFFF55FF, 0xFFFF0055, 
  0xFF000055, 0xFF555555, 0xFFAA0055, 0xFFFFAA00, 
  0xFF0055AA, 0xFFAAAAFF, 0xFFAAAAAA, 0xFFFFFF55, 
  0xFF00AAFF, 0xFF005500, 0xFF55FF55, 0xFFFFFFFF};

PImage img;
String imgName = "flame_blue.png";
String filename = "save.txt";
String screenShotName = "img.png";
String indexed[][];
int ratio = 8;

boolean pixVal = false;
boolean grid = true;
boolean bigGrid = true;
int bigGridWidth = 8;
int bigGridHeight = 8;

void settings()
{
  img = loadImage(imgName);
  size(img.width*ratio, img.height*ratio);
}

void setup()
{
  indexed = new String[img.height][img.width];

  background(128);
  textAlign(LEFT, TOP);
  textSize(ratio);
  noStroke();

  for (int i=0; i<img.height; i++)
  {
    for (int j=0; j<img.width; j++)
    {
      color c = img.get(j, i);
      int index = getPaletteIndex(c);
      if (index < 0)
      {
        print("ERROR: incorrect color");
        return;
      }
      indexed[i][j] = hex(index).substring(7, 8);

      fill(c);
      if (grid)
      {
        stroke(128);
      }
      rect(j*ratio, i*ratio, ratio, ratio);
      if (pixVal)
      {
        fill(255 - brightness(c));
        noStroke();
        text(index, j*ratio, i*ratio);
      }
    }
  }

  if (bigGrid)
  {
    for (int i=0; i<img.height/bigGridHeight; i++)
    {
      for (int j=0; j<img.width/bigGridWidth; j++)
      {
        noFill();
        stroke(255);
        rect(j*ratio*bigGridWidth, i*ratio*bigGridHeight, ratio*bigGridWidth, ratio*bigGridHeight);
      }
    }
  }

  String file[] = new String[img.height];
  for (int i=0; i<img.height; i++)
  {
    String tmp = "";
    for (int j=0; j<img.width; j++)
    {
      tmp += indexed[i][j];
      if ((j%2)==1)
      {
        tmp += " ";
      }
    }
    file[i] = tmp;
  }
  saveStrings(filename, file);
  saveFrame(screenShotName);
}

void draw()
{
}

int getPaletteIndex(color c)
{
  for (int i=0; i<PALETTE_16.length; i++)
  {
    if (PALETTE_16[i] == c)
    {
      return i;
    }
  }
  return -1;
}
