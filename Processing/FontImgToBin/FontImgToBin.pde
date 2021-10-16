PImage img;
int sizeX = 4;
int sizeY = 6;
int fileX = 16;
int fileY = 16;

int ratio = 8;

int size = ((sizeX*fileX) * (sizeY*fileY))/8;
int line = 3;
String data[] = new String[size];
int count = 0;
boolean ox = true;

void settings()
{
  size(fileX*sizeX*ratio, fileY*sizeY*ratio);
}

void setup()
{
  img = loadImage("pix46.png");
  println(size);

  background(128);

  for (int i=1; i<=fileX; i++)
  {
    for (int j=1; j<=fileY; j++)
    {
      int cnt=0;
      String s = "";
      println();
      noFill();
      stroke(255, 0, 0);
      strokeWeight(2);
      rect((j-1)*sizeX*ratio, (i-1)*sizeY*ratio, sizeX*ratio, sizeY*ratio);
      noStroke();
      for (int k=0; k<sizeY; k++)
      {
        for (int l=0; l<sizeX; l++)
        {
          int x = (j-1)*sizeX+l;
          int y = (i-1)*sizeY+k;
          //print(x + "  " + y + "  |");
          fill(img.get(x, y));
          rect(x*ratio, y*ratio, ratio, ratio);
          s += str(int(img.get(x, y) != color(0, 0, 0)));
          cnt++;
          if (cnt == 8)
          {
            //println(s);
            data[count] = hex(unbinary(s)).substring(6, 8);
            count++;
            cnt = 0;
            s = "";
          }
        }
      }
    }
  }

  String newData[] = new String[fileY];
  String s = "";
  count = 0;

  for (int i=0; i<size; i++)
  {
    if (i!=0 && i%(fileX*line)==0)
    {
      println();
      newData[count] = s;
      count++;
      s = "";
    }
    if (i%line == 0)
    {
      print("  ");
      s += "  ";
    }
    if(ox)
    {
      print("0x" + data[i] + ", ");
      s += "0x" + data[i] + ", ";
    }else
    {
      print(data[i] + " ");
      s += data[i] + " ";
    }
  }
  newData[count] = s;

  saveStrings("save.txt", newData);
}

void draw()
{
}
