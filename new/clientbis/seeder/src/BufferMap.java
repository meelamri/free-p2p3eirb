import java.util.*;
import java.io.*;

public class BufferMap {
  private ArrayList<String> buffer;
  private int id;
  private int piecesNumber;
  byte [] buffBytes;
  public BufferMap(int _piecesNumber, boolean all, int _id) {
	  this.piecesNumber = _piecesNumber;
    this.id = _id;

	  int size = this.sizeBuffer();
	  buffer = new ArrayList<String>();

	  for (int i = 0; i < size; ++i)
      if(all)
	      this.buffer.add("255");
       else
        this.buffer.add("0");

    for(int j = _piecesNumber + 1; j <= size * 8; j++)
      this.setBuffer(j, false);
  }

  public BufferMap(String buf) {
    buf = buf.replace("[", "");
    buf = buf.replace("]", "");
    buf = buf.replace(" ", "");
    List<String> buffer = new ArrayList<String>(Arrays.asList(buf.split(",")));
    //String[] b = buf.split(", ");
    int size =  buffer.size();
    //System.out.println(buffer);

    for (int i = 0; i < size; i++)
      for (int j = 1; j != 0; j <<= 1)
        if ((j & Integer.parseInt(buffer.get(i))) != 0)
  	      this.piecesNumber ++;
  }
  public BufferMap(byte []buffBytes) {
      this.buffBytes = buffBytes;
  }

  public int computeNbPieces() {
    int i = 0, j = 1, cmp = 0;
    int size = buffer.size();
    for (i = 0; i < size; i++)
      for (j = 1; j != 0; j <<= 1)
        if ((j & Integer.parseInt(buffer.get(i))) != 0)
  	       cmp ++;
    return cmp;
  }

  public ArrayList<String> getBuffer(){
	    return buffer;
  }

  public int getID(){
	    return this.id;
  }

  public int getPiecesNumber(){
	    return this.piecesNumber;
  }

  public int sizeBuffer(){
    if(this.piecesNumber % 8 == 0)
	    return (int) Math.ceil(this.piecesNumber/8);
    else
      return (int) Math.ceil(this.piecesNumber/8) + 1;
  }

  public void setBuffer(int part, boolean b) {
    part--;
    int c = Integer.valueOf(this.buffer.get(vectorPosition(part)));
	  if(b) {
      String i = Integer.toString(c | 1 << (7 - bitPosition(part)));
      buffer.set(vectorPosition(part), i);
    }
    else {
      String i = Integer.toString(c & ~(1 << (7 - bitPosition(part))));
      buffer.set(vectorPosition(part), i);
	  }
  }

  public int vectorPosition(int pos) {
	    return pos / 8;
  }

  public int bitPosition(int pos) {
	    return pos % 8;
  }
  public boolean getValue(int pos) {
    pos--;
    int c = Integer.valueOf(this.buffer.get(vectorPosition(pos)));
    int i = c & 1 << (7  - bitPosition(pos));
    return (i != 0) ? true : false;
  }
}
